/**
 * ESP32 Human Typer (Wireless AP Mode)
 * 
 * This sketch configures the ESP32 as a Bluetooth LE Keyboard and a Wi-Fi Access Point.
 * It hosts a Web Server serving the control UI directly to your phone or PC.
 * 
 * Required Library:
 * - ESP32-BLE-Keyboard (by T-vK): https://github.com/T-vK/ESP32-BLE-Keyboard
 */

#include <WiFi.h>
#include <WebServer.h>
#include <BleKeyboard.h>
#include "html_page.h"

// Initialize BLE Keyboard
BleKeyboard bleKeyboard("ESP32 Human Typer", "DeepMind", 100);

// Initialize port 80 Web Server
WebServer server(80);

// Background typing task parameters
String queue = "";
bool isTyping = false;
bool isPaused = false;
int queueIndex = 0;

TaskHandle_t typingTaskHandle = NULL;

// Dual-Core Task: Runs on Core 0 to type in background without freezing the Wi-Fi client
void typingTask(void *pvParameters) {
  while (true) {
    if (isTyping && !isPaused && queueIndex < queue.length()) {
      int nextSemi = queue.indexOf(';', queueIndex);
      if (nextSemi != -1) {
        String cmd = queue.substring(queueIndex, nextSemi);
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
            if (delayMs > 0) {
              delay(delayMs);
            }
          }
        }
      } else {
        isTyping = false;
        bleKeyboard.releaseAll();
      }
    } else {
      delay(10); // Check again in 10ms
    }
  }
}

void handleRoot() {
  server.send(200, "text/html", html_page);
}

void handleStart() {
  if (server.hasArg("plain")) {
    queue = server.arg("plain");
    queueIndex = 0;
    isTyping = true;
    isPaused = false;
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing Command Queue");
  }
}

void handleStop() {
  isTyping = false;
  isPaused = false;
  bleKeyboard.releaseAll();
  server.send(200, "text/plain", "OK");
}

void handleStatus() {
  String status = "idle";
  if (isTyping) {
    status = isPaused ? "paused" : "typing";
  }
  server.send(200, "text/plain", status);
}

void setup() {
  Serial.begin(115200);
  
  // 1. Start Wi-Fi Access Point
  // Network SSID: ESP32-AutoTyper
  // Password: 12345678 (8-character minimum)
  WiFi.softAP("ESP32-AutoTyper", "12345678");
  Serial.println("\nWiFi AP Started.");
  Serial.print("SSID: ESP32-AutoTyper\nPassword: 12345678\nIP Address: ");
  Serial.println(WiFi.softAPIP());
  
  // 2. Start BLE Keyboard
  bleKeyboard.begin();
  
  // 3. Configure Web Server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/start", HTTP_POST, handleStart);
  server.on("/stop", HTTP_POST, handleStop);
  server.on("/status", HTTP_GET, handleStatus);
  
  server.begin();
  Serial.println("Web Server running on port 80.");
  
  // 4. Create background typing thread on Core 0
  xTaskCreatePinnedToCore(
    typingTask,
    "typingTask",
    8192,
    NULL,
    1,
    &typingTaskHandle,
    0
  );
}

void loop() {
  // Keep serving clients on Core 1
  server.handleClient();
  delay(1);
}
