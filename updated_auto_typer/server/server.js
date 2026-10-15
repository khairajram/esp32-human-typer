/**
 * Central Server for Multi-ESP32 BLE Auto-Typer
 * Manages WebSocket connections for ESP32 hardware units and Web Dashboards.
 */

const express = require('express');
const http = require('http');
const path = require('path');
const os = require('os');
const { WebSocketServer, WebSocket } = require('ws');

const app = express();
const server = http.createServer(app);
const wss = new WebSocketServer({ server });

const PORT = process.env.PORT || 3000;
const PUBLIC_DIR = path.join(__dirname, '..', 'public');

// Serve static frontend files
app.use(express.static(PUBLIC_DIR));
app.use(express.json({ limit: '10mb' }));

// Connected Devices & Clients Registry
// Key: deviceId (e.g., "ESP_ALPHA", "ESP_BETA")
const devices = new Map();
const webClients = new Set();

// Utility: Get local network IPv4 address (prioritizes real physical Wi-Fi/Ethernet)
function getLocalIpAddress() {
  const interfaces = os.networkInterfaces();

  // 1. First priority: Physical Wi-Fi or Wireless adapters
  for (const name of Object.keys(interfaces)) {
    const lower = name.toLowerCase();
    if (lower.includes('wi-fi') || lower.includes('wireless') || lower.includes('wlan')) {
      for (const iface of interfaces[name]) {
        if (iface.family === 'IPv4' && !iface.internal) {
          return iface.address;
        }
      }
    }
  }

  // 2. Second priority: Physical Ethernet adapters (exclude virtual WSL/VMware/Hyper-V)
  for (const name of Object.keys(interfaces)) {
    const lower = name.toLowerCase();
    if (!lower.includes('vethernet') && !lower.includes('wsl') && !lower.includes('vmware') && !lower.includes('virtual') && !lower.includes('loopback')) {
      for (const iface of interfaces[name]) {
        if (iface.family === 'IPv4' && !iface.internal) {
          return iface.address;
        }
      }
    }
  }

  // 3. Fallback: Any non-internal IPv4
  for (const name of Object.keys(interfaces)) {
    for (const iface of interfaces[name]) {
      if (iface.family === 'IPv4' && !iface.internal) {
        return iface.address;
      }
    }
  }
  return 'localhost';
}

// Broadcast message to all connected web dashboards
function broadcastToWebClients(data) {
  const message = typeof data === 'string' ? data : JSON.stringify(data);
  for (const client of webClients) {
    if (client.readyState === WebSocket.OPEN) {
      client.send(message);
    }
  }
}

// Generate device list payload for web clients
function getDeviceListPayload() {
  const list = [];
  for (const [id, dev] of devices.entries()) {
    list.push({
      id: dev.id,
      name: dev.name || id,
      bleName: dev.bleName || 'Unknown',
      online: dev.ws && dev.ws.readyState === WebSocket.OPEN,
      bleConnected: !!dev.bleConnected,
      state: dev.state || 'idle',
      ip: dev.ip || 'Unknown',
      lastSeen: dev.lastSeen || Date.now()
    });
  }
  return {
    type: 'DEVICE_LIST',
    devices: list,
    serverIp: getLocalIpAddress(),
    serverPort: PORT
  };
}

// REST API Endpoints
app.get('/api/info', (req, res) => {
  res.json({
    serverIp: getLocalIpAddress(),
    port: PORT,
    connectedDevices: devices.size,
    connectedDashboards: webClients.size
  });
});

app.get('/api/devices', (req, res) => {
  res.json(getDeviceListPayload());
});

// WebSocket Connection Router
wss.on('connection', (ws, req) => {
  const url = req.url || '';
  const clientIp = req.socket.remoteAddress;

  // 1. ESP32 Device Connection (/ws/device)
  if (url.startsWith('/ws/device')) {
    let currentDeviceId = null;

    ws.on('message', (raw) => {
      try {
        const msg = JSON.parse(raw.toString());

        // Device Registration Handshake
        if (msg.type === 'REGISTER') {
          currentDeviceId = msg.deviceId;
          devices.set(currentDeviceId, {
            id: currentDeviceId,
            name: msg.name || currentDeviceId,
            bleName: msg.bleName || 'ESP32 Typer',
            bleConnected: !!msg.bleConnected,
            state: 'idle',
            ip: msg.ip || clientIp,
            ws: ws,
            lastSeen: Date.now()
          });

          console.log(`[+] Device Registered: ${msg.name} (${currentDeviceId}) from ${msg.ip || clientIp}`);
          broadcastToWebClients(getDeviceListPayload());
        }

        // Live Status / Telemetry update from ESP32
        else if (msg.type === 'STATUS_UPDATE') {
          const dev = devices.get(currentDeviceId);
          if (dev) {
            dev.bleConnected = msg.bleConnected !== undefined ? msg.bleConnected : dev.bleConnected;
            dev.state = msg.state || dev.state;
            dev.lastSeen = Date.now();
            if (msg.progress !== undefined) dev.progress = msg.progress;

            broadcastToWebClients({
              type: 'DEVICE_STATUS',
              deviceId: currentDeviceId,
              bleConnected: dev.bleConnected,
              state: dev.state,
              progress: dev.progress
            });
          }
        }

        // Heartbeat response
        else if (msg.type === 'PONG') {
          const dev = devices.get(currentDeviceId);
          if (dev) dev.lastSeen = Date.now();
        }

      } catch (err) {
        console.error('[!] Failed to parse device message:', err.message);
      }
    });

    ws.on('close', () => {
      if (currentDeviceId && devices.has(currentDeviceId)) {
        const dev = devices.get(currentDeviceId);
        console.log(`[-] Device Disconnected: ${dev.name} (${currentDeviceId})`);
        dev.ws = null;
        dev.state = 'offline';
        broadcastToWebClients(getDeviceListPayload());
      }
    });

    ws.on('error', (err) => {
      console.error(`[!] Socket error on device ${currentDeviceId}:`, err.message);
    });
  }

  // 2. Web Dashboard Client (/ws/client or root)
  else {
    webClients.add(ws);
    console.log(`[+] Web Dashboard connected from ${clientIp} (Total dashboards: ${webClients.size})`);

    // Send immediate device snapshot on connect
    ws.send(JSON.stringify(getDeviceListPayload()));

    ws.on('message', (raw) => {
      try {
        const msg = JSON.parse(raw.toString());

        // Forward Typing Queue to Selected Target Device(s)
        if (msg.type === 'START_TYPING') {
          const { target, queue } = msg;
          console.log(`[->] Start typing command received for target: ${target}`);

          let dispatchedCount = 0;
          for (const [id, dev] of devices.entries()) {
            if ((target === 'ALL' || target === id) && dev.ws && dev.ws.readyState === WebSocket.OPEN) {
              dev.state = 'typing';
              dev.ws.send(JSON.stringify({
                type: 'EXECUTE_QUEUE',
                queue: queue
              }));
              dispatchedCount++;
            }
          }

          ws.send(JSON.stringify({
            type: 'COMMAND_ACK',
            action: 'START_TYPING',
            target: target,
            dispatched: dispatchedCount
          }));

          broadcastToWebClients(getDeviceListPayload());
        }

        // Forward Stop / Cancel to Target Device(s)
        else if (msg.type === 'STOP_TYPING') {
          const { target } = msg;
          console.log(`[!] Stop typing command received for target: ${target}`);

          for (const [id, dev] of devices.entries()) {
            if ((target === 'ALL' || target === id) && dev.ws && dev.ws.readyState === WebSocket.OPEN) {
              dev.state = 'idle';
              dev.ws.send(JSON.stringify({
                type: 'ABORT_QUEUE'
              }));
            }
          }

          broadcastToWebClients(getDeviceListPayload());
        }

        // Forward Pause to Target Device(s)
        else if (msg.type === 'PAUSE_TYPING') {
          const { target } = msg;
          console.log(`[||] Pause typing command received for target: ${target}`);

          for (const [id, dev] of devices.entries()) {
            if ((target === 'ALL' || target === id) && dev.ws && dev.ws.readyState === WebSocket.OPEN) {
              dev.state = 'paused';
              dev.ws.send(JSON.stringify({
                type: 'PAUSE_QUEUE'
              }));
            }
          }

          broadcastToWebClients(getDeviceListPayload());
        }

        // Forward Resume to Target Device(s)
        else if (msg.type === 'RESUME_TYPING') {
          const { target } = msg;
          console.log(`[>] Resume typing command received for target: ${target}`);

          for (const [id, dev] of devices.entries()) {
            if ((target === 'ALL' || target === id) && dev.ws && dev.ws.readyState === WebSocket.OPEN) {
              dev.state = 'typing';
              dev.ws.send(JSON.stringify({
                type: 'RESUME_QUEUE'
              }));
            }
          }

          broadcastToWebClients(getDeviceListPayload());
        }

      } catch (err) {
        console.error('[!] Failed to parse client message:', err.message);
      }
    });

    ws.on('close', () => {
      webClients.delete(ws);
      console.log(`[-] Web Dashboard disconnected (Remaining dashboards: ${webClients.size})`);
    });

    ws.on('error', (err) => {
      console.error('[!] Dashboard socket error:', err.message);
      webClients.delete(ws);
    });
  }
});

// Periodic Heartbeat check every 5 seconds
setInterval(() => {
  for (const [id, dev] of devices.entries()) {
    if (dev.ws && dev.ws.readyState === WebSocket.OPEN) {
      dev.ws.send(JSON.stringify({ type: 'PING' }));
      // If no activity in 12 seconds, mark as offline
      if (Date.now() - dev.lastSeen > 12000) {
        dev.ws.close();
        dev.ws = null;
        dev.state = 'offline';
        broadcastToWebClients(getDeviceListPayload());
      }
    }
  }
}, 5000);

// Start Server
server.listen(PORT, '0.0.0.0', () => {
  const localIp = getLocalIpAddress();
  console.log('\n========================================================');
  console.log('       ESP32 MULTI-DEVICE AUTO-TYPER CENTRAL SERVER     ');
  console.log('========================================================');
  console.log(` Dashboard URL:       http://localhost:${PORT}`);
  console.log(` Wi-Fi / LAN URL:     http://${localIp}:${PORT}`);
  console.log(` ESP32 WebSocket URL: ws://${localIp}:${PORT}/ws/device`);
  console.log('--------------------------------------------------------');
  console.log(' 👉 USE THIS IP IN YOUR esp32_firmware.ino:');
  console.log(`    const char* SERVER_HOST = "${localIp}";`);
  console.log('========================================================\n');
});
