# Multi-ESP32 Centralized Human Auto-Typer (v2.0)

A centralized, multi-device hardware keystroke automation ecosystem. An Express + WebSocket Central Server acts as the core hub connecting any number of ESP32 Bluetooth LE hardware typers and web control dashboards across your local network.

---

## 🏛️ System Architecture

```
                  ┌──────────────────────────────────────────────────┐
                  │               Web Dashboard                      │
                  │  (Browser: PC, Tablet, or Mobile Phone)          │
                  └──────────────────────────────────────────────────┘
                                        ▲
                                        │ WebSocket (ws://)
                                        │ / HTTP Static Serving
                                        ▼
                  ┌──────────────────────────────────────────────────┐
                  │                 Central Server                   │
                  │        (Node.js Express + WebSocket Server)      │
                  │                                                  │
                  │  - Device Registry & Heartbeats                  │
                  │  - Routes commands to target ESP32               │
                  │  - Broadcasts device status & telemetry to Web   │
                  └──────────────────────────────────────────────────┘
                            /                                \
      WebSocket (ws://)    /                                  \    WebSocket (ws://)
                          ▼                                    ▼
          ┌─────────────────────────────┐        ┌─────────────────────────────┐
          │         ESP32 Unit 1        │        │         ESP32 Unit 2        │
          │       (e.g., ESP_ALPHA)     │        │       (e.g., ESP_BETA)      │
          │  - Wi-Fi Station Mode       │        │  - Wi-Fi Station Mode       │
          │  - WebSocket Client         │        │  - WebSocket Client         │
          │  - BLE HID Keyboard         │        │  - BLE HID Keyboard         │
          │  - Dual-Core FreeRTOS Task  │        │  - Dual-Core FreeRTOS Task  │
          └─────────────────────────────┘        └─────────────────────────────┘
                         │                                      │
                         │ Bluetooth BLE                        │ Bluetooth BLE
                         ▼                                      ▼
          ┌─────────────────────────────┐        ┌─────────────────────────────┐
          │          Target PC 1        │        │          Target PC 2        │
          │   (Receives Keystrokes)     │        │   (Receives Keystrokes)     │
          └─────────────────────────────┘        └─────────────────────────────┘
```

---

## 📁 Project Structure

```
d:\auto_type\updated_auto_typer\
├── server\
│   ├── package.json          # Server dependencies (express, ws)
│   └── server.js             # Central WebSocket router & HTTP static file server
├── public\
│   ├── index.html            # Multi-device dashboard UI
│   ├── style.css             # Glassmorphic dark styling & multi-device cards
│   ├── app.js                # Frontend simulator (141k human typing model + WS client)
│   └── typing_model_params.json
├── esp32_firmware\
│   └── esp32_firmware.ino    # Multi-unit firmware (BLE Keyboard + WebSocket client)
└── README.md
```

---

## 🚀 Step-by-Step Setup Guide

### Step 1: Start the Central Server
1. Open PowerShell or Command Prompt.
2. Navigate to the server folder:
   ```powershell
   cd d:\auto_type\updated_auto_typer\server
   ```
3. Run the server:
   ```powershell
   npm start
   ```
4. The terminal will print your **LAN Access URL** and **ESP32 WebSocket URL** (e.g., `ws://192.168.1.100:3000/ws/device`). Note this IP address for Step 2.

---

### Step 2: Flash ESP32 Unit #1 and Unit #2

In Arduino IDE, install the required libraries if you haven't already:
1. `ESP32-BLE-Keyboard` by T-vK
2. `ArduinoWebsockets` by Gil Maimon (v0.5.3+)

Open `d:\auto_type\updated_auto_typer\esp32_firmware\esp32_firmware.ino`:

#### For ESP32 #1:
1. Set your Wi-Fi name and password:
   ```cpp
   const char* WIFI_SSID     = "YOUR_WIFI_NAME";
   const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
   const char* SERVER_HOST   = "192.168.1.100"; // Server IP from Step 1
   ```
2. Make sure `UNIT_1` is uncommented:
   ```cpp
   #define UNIT_1
   // #define UNIT_2
   ```
3. Connect ESP32 #1 and click **Upload**.

#### For ESP32 #2:
1. Open the same file and comment `UNIT_1`, and uncomment `UNIT_2`:
   ```cpp
   // #define UNIT_1
   #define UNIT_2
   ```
2. Connect ESP32 #2 and click **Upload**.

---

### Step 3: Pair the ESP32s to their Target Computers
* **Target Computer 1**: Pair with Bluetooth device **`ESP32 Typer - Alpha`**.
* **Target Computer 2**: Pair with Bluetooth device **`ESP32 Typer - Beta`**.

---

### Step 4: Open the Web Dashboard
1. Open any browser (on your PC, phone, or tablet) and go to:
   `http://localhost:3000` (or `http://<YOUR_SERVER_IP>:3000`).
2. Both ESP32 cards will appear with live status:
   * **Online / Offline**: Real-time Wi-Fi and WebSocket status.
   * **Bluetooth**: Shows whether each ESP32 is currently paired and connected to its target PC.
3. Select your **Target Destination**:
   * `🔵 ESP32 #1 (Alpha)`: Keystrokes sent only to PC 1.
   * `🟣 ESP32 #2 (Beta)`: Keystrokes sent only to PC 2.
   * `⚡ Both (Simultaneous)`: Broadcasts the exact same typing sequence to both computers simultaneously!
4. Paste your code, adjust speed/error rates, ensure IDE helpers are checked, and click **Start Auto-Typing**!

---

## ✨ Features Retained from Previous Versions
* **141,000+ Keystrokes Biometric Dynamics Model**: Sampled from real human typing sessions (variable hold times, digraph flight transitions, natural punctuation hesitation).
* **Keyboard Neighbor Typo & Correction Simulation**: Configurable error rate with realistic backspaces.
* **Instant Delete Bracket Neutralizer**: Neutralizes IDE auto-closing brackets without desyncing.
* **IDE Auto-Indentation Bypass**: Eliminates double-indent spacing bugs.
* **Simulate Human Thinking Pauses**: 1.8s randomized pause when completing code blocks (`}`), 1.5s randomized pause across random lines.
