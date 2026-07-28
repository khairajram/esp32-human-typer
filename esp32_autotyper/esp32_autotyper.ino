/**
 * ESP32 Human Typer Firmware
 * 
 * This sketch configures the ESP32 as a Bluetooth LE Keyboard.
 * It listens for key press/release commands over the serial port (USB)
 * and relays them via Bluetooth to the connected target PC.
 * 
 * Required Library:
 * - ESP32-BLE-Keyboard (by T-vK): https://github.com/T-vK/ESP32-BLE-Keyboard
 * 
 * Command Protocol:
 * - P<keycode>\n  : Press key with specified decimal ASCII/Keycode
 * - R<keycode>\n  : Release key with specified decimal ASCII/Keycode
 * - A0\n          : Release all keys
 */

#include <BleKeyboard.h>

// Create BLE Keyboard instance
// Device Name: ESP32 Human Typer
// Manufacturer: DeepMind
// Battery level: 100%
BleKeyboard bleKeyboard("ESP32 Human Typer", "DeepMind", 100);

bool wasConnected = false;

void setup() {
  // Initialize USB Serial interface for communication with the Web UI
  Serial.begin(115200);
  
  // Start the BLE Keyboard service
  bleKeyboard.begin();
  
  // Send ready message
  Serial.println("INFO:ESP32 Firmware Initialized. Waiting for connection...");
}

void loop() {
  // Check Bluetooth connection status
  bool isConnected = bleKeyboard.isConnected();
  
  // Detect connection status changes and report back to Web UI
  if (isConnected != wasConnected) {
    if (isConnected) {
      Serial.println("STATUS:CONNECTED");
    } else {
      Serial.println("STATUS:DISCONNECTED");
    }
    wasConnected = isConnected;
  }
  
  // Read and process incoming commands from Web Serial
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim(); // Remove leading/trailing whitespaces or carriage returns (\r)
    
    if (cmd.length() >= 2) {
      char action = cmd.charAt(0);
      int keycode = cmd.substring(1).toInt();
      
      if (isConnected) {
        if (action == 'P') {
          bleKeyboard.press((uint8_t)keycode);
        } 
        else if (action == 'R') {
          bleKeyboard.release((uint8_t)keycode);
        } 
        else if (action == 'A') {
          bleKeyboard.releaseAll();
        }
      } else {
        // Send error if web page tries to type while BLE is disconnected
        Serial.println("ERROR:BLE_NOT_CONNECTED");
      }
    }
  }
  
  // A tiny delay to maintain board stability and prevent CPU watchdog resets
  delay(1);
}
