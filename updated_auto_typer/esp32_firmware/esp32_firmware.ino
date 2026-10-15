/**
 * ============================================================================
 * ESP32 MULTI-DEVICE AUTO-TYPER FIRMWARE (Central Server Client)
 * ============================================================================
 * 
 * Connects to your Wi-Fi network and establishes a WebSocket connection with
 * the Central Server. Simulates human typing on a target PC via Bluetooth LE.
 * 
 * Required Libraries (Install via Arduino Library Manager):
 *  1. "ESP32-BLE-Keyboard" by T-vK (v0.3.2+)
 *  2. "ArduinoWebsockets" by Gil Maimon (v0.5.3+)
 * 
 * Board: ESP32 Dev Module (Core v2.0.x or v3.0.x compatible)
 * ============================================================================
 */

#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <BleKeyboard.h>

using namespace websockets;

// ============================================================================
// CONFIGURATION: SET YOUR WI-FI, SERVER IP, AND UNIT IDENTITY
// ============================================================================
const char* WIFI_SSID      = "YOUR_WIFI_NAME";        // <-- Put your Wi-Fi Name here
const char* WIFI_PASSWORD  = "YOUR_WIFI_PASSWORD";    // <-- Put your Wi-Fi Password here
const char* SERVER_HOST    = "10.208.114.32";         // <-- PC's actual Wi-Fi IP address
const uint16_t SERVER_PORT = 3000;                    // <-- Central Server Port

// Select Unit Identity: Uncomment UNIT_1 for first ESP32, or UNIT_2 for second ESP32
#define UNIT_1
// #define UNIT_2

#if defined(UNIT_1)
  const char* DEVICE_ID         = "ESP_ALPHA";
  const char* DEVICE_NAME       = "ESP32 Unit 1 (Alpha)";
  const char* BLE_KEYBOARD_NAME = "ESP32 Typer - Alpha";
#elif defined(UNIT_2)
  const char* DEVICE_ID         = "ESP_BETA";
  const char* DEVICE_NAME       = "ESP32 Unit 2 (Beta)";
  const char* BLE_KEYBOARD_NAME = "ESP32 Typer - Beta";
#endif
// ============================================================================

// Global Objects
BleKeyboard bleKeyboard(BLE_KEYBOARD_NAME, "DeepMind", 100);
WebsocketsClient wsClient;

// Non-blocking Queue State Machine
String typingQueue = "";
bool isTyping = false;
bool isPaused = false;
int queueIndex = 0;
unsigned long nextActionTime = 0;

// Connection & Telemetry Timers
bool lastBleConnected = false;
unsigned long lastStatusCheck = 0;
unsigned long lastReconnectAttempt = 0;

// Helper: Extract JSON value without external library dependencies
String extractJsonString(const String& json, const String& key) {
  String searchKey = "\"" + key + "\":\"";
  int startIdx = json.indexOf(searchKey);
  if (startIdx == -1) return "";
  startIdx += searchKey.length();
  int endIdx = json.indexOf("\"", startIdx);
  if (endIdx == -1) return "";
  return json.substring(startIdx, endIdx);
}

// Send Status Update to Central Server
void sendStatusUpdate(const String& state) {
  if (wsClient.available()) {
    String payload = "{";
    payload += "\"type\":\"STATUS_UPDATE\",";
    payload += "\"deviceId\":\"" + String(DEVICE_ID) + "\",";
    payload += "\"bleConnected\":" + String(bleKeyboard.isConnected() ? "true" : "false") + ",";
    payload += "\"state\":\"" + state + "\"";
    payload += "}";
    wsClient.send(payload);
  }
}

// Send Registration Handshake upon WebSocket connection
void sendRegistration() {
  String payload = "{";
  payload += "\"type\":\"REGISTER\",";
  payload += "\"deviceId\":\"" + String(DEVICE_ID) + "\",";
  payload += "\"name\":\"" + String(DEVICE_NAME) + "\",";
  payload += "\"bleName\":\"" + String(BLE_KEYBOARD_NAME) + "\",";
  payload += "\"bleConnected\":" + String(bleKeyboard.isConnected() ? "true" : "false") + ",";
  payload += "\"ip\":\"" + WiFi.localIP().toString() + "\"";
  payload += "}";
  wsClient.send(payload);
  Serial.println("[+] Registration handshake sent to Central Server");
}

// WebSocket Event Callbacks
void onMessageCallback(WebsocketsMessage message) {
  String data = message.data();

  // 1. Execute Typing Queue
  if (data.indexOf("\"EXECUTE_QUEUE\"") != -1) {
    String queue = extractJsonString(data, "queue");
    if (queue.length() > 0) {
      Serial.printf("[->] Received typing queue (%d bytes)\n", queue.length());
      typingQueue = queue;
      queueIndex = 0;
      isTyping = true;
      isPaused = false;
      nextActionTime = millis(); // Start immediately

      if (!bleKeyboard.isConnected()) {
        Serial.println("[!] WARNING: Bluetooth is NOT connected to the target PC!");
        Serial.printf("    Please open Bluetooth settings on target PC and pair with '%s'\n", BLE_KEYBOARD_NAME);
      } else {
        Serial.println("[+] Target PC is connected over Bluetooth. Typing now...");
      }

      sendStatusUpdate("typing");
    }
  }
  // 2. Pause Typing Queue
  else if (data.indexOf("\"PAUSE_QUEUE\"") != -1) {
    Serial.println("[||] Received PAUSE request from server");
    isPaused = true;
    bleKeyboard.releaseAll();
    sendStatusUpdate("paused");
  }
  // 3. Resume Typing Queue
  else if (data.indexOf("\"RESUME_QUEUE\"") != -1) {
    Serial.println("[>] Received RESUME request from server");
    isPaused = false;
    nextActionTime = millis() + 50;
    sendStatusUpdate("typing");
  }
  // 4. Abort / Stop Typing
  else if (data.indexOf("\"ABORT_QUEUE\"") != -1) {
    Serial.println("[!] Received abort request from server");
    isTyping = false;
    isPaused = false;
    typingQueue = "";
    queueIndex = 0;
    bleKeyboard.releaseAll();
    sendStatusUpdate("idle");
  }
  // 5. Heartbeat Ping
  else if (data.indexOf("\"PING\"") != -1) {
    wsClient.send("{\"type\":\"PONG\"}");
  }
}

void onEventsCallback(WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("[+] WebSocket connection established with Central Server!");
    sendRegistration();
  } else if (event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("[-] WebSocket connection closed");
  } else if (event == WebsocketsEvent::GotPing) {
    wsClient.pong();
  }
}

void connectToWebSocket() {
  String cleanHost = SERVER_HOST;
  cleanHost.replace("http://", "");
  cleanHost.replace("https://", "");
  cleanHost.replace("ws://", "");
  cleanHost.replace("wss://", "");
  while (cleanHost.endsWith("/")) {
    cleanHost.remove(cleanHost.length() - 1);
  }

  Serial.printf("[...] Connecting to WebSocket server at %s:%d/ws/device ...\n", cleanHost.c_str(), SERVER_PORT);
  bool success = wsClient.connect(cleanHost.c_str(), SERVER_PORT, "/ws/device");
  if (success) {
    Serial.println("[+] SUCCESS: WebSocket connected!");
  } else {
    Serial.println("[-] FAILED: Could not reach WebSocket server.");
    Serial.println("    1. Is the Node.js server running on your PC?");
    Serial.printf("    2. Is your PC Wi-Fi IP '%s'?\n", cleanHost.c_str());
    Serial.println("    3. Did Windows Firewall block port 3000?");
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n==================================================");
  Serial.printf(" Starting %s\n", DEVICE_NAME);
  Serial.printf(" Bluetooth BLE Name: %s\n", BLE_KEYBOARD_NAME);
  Serial.println("==================================================");

  // Initialize Bluetooth LE Keyboard
  bleKeyboard.begin();

  // Connect to Wi-Fi
  Serial.printf("[...] Connecting to Wi-Fi: %s\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[+] Wi-Fi Connected!");
  Serial.print("    IP Address: ");
  Serial.println(WiFi.localIP());

  // Setup WebSocket Client
  wsClient.onMessage(onMessageCallback);
  wsClient.onEvent(onEventsCallback);
  connectToWebSocket();
}

void loop() {
  unsigned long now = millis();

  // 1. Check Wi-Fi Connection
  if (WiFi.status() != WL_CONNECTED) {
    delay(500);
    return;
  }

  // 2. Poll WebSocket & Auto-Reconnect
  if (wsClient.available()) {
    wsClient.poll();
  } else {
    if (now - lastReconnectAttempt > 3000) {
      lastReconnectAttempt = now;
      connectToWebSocket();
    }
  }

  // 3. Monitor Bluetooth Connection Status changes
  if (now - lastStatusCheck > 1000) {
    lastStatusCheck = now;
    bool currentBleState = bleKeyboard.isConnected();
    if (currentBleState != lastBleConnected) {
      lastBleConnected = currentBleState;
      Serial.printf("[BLE] Status changed: %s\n", currentBleState ? "CONNECTED TO PC" : "DISCONNECTED");
      sendStatusUpdate(isTyping ? "typing" : "idle");
    }
  }

  // 4. Non-blocking Typing State Machine
  if (isTyping && !isPaused && now >= nextActionTime) {
    if (queueIndex < typingQueue.length()) {
      int nextSemi = typingQueue.indexOf(';', queueIndex);
      if (nextSemi != -1) {
        String cmd = typingQueue.substring(queueIndex, nextSemi);
        queueIndex = nextSemi + 1;

        if (cmd.length() > 1) {
          char action = cmd.charAt(0);
          int comma = cmd.indexOf(',');
          if (comma != -1) {
            int keycode = cmd.substring(1, comma).toInt();
            int delayMs = cmd.substring(comma + 1).toInt();

            if (bleKeyboard.isConnected()) {
              if (action == 'P') {
                bleKeyboard.press((uint8_t)keycode);
              } else if (action == 'R') {
                bleKeyboard.release((uint8_t)keycode);
              } else if (action == 'A') {
                bleKeyboard.releaseAll();
              }
            }

            // Schedule next event with exact human delay
            nextActionTime = millis() + (delayMs > 0 ? delayMs : 2);
          }
        }
      } else {
        // Queue finished
        isTyping = false;
        typingQueue = "";
        queueIndex = 0;
        bleKeyboard.releaseAll();
        Serial.println("[+] Typing completed successfully!");
        sendStatusUpdate("idle");
      }
    } else {
      isTyping = false;
      typingQueue = "";
      queueIndex = 0;
      bleKeyboard.releaseAll();
      Serial.println("[+] Typing completed successfully!");
      sendStatusUpdate("idle");
    }
  }

  delay(1);
}
