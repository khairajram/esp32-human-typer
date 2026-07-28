#ifndef HTML_PAGE_H
#define HTML_PAGE_H

#include <Arduino.h>

const char html_page[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Human Typer - Control Center</title>
  <style>
/* Modern CSS Reset & Core Design System */
:root {
  --bg-color: #0b0f19;
  --panel-bg: rgba(17, 24, 39, 0.7);
  --border-color: rgba(255, 255, 255, 0.08);
  --text-primary: #f3f4f6;
  --text-secondary: #9ca3af;
  --text-muted: #6b7280;
  
  --primary: #8b5cf6;
  --primary-glow: rgba(139, 92, 246, 0.3);
  --secondary: #3b82f6;
  --secondary-glow: rgba(59, 130, 246, 0.2);
  
  --success: #10b981;
  --success-glow: rgba(16, 185, 129, 0.2);
  --warning: #f59e0b;
  --warning-glow: rgba(245, 158, 11, 0.2);
  --danger: #ef4444;
  --danger-glow: rgba(239, 68, 68, 0.2);
  
  --font-sans: 'Inter', -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
  --font-mono: 'JetBrains Mono', 'Fira Code', monospace;
  
  --shadow-lg: 0 10px 25px -5px rgba(0, 0, 0, 0.3), 0 8px 10px -6px rgba(0, 0, 0, 0.3);
}

* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
}

body {
  background-color: var(--bg-color);
  color: var(--text-primary);
  font-family: var(--font-sans);
  min-height: 100vh;
  overflow-x: hidden;
  position: relative;
  display: flex;
  flex-direction: column;
}

/* Atmospheric Glowing Background Orbs */
.glow-container {
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  overflow: hidden;
  z-index: -1;
  pointer-events: none;
}

.glow-orb {
  position: absolute;
  border-radius: 50%;
  filter: blur(120px);
  opacity: 0.15;
  mix-blend-mode: screen;
  animation: float 20s infinite alternate;
}

#orb-1 {
  background: var(--primary);
  width: 500px;
  height: 500px;
  top: -100px;
  left: -100px;
}

#orb-2 {
  background: var(--secondary);
  width: 600px;
  height: 600px;
  bottom: -150px;
  right: -100px;
  animation-delay: -5s;
}

@keyframes float {
  0% {
    transform: translate(0, 0) scale(1);
  }
  100% {
    transform: translate(80px, 50px) scale(1.1);
  }
}

/* App Layout */
.app-container {
  max-width: 1440px;
  width: 100%;
  margin: 0 auto;
  padding: 24px;
  display: flex;
  flex-direction: column;
  height: 100vh;
  max-height: 100vh;
}

/* Header Styles */
.app-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 24px;
  padding-bottom: 16px;
  border-bottom: 1px solid var(--border-color);
}

.brand {
  display: flex;
  align-items: center;
  gap: 12px;
}

.brand-logo {
  background: linear-gradient(135deg, var(--primary), var(--secondary));
  padding: 8px;
  border-radius: 8px;
  box-shadow: 0 0 15px var(--primary-glow);
  display: flex;
  align-items: center;
  justify-content: center;
  color: white;
}

.logo-icon {
  animation: pulse 3s infinite;
}

@keyframes pulse {
  0%, 100% { transform: scale(1); }
  50% { transform: scale(1.05); }
}

.brand h1 {
  font-size: 20px;
  font-weight: 700;
  letter-spacing: -0.5px;
  display: flex;
  align-items: center;
  gap: 8px;
}

.badge {
  font-size: 11px;
  background: rgba(139, 92, 246, 0.15);
  color: #c084fc;
  padding: 2px 6px;
  border-radius: 4px;
  border: 1px solid rgba(139, 92, 246, 0.3);
  font-weight: 600;
}

.header-status {
  display: flex;
  gap: 24px;
}

.status-item {
  display: flex;
  align-items: center;
  gap: 8px;
  font-size: 13px;
}

.status-label {
  color: var(--text-secondary);
}

.status-val {
  font-weight: 600;
  padding: 3px 8px;
  border-radius: 6px;
  font-family: var(--font-mono);
  font-size: 12px;
  transition: all 0.3s ease;
}

.status-val.disconnected {
  background: rgba(239, 68, 68, 0.1);
  color: #fca5a5;
  border: 1px solid rgba(239, 68, 68, 0.2);
}

.status-val.connected {
  background: rgba(16, 185, 129, 0.1);
  color: #a7f3d0;
  border: 1px solid rgba(16, 185, 129, 0.2);
  box-shadow: 0 0 10px rgba(16, 185, 129, 0.2);
}

.status-val.connecting {
  background: rgba(245, 158, 11, 0.1);
  color: #fde68a;
  border: 1px solid rgba(245, 158, 11, 0.2);
}

/* Main Dashboard Layout */
.app-main {
  display: grid;
  grid-template-columns: 450px 1fr;
  gap: 24px;
  flex-grow: 1;
  min-height: 0; /* Important for scrollable children */
}

.main-column {
  display: flex;
  flex-direction: column;
  gap: 24px;
  min-height: 0;
}

.main-column.left {
  flex-shrink: 0;
}

.main-column.right {
  flex-grow: 1;
}

/* Glassmorphic Panel/Card Styles */
.glass {
  background: var(--panel-bg);
  backdrop-filter: blur(16px);
  -webkit-backdrop-filter: blur(16px);
  border: 1px solid var(--border-color);
  border-radius: 12px;
  box-shadow: var(--shadow-lg);
  overflow: hidden;
}

.card {
  display: flex;
  flex-direction: column;
}

.card-header {
  padding: 16px 20px;
  border-bottom: 1px solid var(--border-color);
}

.card-header h2 {
  font-size: 14px;
  font-weight: 600;
  text-transform: uppercase;
  letter-spacing: 0.5px;
  color: var(--text-secondary);
}

.card-body {
  padding: 20px;
}

.flex-grow {
  flex-grow: 1;
}

.flex-column {
  display: flex;
  flex-direction: column;
}

/* Form Controls & Inputs */
.controls-row {
  display: flex;
  gap: 12px;
}

.btn {
  font-family: var(--font-sans);
  font-weight: 500;
  font-size: 13px;
  padding: 10px 16px;
  border-radius: 8px;
  border: 1px solid transparent;
  cursor: pointer;
  display: inline-flex;
  align-items: center;
  justify-content: center;
  gap: 8px;
  transition: all 0.2s cubic-bezier(0.4, 0, 0.2, 1);
}

.btn:disabled {
  opacity: 0.4;
  cursor: not-allowed;
}

.btn-primary {
  background: linear-gradient(135deg, var(--primary), var(--secondary));
  color: white;
  box-shadow: 0 4px 12px var(--primary-glow);
}

.btn-primary:hover:not(:disabled) {
  transform: translateY(-1px);
  box-shadow: 0 6px 16px rgba(139, 92, 246, 0.4);
}

.btn-secondary {
  background: rgba(255, 255, 255, 0.05);
  border: 1px solid var(--border-color);
  color: var(--text-primary);
}

.btn-secondary:hover:not(:disabled) {
  background: rgba(255, 255, 255, 0.1);
}

.btn-success {
  background: var(--success);
  color: white;
  box-shadow: 0 4px 12px var(--success-glow);
}

.btn-success:hover:not(:disabled) {
  background: #059669;
  transform: translateY(-1px);
  box-shadow: 0 6px 16px rgba(16, 185, 129, 0.4);
}

.btn-warning {
  background: var(--warning);
  color: white;
  box-shadow: 0 4px 12px var(--warning-glow);
}

.btn-warning:hover:not(:disabled) {
  background: #d97706;
  transform: translateY(-1px);
}

.btn-danger {
  background: var(--danger);
  color: white;
  box-shadow: 0 4px 12px var(--danger-glow);
}

.btn-danger:hover:not(:disabled) {
  background: #dc2626;
  transform: translateY(-1px);
  box-shadow: 0 6px 16px rgba(239, 68, 68, 0.4);
}

.btn-text {
  background: transparent;
  color: var(--text-secondary);
  font-size: 12px;
  padding: 6px 12px;
}

.btn-text:hover {
  color: var(--text-primary);
  background: rgba(255, 255, 255, 0.05);
}

.input-textarea {
  flex-grow: 1;
  background: rgba(0, 0, 0, 0.25);
  border: 1px solid var(--border-color);
  border-radius: 8px;
  padding: 16px;
  color: var(--text-primary);
  font-family: var(--font-mono);
  font-size: 13px;
  line-height: 1.6;
  resize: none;
  outline: none;
  min-height: 120px;
  transition: border-color 0.2s;
}

.input-textarea:focus {
  border-color: rgba(139, 92, 246, 0.5);
  box-shadow: 0 0 0 2px var(--primary-glow);
}

.textarea-actions {
  display: flex;
  justify-content: flex-end;
  gap: 8px;
  margin-top: 8px;
}

/* Settings Controls */
.settings-group {
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.setting-item {
  display: flex;
  flex-direction: column;
  gap: 8px;
}

.setting-label-row {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.setting-label-row label {
  font-size: 13px;
  font-weight: 550;
  color: var(--text-secondary);
}

.setting-bubble {
  background: rgba(139, 92, 246, 0.15);
  color: #c084fc;
  font-size: 12px;
  font-weight: 600;
  padding: 2px 6px;
  border-radius: 4px;
  font-family: var(--font-mono);
}

.input-select {
  background: rgba(0, 0, 0, 0.25);
  border: 1px solid var(--border-color);
  color: var(--text-primary);
  padding: 10px 14px;
  border-radius: 8px;
  outline: none;
  font-size: 13px;
  cursor: pointer;
  width: 100%;
}

.input-select:focus {
  border-color: rgba(139, 92, 246, 0.5);
}

.input-range {
  -webkit-appearance: none;
  appearance: none;
  width: 100%;
  height: 6px;
  background: rgba(255, 255, 255, 0.1);
  border-radius: 3px;
  outline: none;
  cursor: pointer;
}

.input-range::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 16px;
  height: 16px;
  border-radius: 50%;
  background: var(--primary);
  box-shadow: 0 0 10px var(--primary-glow);
  transition: transform 0.1s;
}

.input-range::-webkit-slider-thumb:hover {
  transform: scale(1.25);
}

.range-hint {
  font-size: 11px;
  color: var(--text-muted);
}

/* Stats Cards Grid */
.stats-grid {
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  gap: 16px;
  flex-shrink: 0;
}

.stat-card {
  padding: 16px;
  display: flex;
  flex-direction: column;
  gap: 6px;
}

.stat-label {
  font-size: 11px;
  font-weight: 600;
  text-transform: uppercase;
  color: var(--text-secondary);
  letter-spacing: 0.5px;
}

.stat-val-container {
  display: flex;
  align-items: baseline;
  gap: 4px;
}

.stat-value {
  font-size: 28px;
  font-weight: 700;
  font-family: var(--font-mono);
  background: linear-gradient(135deg, #fff, var(--text-secondary));
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
}

.stat-unit {
  font-size: 12px;
  color: var(--text-muted);
  font-weight: 500;
}

/* Terminal Visualizer Styles */
.terminal-card {
  min-height: 0;
}

.terminal-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  background: rgba(0, 0, 0, 0.15);
  padding: 10px 16px;
}

.terminal-dots {
  display: flex;
  gap: 6px;
}

.terminal-dots .dot {
  width: 10px;
  height: 10px;
  border-radius: 50%;
}

.terminal-dots .dot.red { background: #ef4444; }
.terminal-dots .dot.yellow { background: #f59e0b; }
.terminal-dots .dot.green { background: #10b981; }

.terminal-title {
  font-family: var(--font-mono);
  font-size: 12px;
  color: var(--text-secondary);
}

.terminal-status {
  font-size: 11px;
  font-family: var(--font-mono);
  background: rgba(255, 255, 255, 0.05);
  padding: 2px 8px;
  border-radius: 4px;
  color: var(--text-muted);
}

.terminal-body {
  flex-grow: 1;
  background: #05070c;
  color: var(--text-secondary);
  font-family: var(--font-mono);
  font-size: 14px;
  line-height: 1.6;
  padding: 20px;
  overflow-y: auto;
  position: relative;
  min-height: 150px;
}

.visualizer-content {
  white-space: pre-wrap;
  word-break: break-all;
  height: 100%;
}

.placeholder-text {
  color: var(--text-muted);
  font-style: italic;
}

/* Dynamic Text Coloring in Visualizer */
.typed-char {
  color: #ffffff;
}

.typed-char.correct {
  color: var(--success);
  text-shadow: 0 0 4px rgba(16, 185, 129, 0.2);
}

.typed-char.error {
  color: #ffffff;
  background-color: rgba(239, 68, 68, 0.4);
  border-bottom: 2px solid var(--danger);
}

.typed-char.pending {
  color: var(--text-muted);
  opacity: 0.6;
}

.blinking-cursor {
  background-color: var(--primary);
  color: white;
  animation: blink 1s step-end infinite;
  box-shadow: 0 0 8px var(--primary);
  display: inline-block;
  width: 8px;
  height: 18px;
  vertical-align: middle;
}

@keyframes blink {
  from, to { background-color: transparent; box-shadow: none; }
  50% { background-color: var(--primary); box-shadow: 0 0 8px var(--primary); }
}

/* Playback Control Area */
.playback-container {
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.progress-bar-container {
  width: 100%;
  height: 6px;
  background: rgba(255, 255, 255, 0.05);
  border-radius: 3px;
  overflow: hidden;
}

.progress-bar-fill {
  width: 0%;
  height: 100%;
  background: linear-gradient(90deg, var(--secondary), var(--primary));
  border-radius: 3px;
  transition: width 0.1s linear;
  box-shadow: 0 0 10px rgba(139, 92, 246, 0.5);
}

.playback-controls {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.playback-info {
  font-size: 13px;
  color: var(--text-secondary);
  font-weight: 500;
}

.playback-info span {
  font-family: var(--font-mono);
  font-weight: 600;
  color: var(--text-primary);
}

.playback-buttons {
  display: flex;
  gap: 12px;
}

</style>
  <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700&family=JetBrains+Mono:wght@400;500;700&display=swap" rel="stylesheet">
</head>
<body>
  <div class="glow-container">
    <div class="glow-orb" id="orb-1"></div>
    <div class="glow-orb" id="orb-2"></div>
  </div>

  <div class="app-container">
    <header class="app-header">
      <div class="brand">
        <div class="brand-logo">
          <svg viewBox="0 0 24 24" width="24" height="24" stroke="currentColor" stroke-width="2.5" fill="none" stroke-linecap="round" stroke-linejoin="round" class="logo-icon">
            <rect x="2" y="4" width="20" height="16" rx="2" ry="2"></rect>
            <line x1="6" y1="8" x2="6" y2="8"></line>
            <line x1="10" y1="8" x2="10" y2="8"></line>
            <line x1="14" y1="8" x2="14" y2="8"></line>
            <line x1="18" y1="8" x2="18" y2="8"></line>
            <line x1="6" y1="12" x2="6" y2="12"></line>
            <line x1="10" y1="12" x2="18" y2="12"></line>
            <line x1="6" y1="16" x2="18" y2="16"></line>
          </svg>
        </div>
        <h1>ESP32 Human Typer <span class="badge">v2.0</span></h1>
      </div>
      
      <div class="header-status">
        <div class="status-item">
          <span class="status-label">Serial Connection:</span>
          <span id="serial-status" class="status-val disconnected">Disconnected</span>
        </div>
        <div class="status-item">
          <span class="status-label">Bluetooth State:</span>
          <span id="bt-status" class="status-val disconnected">Unknown</span>
        </div>
      </div>
    </header>

    <main class="app-main">
      <!-- Left Column: Controls & Text Input -->
      <section class="main-column left">
        <div class="card glass">
          <div class="card-header">
            <h2>Connection & Device Control</h2>
          </div>
          <div class="card-body controls-row">
            <button id="btn-connect" class="btn btn-primary">
              <svg viewBox="0 0 24 24" width="18" height="18" stroke="currentColor" stroke-width="2" fill="none" stroke-linecap="round" stroke-linejoin="round" class="btn-icon"><path d="M10 13a5 5 0 0 0 7.54.54l3-3a5 5 0 0 0-7.07-7.07l-1.72 1.71"></path><path d="M14 11a5 5 0 0 0-7.54-.54l-3 3a5 5 0 0 0 7.07 7.07l1.71-1.71"></path></svg>
              <span>Connect ESP32 (Web Serial)</span>
            </button>
            <button id="btn-disconnect" class="btn btn-secondary" disabled>
              <span>Disconnect</span>
            </button>
          </div>
        </div>

        <div class="card glass flex-grow">
          <div class="card-header">
            <h2>Typing Data Input</h2>
          </div>
          <div class="card-body flex-column flex-grow">
            <textarea id="typing-input" placeholder="Paste the text you want to auto-type here..." class="input-textarea"></textarea>
            <div class="textarea-actions">
              <button id="btn-clear" class="btn btn-text">Clear Text</button>
              <button id="btn-load-sample" class="btn btn-text">Load Sample Code</button>
            </div>
          </div>
        </div>

        <div class="card glass">
          <div class="card-header">
            <h2>Human Simulation Settings</h2>
          </div>
          <div class="card-body">
            <div class="settings-group">
              <div class="setting-item">
                <div class="setting-label-row">
                  <label for="select-mode">Typing Profile</label>
                </div>
                <select id="select-mode" class="input-select">
                  <option value="human">Human Simulated (Keystroke Model)</option>
                  <option value="robot">Robot (Instant / Fast)</option>
                </select>
              </div>

              <div class="setting-item">
                <div class="setting-label-row">
                  <label for="speed-slider">Speed Multiplier</label>
                  <span id="speed-val" class="setting-bubble">1.0x</span>
                </div>
                <input type="range" id="speed-slider" min="0.2" max="3" step="0.1" value="1.0" class="input-range">
                <span class="range-hint">Higher values make typing faster (shrinks delay between keys)</span>
              </div>

              <div class="setting-item">
                <div class="setting-label-row">
                  <label for="error-slider">Error Rate (Typos)</label>
                  <span id="error-val" class="setting-bubble">5.8%</span>
                </div>
                <input type="range" id="error-slider" min="0" max="20" step="0.5" value="5.8" class="input-range">
                <span class="range-hint">Simulate typos and real-time corrections via backspace</span>
              </div>

              <div class="setting-item" style="margin-top: 8px; border-top: 1px solid var(--border-color); padding-top: 12px;">
                <label style="font-size: 12px; font-weight: 600; text-transform: uppercase; color: var(--text-muted); letter-spacing: 0.5px; display: block; margin-bottom: 8px;">IDE Compatibility Helper</label>
                <div style="display: flex; flex-direction: column; gap: 10px;">
                  <label style="display: flex; align-items: center; gap: 8px; font-size: 13px; color: var(--text-secondary); cursor: pointer;">
                    <input type="checkbox" id="check-ide-indent" style="accent-color: var(--primary); width: 16px; height: 16px; cursor: pointer;">
                    <span>Bypass IDE Auto-Indentation</span>
                  </label>
                  <label style="display: flex; align-items: center; gap: 8px; font-size: 13px; color: var(--text-secondary); cursor: pointer;">
                    <input type="checkbox" id="check-ide-brackets" style="accent-color: var(--primary); width: 16px; height: 16px; cursor: pointer;">
                    <span>Bypass IDE Auto-Brackets / Quotes</span>
                  </label>
                </div>
                <span class="range-hint" style="margin-top: 4px; display: block;">Prevents duplicate brackets, quotes, and excessive spacing in coding IDEs.</span>
              </div>
            </div>
          </div>
        </div>
      </section>

      <!-- Right Column: Visualizer & Live Stats -->
      <section class="main-column right">
        <!-- Stats Dashboard -->
        <div class="stats-grid">
          <div class="stat-card glass">
            <span class="stat-label">Typing Speed</span>
            <div class="stat-val-container">
              <span id="stat-wpm" class="stat-value">0</span>
              <span class="stat-unit">WPM</span>
            </div>
          </div>
          
          <div class="stat-card glass">
            <span class="stat-label">Accuracy</span>
            <div class="stat-val-container">
              <span id="stat-accuracy" class="stat-value">100</span>
              <span class="stat-unit">%</span>
            </div>
          </div>

          <div class="stat-card glass">
            <span class="stat-label">Progress</span>
            <div class="stat-val-container">
              <span id="stat-progress" class="stat-value">0</span>
              <span class="stat-unit">%</span>
            </div>
          </div>

          <div class="stat-card glass">
            <span class="stat-label">Time Remaining</span>
            <div class="stat-val-container">
              <span id="stat-eta" class="stat-value">--:--</span>
            </div>
          </div>
        </div>

        <!-- Terminal Output / Visualizer -->
        <div class="card glass flex-grow terminal-card">
          <div class="card-header terminal-header">
            <div class="terminal-dots">
              <span class="dot red"></span>
              <span class="dot yellow"></span>
              <span class="dot green"></span>
            </div>
            <div class="terminal-title">Human Typing Visualizer</div>
            <div class="terminal-status" id="terminal-state">Idle</div>
          </div>
          <div class="card-body terminal-body">
            <div id="typing-visualizer" class="visualizer-content">
              <span class="placeholder-text">Live typing simulation will display here as characters are sent to the ESP32...</span>
            </div>
          </div>
        </div>

        <!-- Playback Actions -->
        <div class="card glass">
          <div class="card-body playback-container">
            <div class="progress-bar-container">
              <div id="progress-bar-fill" class="progress-bar-fill"></div>
            </div>
            
            <div class="playback-controls">
              <div class="playback-info">
                Typed <span id="chars-typed">0</span> of <span id="chars-total">0</span> chars
              </div>
              <div class="playback-buttons">
                <button id="btn-start" class="btn btn-success" disabled>
                  <svg viewBox="0 0 24 24" width="18" height="18" stroke="currentColor" stroke-width="2" fill="none" stroke-linecap="round" stroke-linejoin="round" class="btn-icon"><polygon points="5 3 19 12 5 21 5 3"></polygon></svg>
                  <span>Start Typing</span>
                </button>
                <button id="btn-pause" class="btn btn-warning" disabled>
                  <svg viewBox="0 0 24 24" width="18" height="18" stroke="currentColor" stroke-width="2" fill="none" stroke-linecap="round" stroke-linejoin="round" class="btn-icon"><rect x="6" y="4" width="4" height="16"></rect><rect x="14" y="4" width="4" height="16"></rect></svg>
                  <span>Pause</span>
                </button>
                <button id="btn-stop" class="btn btn-danger" disabled>
                  <svg viewBox="0 0 24 24" width="18" height="18" stroke="currentColor" stroke-width="2" fill="none" stroke-linecap="round" stroke-linejoin="round" class="btn-icon"><rect x="4" y="4" width="16" height="16"></rect></svg>
                  <span>Stop</span>
                </button>
              </div>
            </div>
          </div>
        </div>
      </section>
    </main>
  </div>

  <script>
// ESP32 Human Typer Control Logic (Wireless Wi-Fi Mode)
const TYPING_PARAMS = {"typo_rate": 0.06760541520575102, "global_hold": {"mean": 177.30470095229066, "std": 74.67728000560433}, "global_flight": {"mean": 106.1325700573886, "std": 205.14941140874942}, "hold_stats": {"enter": {"mean": 96.36621720679013, "std": 28.124030619157967}, "t": {"mean": 171.26787231500342, "std": 41.618743001392374}, "h": {"mean": 154.43391113810503, "std": 42.32831861937749}, "i": {"mean": 173.38883961136025, "std": 57.3135988899338}, "s": {"mean": 207.49316386305165, "std": 62.31028860449479}, "space": {"mean": 196.6332439792176, "std": 60.58059016484647}, "e": {"mean": 194.50596417083494, "std": 52.798753157060304}, "f": {"mean": 148.53279721531575, "std": 39.89672240822195}, "l": {"mean": 183.86505826812729, "std": 80.88925322811845}, "a": {"mean": 288.4624438361947, "std": 103.67006409876704}, "w": {"mean": 207.2280141772152, "std": 51.975936849160306}, "g": {"mean": 154.93742283433136, "std": 38.901411843149255}, "v": {"mean": 138.89343197115386, "std": 31.27682665726677}, "d": {"mean": 177.36142456023896, "std": 52.61024346485298}, "c": {"mean": 179.32224351743048, "std": 49.363558493256406}, "y": {"mean": 140.1167120197585, "std": 29.415311553487047}, "o": {"mean": 202.4739985694667, "std": 76.2325773294352}, "u": {"mean": 158.81074304229196, "std": 57.891787582270474}, "n": {"mean": 157.06233153206654, "std": 43.676108855772945}, "r": {"mean": 186.80060049504954, "std": 51.28936096642768}, "ctrl_l": {"mean": 393.8613466135458, "std": 187.83267766440093}, "backspace": {"mean": 79.87665221789064, "std": 25.38462737272709}, "b": {"mean": 140.98090825471698, "std": 33.970487348325435}, "down": {"mean": 106.64478137931036, "std": 26.513275959569274}, "left": {"mean": 85.59484477911647, "std": 33.10932799187135}, "alt_l": {"mean": 392.02164444444446, "std": 150.44543271075702}, "up": {"mean": 95.99106444444446, "std": 51.9325811582207}, "m": {"mean": 159.2795290563476, "std": 51.35383860510451}, "p": {"mean": 172.96346961147088, "std": 46.708133159892746}, "k": {"mean": 158.18276918604653, "std": 37.14122239992294}, "j": {"mean": 109.60968612716765, "std": 27.327140570714665}, "right": {"mean": 106.40137996515679, "std": 46.40585667854341}, "ctrl_r": {"mean": 237.92333484162896, "std": 140.37858052860832}, ";": {"mean": 111.22763717105262, "std": 54.57593531308217}, "_": {"mean": 99.08578800000001, "std": 23.354590156409422}, "shift": {"mean": 348.96385629453687, "std": 151.81163980304115}, ",": {"mean": 97.91561160714286, "std": 21.209878210007417}, "(": {"mean": 96.24134137931036, "std": 27.381401915623467}, "q": {"mean": 143.88618571428574, "std": 45.258686856203546}, "<222>": {"mean": 102.69125507246379, "std": 22.733308569276765}, "tab": {"mean": 141.34654803149607, "std": 38.42032548838391}, "\u0016": {"mean": 131.42200375, "std": 23.250145985070954}, ".": {"mean": 111.88041940298507, "std": 20.411420813754997}, "+": {"mean": 87.5430164556962, "std": 19.261682123912248}, "=": {"mean": 88.23782795031055, "std": 24.24451015469829}, "\u0001": {"mean": 132.37828333333334, "std": 17.33978298737232}, "\u0003": {"mean": 156.03898292682925, "std": 25.149789137872798}, "*": {"mean": 76.0738909090909, "std": 14.226268027679358}, "3": {"mean": 137.4041175, "std": 51.765379576266454}, "0": {"mean": 95.55557401574805, "std": 25.668918135625024}, "5": {"mean": 117.26026500000003, "std": 30.391634571428945}, "delete": {"mean": 89.29251500000001, "std": 25.51801884528411}, "\u001a": {"mean": 122.14414375000001, "std": 25.047506492312984}, "2": {"mean": 138.32809591836735, "std": 40.35766698715506}, "shift_r": {"mean": 324.49715888888886, "std": 136.33984860260114}, "6": {"mean": 99.34548571428572, "std": 8.471935506369347}, "1": {"mean": 135.52689351851853, "std": 31.026586593801184}, "I": {"mean": 89.27144999999997, "std": 15.026137542545655}, "{": {"mean": 104.22027619047618, "std": 19.915036338259466}, "x": {"mean": 150.64326410256416, "std": 52.1766616304228}, ")": {"mean": 103.69582068965518, "std": 15.431007020335418}, "cmd": {"mean": 341.8871719298245, "std": 169.11757921743123}, "\\": {"mean": 75.9153875, "std": 22.554211792300062}, "<255>": {"mean": 75.75486, "std": 13.041912862245324}, "'": {"mean": 90.26675500000002, "std": 26.80929264464982}, ">": {"mean": 112.61385517241379, "std": 19.405999369242107}, "\u0012": {"mean": 124.93637500000001, "std": 26.249300326996817}, "/": {"mean": 71.49264285714285, "std": 14.361778587810985}, "-": {"mean": 90.18356349206348, "std": 22.05066353633818}, "<": {"mean": 92.85904146341463, "std": 16.077970128324818}, ":": {"mean": 106.8069125, "std": 24.361140709203124}, "z": {"mean": 140.64195925925927, "std": 20.35242539011486}, "N": {"mean": 116.9656, "std": 34.81296800644521}, "M": {"mean": 85.8176, "std": 1.5764069538034873}, "[": {"mean": 124.27861538461535, "std": 26.87925212041511}, "]": {"mean": 109.3788818181818, "std": 20.127979304974648}, "9": {"mean": 112.07687777777778, "std": 36.735959523228516}, "&": {"mean": 89.62360000000001, "std": 32.19866176341468}, "7": {"mean": 112.81423157894737, "std": 31.172432814842107}, "L": {"mean": 115.92260000000002, "std": 27.544369958595894}, "4": {"mean": 141.06344827586207, "std": 59.03111402126977}, "8": {"mean": 113.85823529411766, "std": 40.27602925041061}, "media_volume_up": {"mean": 110.1621052631579, "std": 141.5158071728328}, "media_volume_down": {"mean": 82.56151515151514, "std": 33.03987179162998}, "alt_r": {"mean": 44.88166666666667, "std": 14.252636500739865}}, "flight_stats": {"t,i": {"mean": 28.412991242937853, "std": 68.47150059713537}, "h,s": {"mean": 39.01219444444445, "std": 45.02849603664747}, "s,space": {"mean": 45.326645011600924, "std": 107.25054266246788}, "space,i": {"mean": 99.14861854066986, "std": 168.83722054030815}, "space,s": {"mean": 86.47320597014925, "std": 147.42201428632774}, "space,t": {"mean": 89.46801561916615, "std": 151.4741027382107}, "space,h": {"mean": 104.85954114896461, "std": 158.32601484826364}, "t,e": {"mean": 26.88212404844291, "std": 50.54523762519641}, "e,space": {"mean": 19.49426492675882, "std": 52.4813662056435}, "space,f": {"mean": 73.95075741525423, "std": 143.7825762663512}, "i,l": {"mean": 61.18238339694656, "std": 91.36470729020823}, "l,e": {"mean": 27.29267679882525, "std": 73.52117753816398}, "t,h": {"mean": 21.22398512110727, "std": 65.22281071664213}, "h,a": {"mean": 37.9403, "std": 75.01649471923055}, "a,t": {"mean": 27.194166393442625, "std": 75.98890064448909}, "t,space": {"mean": 24.46960105633803, "std": 71.35256779957115}, "space,w": {"mean": 93.60073855421686, "std": 141.60216583468463}, "l,l": {"mean": 97.75734248704664, "std": 63.64019436712744}, "l,space": {"mean": 34.046637539682536, "std": 85.6380886149257}, "space,g": {"mean": 77.6576835978836, "std": 108.66350453299667}, "g,i": {"mean": 15.579873924731183, "std": 39.300401020723996}, "i,v": {"mean": 56.58520777777777, "std": 92.18458826702991}, "i,e": {"mean": 105.11693305785124, "std": 120.59529521088592}, "v,space": {"mean": 133.57140555555554, "std": 71.69194323815437}, "h,e": {"mean": 37.55877706032285, "std": 71.78928467419738}, "space,d": {"mean": 86.249725, "std": 172.4928408066913}, "space,a": {"mean": 97.68479577039275, "std": 154.60010376179522}, "a,a": {"mean": 121.53042765957449, "std": 165.70998628847533}, "a,space": {"mean": 21.165941191067, "std": 59.20310948562476}, "w,h": {"mean": 23.378931013916503, "std": 60.938635039695995}, "h,i": {"mean": 19.358470694444446, "std": 53.7653864109655}, "h,c": {"mean": 24.90505891472868, "std": 76.02443836564072}, "c,h": {"mean": 21.06213864042934, "std": 69.53550387061101}, "h,space": {"mean": 26.680327461139903, "std": 68.51283311469288}, "space,y": {"mean": 134.9598593457944, "std": 143.39175335089794}, "y,o": {"mean": 23.11160105263158, "std": 67.47661863118215}, "y,u": {"mean": 96.20958499999999, "std": 61.649474769703225}, "u,space": {"mean": 35.58625820895523, "std": 68.61159130956729}, "space,n": {"mean": 103.61099642032332, "std": 165.0599281270129}, "n,e": {"mean": 27.839046257485027, "std": 64.40740335562118}, "e,e": {"mean": 101.83601732168849, "std": 74.20226552499938}, "e,d": {"mean": 128.2054015625, "std": 50.929591411452506}, "h,r": {"mean": 95.97779554140126, "std": 120.30651999855237}, "s,o": {"mean": 20.173610071942445, "std": 57.199842740315134}, "o,space": {"mean": 34.2331222953904, "std": 76.55586879312949}, "d,o": {"mean": 18.618551648351648, "std": 62.219500711445036}, "i,space": {"mean": 125.53085052631579, "std": 136.97349224280077}, "i,n": {"mean": 32.53886434659091, "std": 68.87170115306294}, "n,s": {"mean": 55.79729147982062, "std": 141.93270497739402}, "s,t": {"mean": 27.429884636363635, "std": 68.67839530848873}, "e,a": {"mean": 23.352157987220448, "std": 75.33182752088584}, "a,d": {"mean": 21.88577106227106, "std": 57.204287005369444}, "d,space": {"mean": 18.412314837153197, "std": 54.201815941798905}, "t,o": {"mean": 28.387807197943445, "std": 67.92171109288809}, "o,o": {"mean": 105.66766719999998, "std": 107.30510616615939}, "a,backspace": {"mean": 334.81196356589146, "std": 177.24783668299816}, "backspace,a": {"mean": 309.9305742222222, "std": 247.25938401876834}, "backspace,r": {"mean": 415.1392956896551, "std": 339.932033049077}, "a,e": {"mean": 37.04673564102564, "std": 77.4463644500597}, "a,b": {"mean": 38.1066, "std": 87.07361574964898}, "b,l": {"mean": 25.833399523809522, "std": 77.27323832320052}, "b,e": {"mean": 127.73814901960785, "std": 101.46073305698937}, "space,o": {"mean": 99.17185443298969, "std": 156.4542593813517}, "space,c": {"mean": 141.16382813793106, "std": 167.1959790004918}, "h,n": {"mean": 166.48161065573774, "std": 100.41998613530518}, "n,g": {"mean": 25.45269747706422, "std": 82.26761554779353}, "e,s": {"mean": 111.01649235127478, "std": 126.109488514024}, "space,e": {"mean": 118.71936785714286, "std": 164.64508688366726}, "r,space": {"mean": 24.030178459237096, "std": 74.38620761725028}, "o,backspace": {"mean": 405.9734629464286, "std": 193.2708065368668}, "backspace,i": {"mean": 525.8199315270936, "std": 215.99750109088276}, "i,f": {"mean": 46.88680765306122, "std": 116.12422583729027}, "backspace,s": {"mean": 291.39346397515527, "std": 237.59540003817062}, "e,n": {"mean": 30.18843216019417, "std": 65.20817817465384}, "n,space": {"mean": 26.119390739808754, "std": 65.26859117104752}, "w,e": {"mean": 39.376035412026724, "std": 117.5031987320977}, "e,l": {"mean": 27.90880321167883, "std": 82.09114645715282}, "g,o": {"mean": 20.885498427672957, "std": 41.10889551222839}, "o,d": {"mean": 52.704975000000005, "std": 82.80630726477811}, "f,i": {"mean": 20.765479702970293, "std": 47.734668789956686}, "s,e": {"mean": 19.93191626746507, "std": 49.22002976913683}, "g,e": {"mean": 53.27498760330578, "std": 55.62914172488127}, "n,backspace": {"mean": 399.5861609589041, "std": 176.76344949278516}, "backspace,backspace": {"mean": 116.91180987956012, "std": 118.16049806118936}, "y,s": {"mean": 47.812413043478266, "std": 91.02198502243148}, "down,enter": {"mean": 386.97395714285716, "std": 302.6846336919219}, "media_volume_down,media_volume_down": {"mean": 108.30652, "std": 198.213315481869}, "media_volume_up,media_volume_up": {"mean": 69.33582676923078, "std": 162.2640457521522}, "f,d": {"mean": 374.5181142857144, "std": 425.11397855246315}, "d,d": {"mean": 155.78151376146786, "std": 155.12374896068937}, "s,s": {"mean": 160.3225960591133, "std": 166.7409655704107}, "s,d": {"mean": 67.0708476923077, "std": 170.4973682026614}, "left,left": {"mean": 194.2303088082902, "std": 233.96244862169053}, "f,g": {"mean": 61.13345555555556, "std": 170.60701995683016}, "g,f": {"mean": 37.1858125, "std": 53.28346602367468}, "f,f": {"mean": 186.61205445544556, "std": 307.74457343154444}, "left,s": {"mean": 752.9835999999999, "std": 350.6873198517686}, "space,space": {"mean": 485.6626319444444, "std": 425.0987365118285}, "space,left": {"mean": 1026.3632545454545, "std": 440.4862198473768}, "left,space": {"mean": 352.61451818181814, "std": 199.50304227780958}, "f,space": {"mean": 33.52383303834809, "std": 117.26679186340266}, "m,o": {"mean": 14.041907326007326, "std": 25.28697210170777}, "o,enter": {"mean": 425.20038000000005, "std": 336.5699096768525}, "u,s": {"mean": 33.08260303030303, "std": 72.27505529234051}, "u,e": {"mean": 123.84939056603775, "std": 99.22330498125108}, "a,c": {"mean": 34.938577947598255, "std": 93.06558709584915}, "c,backspace": {"mean": 385.74169056603773, "std": 204.3794413695053}, "backspace,c": {"mean": 302.34167058823533, "std": 247.58412271859427}, "a,l": {"mean": 31.56969977426637, "std": 63.89601529954549}, "space,p": {"mean": 188.23468132635253, "std": 157.8340336517459}, "p,r": {"mean": 206.0769832917706, "std": 226.1493203642772}, "p,o": {"mean": 61.84581312741314, "std": 152.85345911295477}, "o,b": {"mean": 139.19772156862746, "std": 112.91043205936325}, "e,m": {"mean": 35.65131428571429, "std": 108.2617559626148}, "m,space": {"mean": 30.43478491620112, "std": 89.51418054564235}, "space,l": {"mean": 59.80275529640427, "std": 117.18789308769647}, "l,o": {"mean": 151.91858413597734, "std": 92.8028416730472}, "o,n": {"mean": 27.605870150987226, "std": 72.29907963923804}, "f,r": {"mean": 122.79582359550562, "std": 81.19856577231116}, "f,o": {"mean": 53.87307029177719, "std": 98.93803999279194}, "r,m": {"mean": 20.945565536723162, "std": 38.26395966809621}, "o,r": {"mean": 32.648413221153845, "std": 86.83075509783451}, "r,t": {"mean": 85.8061, "std": 91.70718192868267}, "r,h": {"mean": 179.52002857142855, "std": 145.90393388730342}, "h,backspace": {"mean": 385.75545937499993, "std": 152.93508493528225}, "backspace,space": {"mean": 247.5764142857143, "std": 186.88073511046366}, "r,backspace": {"mean": 365.31816402877695, "std": 171.99845480336288}, "backspace,o": {"mean": 531.9497538732395, "std": 201.26113342521606}, "o,t": {"mean": 24.368763397129186, "std": 60.26273359894957}, "o,h": {"mean": 57.91713387096774, "std": 64.07280668844986}, "f,c": {"mean": 186.19171739130437, "std": 177.94752378256047}, "c,e": {"mean": 152.86435227272725, "std": 103.7824504895893}, "c,u": {"mean": 36.936684033613446, "std": 58.9134999747593}, "r,e": {"mean": 21.625036773547095, "std": 59.17243319336841}, "space,u": {"mean": 118.39467688022283, "std": 151.78582359938224}, "e,r": {"mean": 14.46919371492705, "std": 39.239547666098076}, "backspace,v": {"mean": 318.2208, "std": 287.3340231905391}, "v,e": {"mean": 62.47584676564157, "std": 44.63104291421529}, "v,r": {"mean": 163.6643971830986, "std": 47.10269174481469}, "a,n": {"mean": 36.77618758434548, "std": 98.32988440454788}, "n,k": {"mean": 49.11456614173229, "std": 87.43986742866436}, "k,space": {"mean": 26.65809349442379, "std": 52.84246364071875}, "o,e": {"mean": 62.31665641025641, "std": 105.3319580771854}, "space,m": {"mean": 87.84566720812182, "std": 149.39773055293279}, "m,v": {"mean": 48.64896153846155, "std": 33.31048460340836}, "e,backspace": {"mean": 354.8812101503759, "std": 162.4691750057896}, "backspace,n": {"mean": 498.4384520930233, "std": 262.87577270360975}, "n,a": {"mean": 15.479477575757576, "std": 41.91494359038184}, "w,a": {"mean": 40.67929107142857, "std": 45.1466767772133}, "i,backspace": {"mean": 447.34560350877194, "std": 191.06656856268933}, "backspace,t": {"mean": 249.93218489795922, "std": 217.5027496861681}, "backspace,h": {"mean": 581.0496193548387, "std": 292.02852324767554}, "e,y": {"mean": 21.909934033613446, "std": 37.9603932903477}, "y,space": {"mean": 21.22863173828125, "std": 51.22457163533181}, "n,d": {"mean": 34.0921666136725, "std": 101.54122673167382}, "d,w": {"mean": 103.13653636363637, "std": 69.92334447106116}, "w,i": {"mean": 17.618164772727273, "std": 35.371652679302905}, "t,a": {"mean": 17.37056807511737, "std": 46.471106078085796}, "o,u": {"mean": 14.552908024072217, "std": 41.13338651367117}, "k,e": {"mean": 29.25141666666666, "std": 61.748541827133984}, "e,p": {"mean": 63.164356250000004, "std": 98.72263071076945}, "p,space": {"mean": 21.579948571428574, "std": 53.95284155465136}, "enter,f": {"mean": 790.1127834862386, "std": 204.597730459788}, "f,backspace": {"mean": 392.34935294117645, "std": 203.60403907631294}, "backspace,l": {"mean": 375.6270158469946, "std": 199.98091254074333}, "l,i": {"mean": 19.369475226586104, "std": 56.47843895791624}, "p,l": {"mean": 133.51766948717952, "std": 97.05253376326007}, "p,a": {"mean": 120.10845800000001, "std": 107.46799756081452}, "l,n": {"mean": 63.05281397849462, "std": 96.89767683523543}, "o,w": {"mean": 22.470645649263723, "std": 63.32018626739846}, "w,space": {"mean": 16.25969832167832, "std": 38.60398465327282}, "space,r": {"mean": 130.62527184750732, "std": 196.65865486339797}, "r,a": {"mean": 17.927227139364305, "std": 40.208021812175524}, "l,a": {"mean": 19.137788265971317, "std": 47.44723535558372}, "a,r": {"mean": 47.75038353658537, "std": 95.57697342716042}, "r,g": {"mean": 143.43360227272728, "std": 75.3119855825977}, "a,v": {"mean": 31.01283423913043, "std": 99.541722102343}, "g,space": {"mean": 23.012545478036177, "std": 73.16344303437572}, "p,e": {"mean": 94.01413674121406, "std": 126.98944661365672}, "h,w": {"mean": 15.800696721311475, "std": 21.271377083081507}, "space,v": {"mean": 136.1923128440367, "std": 154.59120085680792}, "backspace,b": {"mean": 390.2771, "std": 287.7158623497163}, "a,k": {"mean": 35.96671183206107, "std": 87.56355794072616}, "u,b": {"mean": 172.34263085106383, "std": 121.85934507575057}, "u,l": {"mean": 24.70481801470588, "std": 67.33540769877429}, "b,i": {"mean": 40.68310519480519, "std": 116.04190206820434}, "i,c": {"mean": 88.43650405405407, "std": 107.66990505056503}, "l,s": {"mean": 54.690276859504124, "std": 82.52728074780023}, "o,k": {"mean": 143.164661, "std": 87.75766017327022}, "e,t": {"mean": 36.08517455357143, "std": 67.89037571176738}, "o,m": {"mean": 20.514949520153554, "std": 64.03793243308095}, "i,t": {"mean": 43.574765977742445, "std": 77.58284829871003}, "e,c": {"mean": 191.95969521912352, "std": 109.10375242294513}, "a,h": {"mean": 54.41736279069768, "std": 105.15806635038119}, "r,o": {"mean": 24.963369133574005, "std": 73.49352792545899}, "o,g": {"mean": 61.835, "std": 125.69184796412418}, "g,r": {"mean": 124.10448782051283, "std": 42.532506752604704}, "g,a": {"mean": 44.9055, "std": 111.8346847058542}, "a,m": {"mean": 43.29820970873786, "std": 102.02267743779616}, "n,o": {"mean": 25.231427348643006, "std": 82.41197601377797}, "w,backspace": {"mean": 365.5073925373134, "std": 202.92807293777713}, "backspace,e": {"mean": 251.98432607594935, "std": 230.93477333145168}, "a,enter": {"mean": 329.2900404761905, "std": 259.2464776520912}, "enter,b": {"mean": 854.2639130952382, "std": 173.72813713346972}, "enter,a": {"mean": 840.0991806451614, "std": 216.22403400387395}, "y,e": {"mean": 26.881583644859816, "std": 64.13921690187641}, "d,e": {"mean": 116.130871657754, "std": 47.16782161785676}, "e,v": {"mean": 115.33008521303258, "std": 85.30370556331617}, "e,o": {"mean": 38.881406024096385, "std": 72.0373591696678}, "y,r": {"mean": 105.84521, "std": 122.88082620574252}, "y,a": {"mean": 16.922900790513832, "std": 30.54408997148751}, "space,backspace": {"mean": 341.2490612762238, "std": 162.98368149777681}, "d,u": {"mean": 162.060175, "std": 266.349524465345}, "u,r": {"mean": 53.71469233038349, "std": 100.69417795767176}, "u,i": {"mean": 111.51549310344826, "std": 156.91797533221182}, "r,y": {"mean": 26.596783157894734, "std": 52.973354499310425}, "y,backspace": {"mean": 439.30875652173904, "std": 192.51581730742748}, "l,y": {"mean": 15.564989444444445, "std": 40.35668882300747}, "n,u": {"mean": 149.97551379310346, "std": 118.08383196254736}, "u,m": {"mean": 157.30809083333332, "std": 81.86558760524268}, "b,r": {"mean": 268.5365911764706, "std": 229.21704274566338}, "h,o": {"mean": 14.67996626054358, "std": 32.543744246087556}, "h,u": {"mean": 146.4147846153846, "std": 78.97902785139908}, "o,s": {"mean": 32.207054670329676, "std": 51.64595412061516}, "r,b": {"mean": 404.3433, "std": 177.468434055252}, "l,m": {"mean": 21.940326744186045, "std": 43.432141403636166}, "n,r": {"mean": 55.74658888888889, "std": 70.62312059684325}, "r,l": {"mean": 37.61841782178218, "std": 82.37914049613507}, "c,space": {"mean": 37.52365177664975, "std": 104.87191813764393}, "space,b": {"mean": 124.791059375, "std": 168.16889882373297}, "b,t": {"mean": 177.40920833333337, "std": 56.55762340104203}, "t,w": {"mean": 40.56036630434782, "std": 68.85105729914436}, "a,i": {"mean": 39.81806028368794, "std": 81.93730975297962}, "n,enter": {"mean": 324.29090434782603, "std": 113.91258048820664}, "enter,c": {"mean": 872.697806557377, "std": 154.93915240566386}, "enter,o": {"mean": 834.7222720338983, "std": 156.9402172083537}, "l,d": {"mean": 28.520653090332807, "std": 83.31958359998859}, "m,e": {"mean": 22.87476791720569, "std": 51.85675202128784}, "m,a": {"mean": 23.990922504892367, "std": 48.20644988869696}, "m,i": {"mean": 32.35729358974359, "std": 43.02911333207374}, "m,g": {"mean": 83.70116666666668, "std": 40.52208152173315}, "g,h": {"mean": 22.072135342465756, "std": 75.0375553130059}, "h,t": {"mean": 36.185248123620305, "std": 59.0050361835123}, "t,l": {"mean": 59.2709711409396, "std": 94.6887457388562}, "backspace,p": {"mean": 656.5625166666666, "std": 258.9189175368833}, "e,u": {"mean": 67.67732592592593, "std": 95.0278663024912}, "backspace,w": {"mean": 217.03835504587153, "std": 228.53396265073837}, "w,y": {"mean": 17.846854444444446, "std": 22.358228822820273}, "f,w": {"mean": 111.74600714285714, "std": 105.80568330593093}, "s,i": {"mean": 43.05928704453441, "std": 95.87931133803762}, "s,b": {"mean": 50.905393333333336, "std": 40.4152984685167}, "m,b": {"mean": 32.25854193548387, "std": 46.22684260987477}, "b,backspace": {"mean": 420.302944117647, "std": 127.87236667264837}, "b,o": {"mean": 28.636636, "std": 71.85785933950513}, "i,k": {"mean": 129.595302247191, "std": 69.36183078949838}, "w,o": {"mean": 15.298794162436547, "std": 36.481936398924454}, "o,l": {"mean": 132.60523488372093, "std": 83.23966049161172}, "l,r": {"mean": 41.562325471698124, "std": 75.15614013924869}, "d,backspace": {"mean": 408.18886785714284, "std": 195.8462555771346}, "h,l": {"mean": 105.55165192307693, "std": 69.72943921075199}, "t,backspace": {"mean": 374.37992180451124, "std": 155.27207228214974}, "m,s": {"mean": 81.6675027027027, "std": 75.94093168782115}, "a,s": {"mean": 26.246673694779115, "std": 62.72037980155809}, "b,y": {"mean": 29.02174358974359, "std": 91.87340569599144}, "i,g": {"mean": 32.83755480427046, "std": 71.40075768501644}, "s,y": {"mean": 26.767028787878786, "std": 46.91417110385246}, "t,m": {"mean": 52.103122388059695, "std": 173.02133714151518}, "n,t": {"mean": 35.66036141078838, "std": 78.43981929338018}, "c,l": {"mean": 31.52768301886793, "std": 58.794551647849126}, "e,w": {"mean": 26.51528275862069, "std": 72.64181322875909}, "m,backspace": {"mean": 378.4211782608696, "std": 178.8457969194666}, "backspace,j": {"mean": 402.7076375, "std": 208.97585224694288}, "j,s": {"mean": 116.41210000000001, "std": 44.835249120009735}, "s,u": {"mean": 48.50928035714286, "std": 124.22195926082982}, "u,backspace": {"mean": 398.43006341463416, "std": 170.07746663905297}, "backspace,u": {"mean": 614.6799373493976, "std": 270.1309477407639}, "n,c": {"mean": 36.155322500000004, "std": 78.97957532539249}, "d,y": {"mean": 36.03789885057471, "std": 81.63459927567688}, "w,n": {"mean": 32.763111926605504, "std": 58.18780240246015}, "space,k": {"mean": 88.77160052356021, "std": 147.51341589972054}, "i,h": {"mean": 59.357766477272726, "std": 111.86268560160072}, "m,u": {"mean": 142.85267923497267, "std": 44.84682816866343}, "u,c": {"mean": 47.99580677966102, "std": 65.99945977463686}, "u,h": {"mean": 160.39368285714284, "std": 30.769214225191163}, "b,c": {"mean": 106.41716666666667, "std": 102.82329156281233}, "c,a": {"mean": 88.72690684931507, "std": 185.12069938329597}, "g,enter": {"mean": 383.16017272727277, "std": 156.78454928110796}, "enter,m": {"mean": 867.2885543689322, "std": 159.18828598623116}, "enter,e": {"mean": 872.4317500000002, "std": 213.3660949084925}, "n,l": {"mean": 37.97406818181819, "std": 59.18934590670079}, "space,j": {"mean": 72.00971875, "std": 132.5149754908994}, "j,u": {"mean": 126.80932327586206, "std": 58.246483067591}, "j,backspace": {"mean": 504.6686777777778, "std": 166.9259961936944}, "i,b": {"mean": 49.960130588235295, "std": 96.51492489760635}, "f,t": {"mean": 125.37588260869566, "std": 21.089726871823242}, "t,r": {"mean": 109.18177888888889, "std": 98.02776361747556}, "n,w": {"mean": 33.60484952380953, "std": 76.256271620606}, "p,i": {"mean": 74.54844074074074, "std": 127.40179190794376}, "w,l": {"mean": 68.63328666666668, "std": 90.64238988195461}, "r,d": {"mean": 140.3943926380368, "std": 59.44114735197608}, "d,r": {"mean": 209.75324285714282, "std": 87.70066220948105}, "space,enter": {"mean": 226.5310658736059, "std": 200.9483383490526}, "enter,t": {"mean": 930.9957864285713, "std": 189.74809429939708}, "enter,h": {"mean": 891.7501393939394, "std": 215.41165151046184}, "o,f": {"mean": 29.27880448430493, "std": 63.27302182243623}, "f,e": {"mean": 49.68325368421052, "std": 76.77155247708251}, "k,backspace": {"mean": 654.7436413793104, "std": 383.5657595480602}, "s,h": {"mean": 19.02003552631579, "std": 46.79377272584969}, "e,f": {"mean": 76.57067979797979, "std": 122.67408511057569}, "c,n": {"mean": 41.34869245283019, "std": 42.577891126093576}, "u,o": {"mean": 12.83926013986014, "std": 20.229903824953613}, "r,w": {"mean": 184.44024, "std": 157.72483692580062}, "w,u": {"mean": 47.12402000000001, "std": 52.75364255182385}, "u,d": {"mean": 56.88067592592592, "std": 97.30763884715576}, "h,m": {"mean": 176.7948875, "std": 29.133736152458955}, "e,g": {"mean": 86.95791683168316, "std": 103.11551067551326}, "g,backspace": {"mean": 398.8717533333333, "std": 188.32951983050032}, "backspace,g": {"mean": 407.03558507462685, "std": 305.9834774968887}, "b,u": {"mean": 28.484793129770992, "std": 64.21232071725132}, "u,t": {"mean": 34.128839627039625, "std": 92.91222484011737}, "u,p": {"mean": 31.32558782608696, "std": 84.1952043484995}, "f,a": {"mean": 16.173019819819817, "std": 44.062755806759206}, "c,m": {"mean": 91.85596, "std": 51.34324977295847}, "a,y": {"mean": 40.81752098445596, "std": 96.81332458467462}, "s,a": {"mean": 35.05857389380531, "std": 64.72537175366313}, "a,f": {"mean": 55.87288194444445, "std": 163.8427297973123}, "a,o": {"mean": 50.95563207547169, "std": 104.51375226524519}, "r,u": {"mean": 35.523070833333335, "std": 62.6548062159173}, "u,n": {"mean": 142.71460107913668, "std": 59.44546342353115}, "a,g": {"mean": 43.474039273927396, "std": 73.67311104995107}, "o,y": {"mean": 132.4576111111111, "std": 202.12698407706446}, "l,backspace": {"mean": 378.9495973684211, "std": 166.14188151652849}, "l,p": {"mean": 49.77875461538462, "std": 85.30157923132771}, "s,backspace": {"mean": 368.4400261437908, "std": 210.65079740445725}, "k,enter": {"mean": 374.4100142857143, "std": 156.4742988736528}, "s,m": {"mean": 75.16607874015747, "std": 88.33144456261343}, "o,v": {"mean": 29.8939125, "std": 70.44967313695551}, "u,g": {"mean": 46.89559285714286, "std": 93.03501407206907}, "l,c": {"mean": 51.21869130434783, "std": 45.3122233546881}, "t,t": {"mean": 110.79937393617021, "std": 86.91186454735008}, "enter,l": {"mean": 792.6331364705883, "std": 220.0693744019526}, "r,s": {"mean": 48.825511507936504, "std": 94.5327450334492}, "n,y": {"mean": 140.2681679012346, "std": 26.08373845446517}, "t,k": {"mean": 56.72678378378379, "std": 38.12915908011209}, "w,r": {"mean": 37.459160130718956, "std": 89.1091207852348}, "r,i": {"mean": 25.410635737704915, "std": 67.14619177946425}, "p,enter": {"mean": 346.68957500000005, "std": 152.97724751655133}, "enter,w": {"mean": 852.0219000000002, "std": 168.5774498681688}, "i,r": {"mean": 40.32262909090909, "std": 87.20091059407615}, "backspace,y": {"mean": 572.761044117647, "std": 286.9936515868756}, "d,h": {"mean": 137.32948000000002, "std": 137.93990614115117}, "r,k": {"mean": 43.015603846153844, "std": 85.35416622714072}, "s,k": {"mean": 30.9873126984127, "std": 66.06434298284333}, "right,right": {"mean": 340.2362767647059, "std": 313.5047390945303}, "m,n": {"mean": 124.38122970297032, "std": 74.87691682110955}, "s,c": {"mean": 126.63822371134022, "std": 113.70440176125145}, "e,enter": {"mean": 228.17666136363638, "std": 269.4173446096568}, "enter,n": {"mean": 888.2300124999999, "std": 165.71554172485557}, "i,m": {"mean": 49.94991769911504, "std": 72.22500832498471}, "backspace,m": {"mean": 672.8460620155038, "std": 296.70005623270885}, "v,n": {"mean": 61.059667272727275, "std": 43.4529725059384}, "h,g": {"mean": 44.33385294117647, "std": 56.890946149313244}, "k,n": {"mean": 42.37197684210526, "std": 71.55659696126365}, "c,o": {"mean": 19.106338241758245, "std": 49.55370784508582}, "down,down": {"mean": 577.6384333333333, "std": 399.7223724612516}, "right,space": {"mean": 381.2219512820513, "std": 381.76893292115034}, "c,t": {"mean": 158.81148413793102, "std": 170.97361526756538}, "i,s": {"mean": 80.06426637931035, "std": 200.95178393413482}, "o,p": {"mean": 15.477014285714287, "std": 36.66849441323924}, "n,;": {"mean": 53.86046666666667, "std": 62.63746663643981}, "r,p": {"mean": 131.89360000000002, "std": 208.76467978883946}, "p,n": {"mean": 93.66006923076922, "std": 80.39392765286716}, "g,t": {"mean": 76.6721525, "std": 84.80605532394779}, "d,n": {"mean": 50.432561538461535, "std": 74.3565197820457}, "r,enter": {"mean": 342.0766527777778, "std": 313.8139100665229}, "h,v": {"mean": 106.94742142857145, "std": 57.82669295997652}, "a,u": {"mean": 75.88252352941176, "std": 118.6130937949237}, "d,f": {"mean": 365.08901000000003, "std": 357.83154728729676}, "s,f": {"mean": 240.34601999999995, "std": 436.08442651418034}, "r,r": {"mean": 149.31617142857144, "std": 132.560799660002}, "k,o": {"mean": 125.21502857142856, "std": 75.26223551741725}, "c,r": {"mean": 115.42462568807339, "std": 90.0206479788753}, "i,d": {"mean": 43.891149514563104, "std": 73.9026308285027}, "s,l": {"mean": 81.33846000000001, "std": 93.94943988414407}, "k,a": {"mean": 14.392442857142855, "std": 23.820442872538653}, "s,n": {"mean": 34.29445476190475, "std": 76.3691896270479}, "t,u": {"mean": 25.819434615384615, "std": 69.14467176762813}, "l,v": {"mean": 70.11351818181818, "std": 56.73399182761629}, "enter,d": {"mean": 774.6665351351352, "std": 165.06291349099692}, "backspace,d": {"mean": 210.13612441860462, "std": 175.2252985821967}, "i,enter": {"mean": 400.95743478260874, "std": 269.0155419756872}, "enter,g": {"mean": 850.6688175675674, "std": 188.49494228950087}, "c,c": {"mean": 88.18086363636363, "std": 22.602136082047753}, "p,t": {"mean": 278.5632266666667, "std": 232.52832612057819}, "backspace,shift": {"mean": 756.0529916666668, "std": 426.6932387498194}, "backspace,_": {"mean": 583.6596, "std": 192.30727397764582}, "shift,i": {"mean": 403.41273488372093, "std": 364.7325554333195}, "backspace,,": {"mean": 509.9105428571429, "std": 159.35430958049656}, ",,space": {"mean": 195.07154999999997, "std": 225.8261421251191}, "e,q": {"mean": 425.29785714285714, "std": 403.07112542692886}, "r,shift": {"mean": 222.503975, "std": 202.3119481193585}, "d,right": {"mean": 808.02862, "std": 319.07951264551224}, "t,right": {"mean": 530.4898875, "std": 209.16151214190577}, "ctrl_l,left": {"mean": 1079.8143333333333, "std": 367.39980789458946}, "left,shift": {"mean": 692.0611066666665, "std": 242.45092426278342}, "shift,shift": {"mean": 714.0187733333332, "std": 404.3269573487641}, "j,o": {"mean": 134.76274, "std": 212.4705574020137}, "j,i": {"mean": 187.86672000000002, "std": 203.9482307353844}, "enter,i": {"mean": 840.1184403846153, "std": 193.5123383330557}, "s,shift": {"mean": 803.7714333333333, "std": 216.9196488292279}, "shift,enter": {"mean": 570.5747818181818, "std": 294.6663227734375}, "tab,ctrl_l": {"mean": 995.9120769230769, "std": 358.1386664792534}, "enter,tab": {"mean": 321.93833428571423, "std": 245.9411362895171}, "enter,backspace": {"mean": 564.1310105263158, "std": 356.58302380571666}, "/,ctrl_r": {"mean": 829.24352, "std": 295.34446667631613}, "/,<222>": {"mean": 1002.1591199999999, "std": 241.2539389025381}, "enter,enter": {"mean": 294.67382564935065, "std": 292.128847455131}, "d,a": {"mean": 32.745269444444446, "std": 73.36717950558237}, "shift,n": {"mean": 591.6792625, "std": 397.1078354154808}, "right,enter": {"mean": 418.0475896551725, "std": 302.6991426026194}, "m,enter": {"mean": 443.4050666666667, "std": 293.7600792200934}, "backspace,f": {"mean": 301.0605034482759, "std": 227.04997419628089}, ".,right": {"mean": 864.1424799999999, "std": 246.2419859109766}, "space,shift": {"mean": 653.4651252941176, "std": 411.435326869405}, "space,+": {"mean": 525.777475, "std": 232.23645165489387}, "shift,space": {"mean": 194.04269137931038, "std": 241.5774350854445}, "enter,j": {"mean": 599.379775, "std": 353.78812768470743}, ".,i": {"mean": 278.0612, "std": 344.67742932524607}, "space,=": {"mean": 271.56732727272725, "std": 222.4919496407024}, "=,space": {"mean": 132.17821584158415, "std": 137.47752146668026}, "\u0001,\u0003": {"mean": 240.72378571428567, "std": 70.17525247415377}, "c,enter": {"mean": 494.9526, "std": 348.33093712456235}, "h,y": {"mean": 239.57516666666666, "std": 298.12852932247563}, "m,l": {"mean": 94.63329183673468, "std": 38.710387612217176}, "l,t": {"mean": 85.99333789473684, "std": 138.88546725809815}, "v,l": {"mean": 108.50042333333333, "std": 45.7279991566559}, "e,i": {"mean": 68.38482272727272, "std": 76.32148247910361}, "g,n": {"mean": 41.073095714285714, "std": 116.42363573069375}, "e,b": {"mean": 146.40534285714284, "std": 64.71772854363913}, "ctrl_l,enter": {"mean": 338.11274230769226, "std": 274.86036489800694}, "ctrl_l,ctrl_l": {"mean": 990.238893150685, "std": 434.31107224117426}, "r,n": {"mean": 37.56630634146342, "std": 74.63573551068278}, "ctrl_l,backspace": {"mean": 794.8442333333332, "std": 364.5389572339956}, "2,0": {"mean": 262.86053636363636, "std": 157.20252042263695}, "0,space": {"mean": 282.1244285714286, "std": 251.5482861407596}, "space,shift_r": {"mean": 792.3929366666666, "std": 311.0159924308368}, "v,shift": {"mean": 611.8411285714286, "std": 35.121433260605734}, "v,_": {"mean": 871.8023833333333, "std": 95.67430540689276}, "right,left": {"mean": 756.9320941176471, "std": 307.08966803155005}, "left,backspace": {"mean": 506.904335, "std": 213.75336084395323}, "backspace,1": {"mean": 730.3182499999999, "std": 323.67931943898196}, "backspace,right": {"mean": 507.310875, "std": 210.9471133535415}, "space,,": {"mean": 197.60971666666669, "std": 161.04371757940365}, "backspace,shift_r": {"mean": 684.1496821428572, "std": 292.12336813043567}, "shift_r,enter": {"mean": 331.7019722222222, "std": 261.97557448767964}, "i,o": {"mean": 19.987237410071945, "std": 66.93221163393468}, "enter,shift": {"mean": 1230.8711538461537, "std": 257.3062277860987}, "0,1": {"mean": 453.2128125, "std": 205.88944218911175}, "1,.": {"mean": 509.35508571428574, "std": 376.6605924741623}, ".,p": {"mean": 440.9282, "std": 480.94105049951804}, "backspace,(": {"mean": 829.5514625, "std": 135.2087043227704}, "t,,": {"mean": 342.22275, "std": 243.33934365924574}, ",,l": {"mean": 954.5568599999999, "std": 293.4376975586988}, "n,right": {"mean": 523.4492545454546, "std": 101.30895771550746}, "shift,s": {"mean": 487.21626000000003, "std": 371.4658578974579}, "u,a": {"mean": 18.654206329113922, "std": 60.05876456212023}, "t,.": {"mean": 283.22115625, "std": 376.8336788570671}, "o,c": {"mean": 116.51412631578947, "std": 143.40685829413292}, "enter,ctrl_l": {"mean": 662.1327032258064, "std": 398.7593125944938}, "ctrl_l,shift_r": {"mean": 1291.5694555555556, "std": 207.66027500158953}, "backspace,2": {"mean": 333.92269999999996, "std": 210.3064002758802}, "2,right": {"mean": 187.80390000000003, "std": 132.06323589372954}, "right,backspace": {"mean": 427.41346521739143, "std": 220.72690934361407}, "ctrl_l,shift": {"mean": 276.50525555555555, "std": 216.27368682596037}, "enter,\u0016": {"mean": 571.9726153846154, "std": 355.0269098750183}, "tab,tab": {"mean": 411.89756428571434, "std": 310.08912498226704}, "backspace,ctrl_r": {"mean": 774.0565999999999, "std": 263.96368311455853}, "backspace,<222>": {"mean": 923.438, "std": 169.19867537036828}, "y,enter": {"mean": 398.4711555555556, "std": 284.0474621358281}, "backspace,delete": {"mean": 646.9565571428573, "std": 414.54346044823075}, "d,shift": {"mean": 734.6728243243244, "std": 441.8338500218792}, "backspace,tab": {"mean": 649.4585999999999, "std": 317.31896597542357}, "t,shift": {"mean": 621.9261217821783, "std": 395.4832706947473}, "t,(": {"mean": 503.8287, "std": 313.8707702812736}, "m,p": {"mean": 32.229018421052636, "std": 28.89250733444532}, "ctrl_r,ctrl_r": {"mean": 359.72711395348836, "std": 290.7862723706182}, "space,ctrl_l": {"mean": 415.6528533333333, "std": 251.01488563727546}, "space,\u0016": {"mean": 481.2514625, "std": 157.86681553911146}, ",,right": {"mean": 1109.1257894736843, "std": 225.57100692867823}, "ctrl_l,ctrl_r": {"mean": 932.1730333333334, "std": 447.2174501018629}, "backspace,left": {"mean": 1108.2774307692305, "std": 350.67530532085436}, ",,backspace": {"mean": 800.805825, "std": 259.60026224512984}, "\\,backspace": {"mean": 442.71320000000003, "std": 434.4211920545441}, "backspace,enter": {"mean": 499.47216296296295, "std": 307.20188148490746}, "c,ctrl_l": {"mean": 1051.797757142857, "std": 248.48851425496431}, "ctrl_l,space": {"mean": 562.9680833333333, "std": 323.326061387667}, "e,x": {"mean": 342.4182333333333, "std": 279.7142363634576}, "shift,ctrl_l": {"mean": 591.9802222222223, "std": 350.510379139115}, "f,u": {"mean": 20.63646, "std": 14.852151525162945}, "p,p": {"mean": 175.93713684210525, "std": 147.4656294356835}, "a,w": {"mean": 14.387464, "std": 14.881596665381844}, "n,v": {"mean": 230.1934625, "std": 135.2380148659109}, "s,p": {"mean": 227.31283076923074, "std": 232.88898140854874}, "c,i": {"mean": 28.502621428571427, "std": 43.14559657618076}, "s,r": {"mean": 144.854475, "std": 110.96361299524891}, "v,backspace": {"mean": 418.22732333333335, "std": 198.82623216220856}, "r,(": {"mean": 315.2277583333334, "std": 201.78401161251378}, "right,,": {"mean": 864.69458, "std": 113.96658684015065}, "s,enter": {"mean": 281.9021473684211, "std": 162.08988745289437}, "d,i": {"mean": 22.605624637681164, "std": 61.72366844589415}, "s,,": {"mean": 312.6777, "std": 545.7027382917663}, "space,right": {"mean": 843.6988833333331, "std": 334.9897358160439}, "t,s": {"mean": 50.778075757575756, "std": 106.30152332865862}, "shift,backspace": {"mean": 487.8596870967741, "std": 282.9630576167471}, "space,'": {"mean": 107.18668333333335, "std": 99.05524265294959}, "x,i": {"mean": 185.02518333333333, "std": 328.1608003990037}, "enter,r": {"mean": 880.5886073170731, "std": 181.19711862461048}, "shift,right": {"mean": 743.2132608695654, "std": 267.37821416891796}, "space,>": {"mean": 379.9155642857143, "std": 264.19322530575613}, "space,1": {"mean": 463.5200318181818, "std": 372.30138101393356}, "a,x": {"mean": 125.25201176470588, "std": 61.5987781762601}, "shift,ctrl_r": {"mean": 377.7433, "std": 276.6614938464047}, "m,r": {"mean": 20.60786, "std": 23.234267486116273}, "',enter": {"mean": 251.91226666666668, "std": 193.33103515392682}, "enter,u": {"mean": 826.99245, "std": 118.48789860744219}, "p,u": {"mean": 294.8218354430379, "std": 257.41940004111405}, "backspace,k": {"mean": 450.08750975609763, "std": 294.37935118184464}, "enter,s": {"mean": 803.061169047619, "std": 198.32207155716492}, "cmd,c": {"mean": 104.50385, "std": 250.0336850381514}, "t,enter": {"mean": 171.6298489795918, "std": 154.72463184024724}, "m,k": {"mean": 39.3392962962963, "std": 35.12428244561895}, "b,a": {"mean": 33.23772115384615, "std": 139.363365400299}, "v,enter": {"mean": 533.9300117647059, "std": 299.0473439387618}, "down,up": {"mean": 478.50944, "std": 168.38461979205346}, "u,enter": {"mean": 302.7216833333334, "std": 57.85263317690379}, "d,s": {"mean": 427.92355, "std": 428.39613645218606}, "down,left": {"mean": 392.02829, "std": 269.10733097779206}, "e,shift": {"mean": 441.68825846153845, "std": 436.89331488351246}, "e,(": {"mean": 237.4924657894737, "std": 110.32545857608196}, "right,shift_r": {"mean": 588.0338148148148, "std": 188.6007158087094}, "right,{": {"mean": 804.0006615384615, "std": 191.17714913754142}, "[,enter": {"mean": 286.21853333333337, "std": 349.2147771272763}, "shift,1": {"mean": 353.85604705882355, "std": 129.47827177991778}, "1,0": {"mean": 151.55682727272728, "std": 154.1132203263921}, "0,;": {"mean": 430.11861612903226, "std": 324.13980531145273}, "left,right": {"mean": 608.7354214285714, "std": 355.04486350980085}, "n,i": {"mean": 92.42642857142857, "std": 127.40362919965611}, ";,enter": {"mean": 366.76345769230767, "std": 306.38272953796843}, "i,i": {"mean": 290.45862222222223, "std": 240.42417167099936}, "n,shift": {"mean": 772.4534307692309, "std": 557.9939962157229}, "shift,m": {"mean": 647.7581545454545, "std": 357.214049248841}, "i,,": {"mean": 224.4459272727273, "std": 108.7527961618299}, ",,d": {"mean": 217.13432000000003, "std": 303.05797581637347}, ",,i": {"mean": 365.5653739130434, "std": 362.8392361167051}, "right,;": {"mean": 694.7939181818182, "std": 114.6944259145802}, ";,shift": {"mean": 647.2350166666666, "std": 511.938902230095}, ";,alt_l": {"mean": 712.63698, "std": 401.1787599258635}, ";,down": {"mean": 836.2933857142858, "std": 226.00285994434694}, "n,left": {"mean": 902.0460599999999, "std": 221.03013411127992}, "-,space": {"mean": 165.86361874999997, "std": 277.6330270817334}, "t,>": {"mean": 329.4549733333333, "std": 416.59708525488503}, "backspace,;": {"mean": 592.3965555555556, "std": 315.386989336586}, "shift,a": {"mean": 558.6395705882352, "std": 260.8876344335504}, "space,:": {"mean": 216.64637499999998, "std": 219.93591091696322}, "i,a": {"mean": 28.92627717391304, "std": 139.01769791661263}, "shift,;": {"mean": 452.18847894736837, "std": 361.49743304969184}, "h,shift": {"mean": 855.6606647058824, "std": 307.11355995424645}, "shift,b": {"mean": 532.8461500000001, "std": 193.93231386514515}, "c,k": {"mean": 21.88570142857143, "std": 49.08022706611375}, "k,shift": {"mean": 185.58904, "std": 66.44018611167792}, "space,0": {"mean": 392.18698571428564, "std": 348.5341140591288}, ":,space": {"mean": 48.77568, "std": 18.029356801106356}, "tab,backspace": {"mean": 391.98174000000006, "std": 162.25207574531797}, "f,shift": {"mean": 69.59170333333333, "std": 139.09387686093515}, "=,=": {"mean": 119.35500000000002, "std": 94.75218350476152}, "backspace,=": {"mean": 324.5116157894737, "std": 239.52415918393234}, "m,;": {"mean": 120.34114285714286, "std": 263.7642804368642}, ";,ctrl_r": {"mean": 715.54609, "std": 296.3779594409356}, ";,<222>": {"mean": 848.6678823529411, "std": 244.68182963449692}, ";,i": {"mean": 208.0395, "std": 215.4387955359635}, "i,shift": {"mean": 424.6594, "std": 257.9441399096324}, "s,.": {"mean": 199.33016923076926, "std": 215.23176796129258}, ".,s": {"mean": 298.0980875, "std": 225.72402865077822}, "i,z": {"mean": 121.89778666666668, "std": 120.81174561885318}, "z,e": {"mean": 77.60406000000002, "std": 108.22647956467216}, "e,)": {"mean": 362.9558285714285, "std": 160.4277838870741}, "i,+": {"mean": 689.26308, "std": 220.55125127331652}, "+,+": {"mean": 149.5304086956522, "std": 173.2722500888073}, "[,i": {"mean": 230.10643125, "std": 203.94208702812767}, "s,[": {"mean": 420.97679166666666, "std": 384.7029089358446}, "backspace,down": {"mean": 883.5097571428572, "std": 395.57334719355634}, "j,space": {"mean": 81.40847692307692, "std": 54.34825535278025}, "0,right": {"mean": 777.2332681818182, "std": 343.46541712177896}, "e,;": {"mean": 94.8442, "std": 60.85564039366715}, ";,backspace": {"mean": 423.7894666666666, "std": 98.4899935007895}, "f,(": {"mean": 164.61621785714289, "std": 185.17016196539728}, "space,-": {"mean": 272.1252875, "std": 287.47064759085015}, "i,]": {"mean": 197.67170000000002, "std": 138.77558937454177}, "1,,": {"mean": 522.28684, "std": 411.5596471042447}, "j,shift": {"mean": 948.018175, "std": 340.49646755383725}, "j,+": {"mean": 797.6172428571429, "std": 381.33335208101005}, "s,;": {"mean": 58.673966666666665, "std": 70.57345223190437}, "shift,c": {"mean": 768.1023272727274, "std": 326.18819455332306}, "backspace,0": {"mean": 606.4338500000001, "std": 262.15543431360527}, "right,shift": {"mean": 912.3204214285714, "std": 313.6341279713563}, "enter,v": {"mean": 872.1234666666668, "std": 227.9846517333666}, "r,<": {"mean": 305.4415583333334, "std": 190.68146409630583}, "c,v": {"mean": 69.78173333333334, "std": 169.08827612434348}, "d,p": {"mean": 109.5979, "std": 132.53740815989775}, "p,shift": {"mean": 176.92517142857145, "std": 67.9239740472225}, "shift,(": {"mean": 520.2489714285714, "std": 199.87372219491638}, "-,1": {"mean": 184.74012142857143, "std": 169.64043918514483}, "1,right": {"mean": 847.0390000000002, "std": 464.4525734638742}, "p,[": {"mean": 491.73423125, "std": 290.42838077339246}, "up,up": {"mean": 219.29205172413796, "std": 232.92438485489845}, "up,right": {"mean": 951.9127172413793, "std": 308.0707975336324}, "0,backspace": {"mean": 934.9287, "std": 327.67958847831824}, "delete,delete": {"mean": 155.59656315789476, "std": 74.42045924878602}, "r,ctrl_l": {"mean": 540.17514, "std": 238.2103185148586}, "r,\u0012": {"mean": 651.70362, "std": 242.52200377816771}, "m,m": {"mean": 320.3900142857143, "std": 421.98102935450424}, "l,shift": {"mean": 674.9396249999999, "std": 335.664382592926}, "&,&": {"mean": 80.36826250000001, "std": 17.486008693948534}, ".,space": {"mean": 171.00625, "std": 145.51045132796875}, "-,-": {"mean": 115.39774166666666, "std": 95.5054297116265}, "-,;": {"mean": 473.75187999999997, "std": 266.02626359843794}, "l,;": {"mean": 50.356397435897435, "std": 86.60201989650776}, "1,;": {"mean": 491.46931666666666, "std": 416.3995280956677}, "backspace,-": {"mean": 320.88136666666674, "std": 309.20736143669023}, "a,p": {"mean": 288.45928, "std": 387.38572410307995}, "p,<": {"mean": 344.07186, "std": 156.82280955488076}, "shift,h": {"mean": 760.55862, "std": 182.51138095923113}, "p,;": {"mean": 161.94561249999998, "std": 80.1178659577943}, "t,down": {"mean": 1083.481142857143, "std": 202.70209153286896}, "space,<": {"mean": 158.89671, "std": 78.47315345189652}, ";,space": {"mean": 102.09033571428571, "std": 239.70151537105355}, "=,backspace": {"mean": 273.49695666666673, "std": 250.71033285788954}, "c,right": {"mean": 1150.07512, "std": 414.1908012940384}, "shift_r,ctrl_r": {"mean": 965.7677499999999, "std": 223.32428648204296}, "t,<": {"mean": 325.53682000000003, "std": 180.6461205973314}, "t,;": {"mean": 83.82097000000002, "std": 139.30576647741506}, "d,(": {"mean": 482.05935, "std": 436.2026711772512}, "t,y": {"mean": 34.60642857142857, "std": 72.43604118979158}, "v,a": {"mean": 35.154403125, "std": 84.10954633503815}, "p,backspace": {"mean": 338.31822, "std": 115.53663386868946}, "g,v": {"mean": 129.6126928571429, "std": 39.526899391995684}, "b,n": {"mean": 29.410764285714283, "std": 58.70389271365725}, "t,n": {"mean": 36.64053641618497, "std": 57.9837213439609}, "enter,y": {"mean": 933.0629153846154, "std": 206.15759296226176}, "g,u": {"mean": 140.4165142857143, "std": 130.98334965490218}, "s,g": {"mean": 89.48363333333333, "std": 60.71752529546602}, "l,f": {"mean": 71.89259583333335, "std": 166.4345739368188}, "l,enter": {"mean": 317.954736, "std": 194.6202044875709}, "enter,p": {"mean": 993.8517796610171, "std": 157.75401724434064}, "b,space": {"mean": 99.3151, "std": 98.79111468029039}, ",,n": {"mean": 497.6494799999999, "std": 332.9532248193514}, "space,&": {"mean": 789.6569400000001, "std": 294.52043884892}, "n,(": {"mean": 276.14793636363635, "std": 65.38041071407012}, "shift,=": {"mean": 572.0812166666667, "std": 156.94182637649973}, ",,m": {"mean": 211.226825, "std": 181.56513097630935}, "o,=": {"mean": 358.86015714285713, "std": 73.59411746196366}, "[,n": {"mean": 647.5775000000001, "std": 267.0554948328843}, "e,k": {"mean": 62.826575000000005, "std": 91.544713501733}, "k,l": {"mean": 99.51232727272728, "std": 131.9762005368674}, "i,w": {"mean": 31.882236842105264, "std": 86.10620770799164}, "m,c": {"mean": 94.39436, "std": 86.0017298339888}, "k,d": {"mean": 99.5836, "std": 188.25344526398447}, "h,enter": {"mean": 393.9244789473684, "std": 260.3868233044275}, "y,h": {"mean": 124.21507999999999, "std": 105.60008271978577}, "w,t": {"mean": 59.900315, "std": 57.54278092067913}, "d,l": {"mean": 27.930606153846153, "std": 119.24564455355157}, "l,g": {"mean": 56.6145, "std": 40.059888304886734}, "w,enter": {"mean": 249.70801428571434, "std": 231.0108358810908}, "g,y": {"mean": 108.8474285714286, "std": 128.19647204735298}, "n,,": {"mean": 236.72816666666665, "std": 133.98641950823057}, "o,;": {"mean": 56.40162631578948, "std": 116.14280904412844}, "d,enter": {"mean": 207.93886206896548, "std": 196.75795715683952}, "<,<": {"mean": 76.18476000000001, "std": 19.29511983778282}, "g,l": {"mean": 51.72650000000001, "std": 47.27934156777255}, "shift,o": {"mean": 812.1257333333333, "std": 360.5188120245307}, "m,y": {"mean": 161.4633153846154, "std": 51.444054463014574}, "v,o": {"mean": 32.501558333333335, "std": 108.33826911278058}, "v,i": {"mean": 26.01778181818182, "std": 33.25341510997078}, "d,shift_r": {"mean": 991.8078846153848, "std": 325.95088392349464}, "m,,": {"mean": 43.27493333333333, "std": 74.405012880107}, "cmd,space": {"mean": 10.0, "std": 0.0}, "b,g": {"mean": 81.93222222222222, "std": 106.65412507288094}, "h,h": {"mean": 269.406, "std": 241.26132351456582}, "cmd,tab": {"mean": 37.79793103448275, "std": 147.09282495873165}, "tab,cmd": {"mean": 10.0, "std": 0.0}, "r,;": {"mean": 24.67625, "std": 38.8297076790117}, ";,o": {"mean": 14.01720930232558, "std": 18.188704177424707}, "e,h": {"mean": 41.06068965517241, "std": 47.567594513494555}, "k,i": {"mean": 136.31799999999998, "std": 84.24631502920468}, "p,s": {"mean": 150.12428571428572, "std": 88.47189494929091}, "y,;": {"mean": 11.376000000000001, "std": 4.128}, "b,m": {"mean": 88.698125, "std": 119.30962970453966}, "b,v": {"mean": 10.0, "std": 0.0}, "k,c": {"mean": 10.0, "std": 0.0}, "o,a": {"mean": 44.59043478260869, "std": 102.86959306846623}, ";,m": {"mean": 10.470769230769232, "std": 2.902016438320779}, "cmd,cmd": {"mean": 519.5525, "std": 256.3895615440496}, "h,9": {"mean": 10.0, "std": 0.0}, "o,i": {"mean": 14.043766233766233, "std": 22.88408197619921}, "k,s": {"mean": 38.05733333333333, "std": 62.708284669748565}, ";,y": {"mean": 10.0, "std": 0.0}, "l,b": {"mean": 25.277142857142856, "std": 37.42120472760473}, "l,k": {"mean": 155.150625, "std": 146.89237765149483}, "enter,k": {"mean": 640.1653846153846, "std": 215.86041114983345}, "o,9": {"mean": 10.0, "std": 0.0}, ";,u": {"mean": 10.0, "std": 0.0}, "y,t": {"mean": 23.82314285714286, "std": 35.9405247414931}, ";,t": {"mean": 13.448571428571428, "std": 8.447240341540846}, "space,;": {"mean": 84.51829268292683, "std": 212.31747080558443}, "r,f": {"mean": 117.67500000000001, "std": 181.7705874401393}, ";,l": {"mean": 10.0, "std": 0.0}, ";,a": {"mean": 10.0, "std": 0.0}, "j,h": {"mean": 57.83399999999999, "std": 95.668}, "f,l": {"mean": 147.6536363636364, "std": 237.73033711913348}, "\\,\\": {"mean": 1016.2433333333333, "std": 268.668391495704}, "p,d": {"mean": 233.21166666666667, "std": 162.37133402755822}, "l,u": {"mean": 149.1977777777778, "std": 372.0771719766921}, ";,e": {"mean": 57.278333333333336, "std": 79.45497308049525}, "c,g": {"mean": 106.684, "std": 55.14463622148576}, "n,n": {"mean": 241.42499999999998, "std": 155.9881614899028}, "t,g": {"mean": 88.19666666666667, "std": 171.75925881561346}, "space,cmd": {"mean": 116.36833333333334, "std": 237.84682398669017}, "h,;": {"mean": 13.7725, "std": 9.981096820991167}, ";,n": {"mean": 29.95285714285714, "std": 31.549438172170223}, "u,;": {"mean": 26.51142857142857, "std": 38.16826184469104}, "space,x": {"mean": 256.77599999999995, "std": 254.47098959213406}, "n,m": {"mean": 119.16555555555556, "std": 155.31056264509442}, "l,w": {"mean": 10.0, "std": 0.0}, "u,y": {"mean": 13.622499999999999, "std": 9.584234124331479}, "n,p": {"mean": 25.636000000000003, "std": 31.272000000000002}, "9,o": {"mean": 10.0, "std": 0.0}, "c,d": {"mean": 77.54400000000001, "std": 85.86775823322745}, "b,enter": {"mean": 385.69571428571425, "std": 213.83847801935144}, "space,\\": {"mean": 272.448, "std": 123.17634097504276}, "cmd,z": {"mean": 10.0, "std": 0.0}, ".,5": {"mean": 548.0220000000002, "std": 257.12016306777656}, "ctrl_r,right": {"mean": 269.6449358974359, "std": 324.4894137767841}, "right,ctrl_r": {"mean": 57.0565625, "std": 137.74308259875917}, "left,alt_r": {"mean": 10.0, "std": 0.0}, "cmd,q": {"mean": 10.0, "std": 0.0}, "9,3": {"mean": 326.62399999999997, "std": 177.3390101021205}, "cmd,shift": {"mean": 10.0, "std": 0.0}, "c,cmd": {"mean": 10.0, "std": 0.0}, "v,cmd": {"mean": 10.0, "std": 0.0}, "0,0": {"mean": 132.5240909090909, "std": 55.63596863533281}, "cmd,v": {"mean": 10.0, "std": 0.0}}};

const KEY_NEIGHBORS = {
  'a': ['q', 'w', 's', 'z'],
  'b': ['v', 'g', 'h', 'n'],
  'c': ['x', 'd', 'f', 'v'],
  'd': ['s', 'e', 'r', 'f', 'c', 'x'],
  'e': ['w', 'r', 'd', 's'],
  'f': ['d', 'r', 't', 'g', 'v', 'c'],
  'g': ['f', 't', 'y', 'h', 'b', 'v'],
  'h': ['g', 'y', 'u', 'j', 'n', 'b'],
  'i': ['u', 'o', 'k', 'j'],
  'j': ['h', 'u', 'i', 'k', 'm', 'n'],
  'k': ['j', 'i', 'o', 'l', 'm'],
  'l': ['k', 'o', 'p'],
  'm': ['n', 'j', 'k'],
  'n': ['b', 'h', 'j', 'm'],
  'o': ['i', 'p', 'l', 'k'],
  'p': ['o', 'l'],
  'q': ['w', 'a'],
  'r': ['e', 't', 'f', 'd'],
  's': ['a', 'w', 'e', 'd', 'x', 'z'],
  't': ['r', 'y', 'g', 'f'],
  'u': ['y', 'i', 'j', 'h'],
  'v': ['c', 'f', 'g', 'b'],
  'w': ['q', 'e', 's', 'a'],
  'x': ['z', 's', 'd', 'c'],
  'y': ['t', 'u', 'h', 'g'],
  'z': ['a', 's', 'x'],
  '1': ['2', 'q'],
  '2': ['1', '3', 'q', 'w'],
  '3': ['2', '4', 'w', 'e'],
  '4': ['3', '5', 'e', 'r'],
  '5': ['4', '6', 'r', 't'],
  '6': ['5', '7', 't', 'y'],
  '7': ['6', '8', 'y', 'u'],
  '8': ['7', '9', 'u', 'i'],
  '9': ['8', '0', 'i', 'o'],
  '0': ['9', '-', 'o', 'p'],
  '-': ['0', '='],
  '=': ['-']
};

let typingTimeline = [];
let playbackIndex = 0;
let playbackTimeout = null;
let typingState = 'idle'; // 'idle', 'typing', 'paused'
let originalText = '';
let speedMultiplier = 1.0;
let customErrorRate = 0.058;
let typingMode = 'human';

let bypassIdeIndent = false;
let bypassIdeBrackets = false;

let stats = {
  totalKeystrokes: 0,
  correctKeystrokes: 0,
  startEpochMs: 0,
  elapsedSeconds: 0,
  wpmInterval: null
};

// UI Elements
const btnStart = document.getElementById('btn-start');
const btnPause = document.getElementById('btn-pause');
const btnStop = document.getElementById('btn-stop');
const btnClear = document.getElementById('btn-clear');
const btnLoadSample = document.getElementById('btn-load-sample');

const txtInput = document.getElementById('typing-input');
const valSerialStatus = document.getElementById('serial-status');
const valBtStatus = document.getElementById('bt-status');
const valSpeedMultiplier = document.getElementById('speed-slider');
const bubbleSpeedVal = document.getElementById('speed-val');
const valErrorRate = document.getElementById('error-slider');
const bubbleErrorVal = document.getElementById('error-val');
const selectMode = document.getElementById('select-mode');

const checkIdeIndent = document.getElementById('check-ide-indent');
const checkIdeBrackets = document.getElementById('check-ide-brackets');

const statWpm = document.getElementById('stat-wpm');
const statAccuracy = document.getElementById('stat-accuracy');
const statProgress = document.getElementById('stat-progress');
const statEta = document.getElementById('stat-eta');

const visualizer = document.getElementById('typing-visualizer');
const terminalStateBadge = document.getElementById('terminal-state');
const progressBarFill = document.getElementById('progress-bar-fill');
const charsTypedBadge = document.getElementById('chars-typed');
const charsTotalBadge = document.getElementById('chars-total');

function randomNormal(mean, std) {
  let u = 0, v = 0;
  while(u === 0) u = Math.random();
  while(v === 0) v = Math.random();
  let num = Math.sqrt(-2.0 * Math.log(u)) * Math.cos(2.0 * Math.PI * v);
  return num * std + mean;
}

function getTypoChar(char) {
  const lower = char.toLowerCase();
  if (KEY_NEIGHBORS[lower]) {
    const list = KEY_NEIGHBORS[lower];
    const picked = list[Math.floor(Math.random() * list.length)];
    return char === char.toUpperCase() ? picked.toUpperCase() : picked;
  }
  return ' ';
}

function getESP32Keycode(char) {
  if (char === '\n') return 176;
  if (char === '\t') return 179;
  if (char === 'backspace') return 178;
  if (char === 'right') return 215;
  if (char === 'delete') return 212;
  return char.charCodeAt(0);
}

// Initialize Wi-Fi status on load since we are loaded from the ESP32 web server
window.addEventListener('DOMContentLoaded', () => {
  valSerialStatus.textContent = 'Wi-Fi Active';
  valSerialStatus.className = 'status-val connected';
  valBtStatus.textContent = 'Ready';
  valBtStatus.className = 'status-val connected';
  
  btnStart.disabled = false;
  
  btnStart.addEventListener('click', startTyping);
  btnPause.addEventListener('click', pauseTyping);
  btnStop.addEventListener('click', stopTyping);
  
  btnClear.addEventListener('click', () => {
    txtInput.value = '';
    updateTotalCharsBadge();
  });
  
  btnLoadSample.addEventListener('click', () => {
    txtInput.value = `def calculate_fibonacci(n):
    # This is a human typing simulation test
    if n <= 0:
        return []
    elif n == 1:
        return [0]
        
    sequence = [0, 1]
    while len(sequence) < n:
        next_num = sequence[-1] + sequence[-2]
        sequence.append(next_num)
        
    return sequence

print(calculate_fibonacci(10))`;
    updateTotalCharsBadge();
  });
  
  valSpeedMultiplier.addEventListener('input', (e) => {
    speedMultiplier = parseFloat(e.target.value);
    bubbleSpeedVal.textContent = speedMultiplier.toFixed(1) + 'x';
    if (typingState === 'typing') {
      updateETA();
    }
  });
  
  valErrorRate.addEventListener('input', (e) => {
    customErrorRate = parseFloat(e.target.value) / 100;
    bubbleErrorVal.textContent = e.target.value + '%';
  });
  
  selectMode.addEventListener('change', (e) => {
    typingMode = e.target.value;
  });

  checkIdeIndent.addEventListener('change', (e) => {
    bypassIdeIndent = e.target.checked;
  });
  checkIdeBrackets.addEventListener('change', (e) => {
    bypassIdeBrackets = e.target.checked;
  });
  
  txtInput.addEventListener('input', updateTotalCharsBadge);
  updateTotalCharsBadge();
  
  // Start polling ESP32 status
  setInterval(pollESP32Status, 2000);
});

function updateTotalCharsBadge() {
  const len = txtInput.value.length;
  charsTotalBadge.textContent = len;
  if (typingState === 'idle') {
    charsTypedBadge.textContent = 0;
    progressBarFill.style.width = '0%';
  }
}

async function pollESP32Status() {
  try {
    const res = await fetch('/status');
    const status = await res.text();
    if (status === 'idle' && typingState === 'typing') {
      // If ESP32 finished but browser is still playing, force stop
      stopTyping();
    }
  } catch (err) {
    console.warn('Status poll failed:', err);
  }
}

function buildTypingTimeline(text) {
  const timeline = [];
  let currentTimeOffset = 0;
  let prevChar = 'space';
  
  originalText = text;
  
  const OPENING_BRACKETS = new Set(['(', '{', '[', '"', "'"]);
  
  for (let i = 0; i < text.length; i++) {
    const char = text[i];
    const doTypo = (typingMode === 'human') && (Math.random() < customErrorRate) && (char !== '\n' && char !== '\t');
    
    if (doTypo) {
      const typoChar = getTypoChar(char);
      if (typoChar && typoChar !== char) {
        const flightTime = sampleFlightTime(prevChar, typoChar);
        const holdTime = sampleHoldTime(typoChar);
        
        currentTimeOffset += flightTime;
        const pressOffset = currentTimeOffset;
        currentTimeOffset += holdTime;
        const releaseOffset = currentTimeOffset;
        
        timeline.push({
          type: 'press',
          char: typoChar,
          code: getESP32Keycode(typoChar),
          offset: pressOffset,
          isTypo: true,
          charIndex: i
        });
        
        timeline.push({
          type: 'release',
          char: typoChar,
          code: getESP32Keycode(typoChar),
          offset: releaseOffset,
          isTypo: true,
          charIndex: i
        });
        
        const hesitateDelay = randomNormal(200, 40);
        currentTimeOffset += Math.max(50, hesitateDelay);
        
        const bsFlight = sampleFlightTime(typoChar, 'backspace');
        const bsHold = sampleHoldTime('backspace');
        
        currentTimeOffset += bsFlight;
        const bsPressOffset = currentTimeOffset;
        currentTimeOffset += bsHold;
        const bsReleaseOffset = currentTimeOffset;
        
        timeline.push({
          type: 'press',
          char: 'backspace',
          code: getESP32Keycode('backspace'),
          offset: bsPressOffset,
          isCorrection: true,
          charIndex: i
        });
        
        timeline.push({
          type: 'release',
          char: 'backspace',
          code: getESP32Keycode('backspace'),
          offset: bsReleaseOffset,
          isCorrection: true,
          charIndex: i
        });
        
        const resumeDelay = randomNormal(260, 50);
        currentTimeOffset += Math.max(50, resumeDelay);
        prevChar = 'backspace';
      }
    }
    
    const flightTime = sampleFlightTime(prevChar, char);
    const holdTime = sampleHoldTime(char);
    
    currentTimeOffset += flightTime;
    const pressOffset = currentTimeOffset;
    currentTimeOffset += holdTime;
    const releaseOffset = currentTimeOffset;
    
    timeline.push({
      type: 'press',
      char: char,
      code: getESP32Keycode(char),
      offset: pressOffset,
      isTypo: false,
      charIndex: i
    });
    
    timeline.push({
      type: 'release',
      char: char,
      code: getESP32Keycode(char),
      offset: releaseOffset,
      isTypo: false,
      charIndex: i
    });
    
    prevChar = char;
    
    if (bypassIdeBrackets && OPENING_BRACKETS.has(char)) {
      currentTimeOffset += 20;
      const delPressOffset = currentTimeOffset;
      currentTimeOffset += 15;
      const delReleaseOffset = currentTimeOffset;
      
      timeline.push({
        type: 'press',
        char: 'delete',
        code: 212,
        offset: delPressOffset,
        isTypo: false,
        charIndex: i
      });
      
      timeline.push({
        type: 'release',
        char: 'delete',
        code: 212,
        offset: delReleaseOffset,
        isTypo: false,
        charIndex: i
      });
      
      prevChar = 'delete';
    }
    
    if (bypassIdeIndent && char === '\n') {
      while (i + 1 < text.length && (text[i + 1] === ' ' || text[i + 1] === '\t')) {
        i++;
      }
    }
  }
  
  return timeline;
}

function sampleHoldTime(char) {
  if (typingMode === 'robot') return 5;
  const key = char === ' ' ? 'space' : char.toLowerCase();
  const stats = TYPING_PARAMS.hold_stats[key] || TYPING_PARAMS.global_hold;
  let val = randomNormal(stats.mean, stats.std);
  return Math.min(400, Math.max(30, val));
}

function sampleFlightTime(char1, char2) {
  if (typingMode === 'robot') return 5;
  const key1 = char1 === ' ' ? 'space' : char1.toLowerCase();
  const key2 = char2 === ' ' ? 'space' : char2.toLowerCase();
  const digraph = key1 + ',' + key2;
  const stats = TYPING_PARAMS.flight_stats[digraph] || TYPING_PARAMS.global_flight;
  let val = randomNormal(stats.mean, stats.std);
  if (char1 === '\n' || char1 === '.') {
    val += randomNormal(450, 150);
  } else if (char1 === ',' || char1 === ';') {
    val += randomNormal(250, 80);
  }
  return Math.min(1500, Math.max(10, val));
}

async function startTyping() {
  if (typingState === 'typing') return;
  const text = txtInput.value;
  if (!text) {
    alert('Please enter some text to type.');
    return;
  }
  
  if (typingState === 'idle') {
    typingTimeline = buildTypingTimeline(text);
    playbackIndex = 0;
    
    // Generate the compressed command queue for the ESP32
    // Command format: P<code>,<delay>;R<code>,<delay>;
    let queueString = "";
    for (let i = 0; i < typingTimeline.length; i++) {
      const ev = typingTimeline[i];
      let delay = 0;
      if (i > 0) {
        delay = Math.round((ev.offset - typingTimeline[i - 1].offset) / speedMultiplier);
      } else {
        delay = Math.round(ev.offset / speedMultiplier);
      }
      const action = ev.type === 'press' ? 'P' : 'R';
      queueString += action + ev.code + ',' + delay + ';';
    }
    
    // Send the queue to the ESP32
    try {
      terminalStateBadge.textContent = 'Sending...';
      const res = await fetch('/start', {
        method: 'POST',
        headers: { 'Content-Type': 'text/plain' },
        body: queueString
      });
      if (!res.ok) throw new Error('ESP32 start failed');
    } catch (err) {
      alert('Error sending typing commands to ESP32: ' + err.message);
      terminalStateBadge.textContent = 'Error';
      return;
    }
    
    stats.totalKeystrokes = 0;
    stats.correctKeystrokes = 0;
    stats.startEpochMs = Date.now();
    stats.elapsedSeconds = 0;
    
    renderVisualizer(0, []);
    
    if (stats.wpmInterval) clearInterval(stats.wpmInterval);
    stats.wpmInterval = setInterval(() => {
      if (typingState === 'typing') {
        stats.elapsedSeconds += 1;
        updateSpeedAndWPM();
      }
    }, 1000);
  }
  
  typingState = 'typing';
  updateUIForState();
  playNextEvent();
}

async function pauseTyping() {
  // FreeRTOS task handles stop best; we map pause to stopping the task locally
  stopTyping();
}

async function stopTyping() {
  typingState = 'idle';
  if (playbackTimeout) clearTimeout(playbackTimeout);
  if (stats.wpmInterval) clearInterval(stats.wpmInterval);
  
  try {
    await fetch('/stop', { method: 'POST' });
  } catch(e) {}
  
  playbackIndex = 0;
  updateUIForState();
  updateTotalCharsBadge();
}

function playNextEvent() {
  if (typingState !== 'typing') return;
  if (playbackIndex >= typingTimeline.length) {
    stopTyping();
    terminalStateBadge.textContent = 'Finished';
    statProgress.textContent = '100';
    progressBarFill.style.width = '100%';
    alert('Typing finished successfully!');
    return;
  }
  
  const currentEvent = typingTimeline[playbackIndex];
  let delay = 0;
  if (playbackIndex > 0) {
    const prevEvent = typingTimeline[playbackIndex - 1];
    delay = (currentEvent.offset - prevEvent.offset) / speedMultiplier;
  } else {
    delay = currentEvent.offset / speedMultiplier;
  }
  
  playbackTimeout = setTimeout(() => {
    executeEvent(currentEvent);
    playbackIndex++;
    playNextEvent();
  }, delay);
}

function executeEvent(event) {
  if (event.type === 'press') {
    stats.totalKeystrokes++;
    if (!event.isTypo) {
      stats.correctKeystrokes++;
    }
  }
  updateLiveDashboard(event);
}

function updateUIForState() {
  if (typingState === 'typing') {
    terminalStateBadge.textContent = 'Typing...';
    terminalStateBadge.style.color = '#10b981';
    btnPause.disabled = true; // Disabled for simplicity in wireless mode
    btnStop.disabled = false;
    txtInput.disabled = true;
  } else if (typingState === 'idle') {
    terminalStateBadge.textContent = 'Idle';
    terminalStateBadge.style.color = '#6b7280';
    btnPause.disabled = true;
    btnStop.disabled = true;
    txtInput.disabled = false;
    
    statWpm.textContent = '0';
    statAccuracy.textContent = '100';
    statProgress.textContent = '0';
    statEta.textContent = '--:--';
  }
}

function updateLiveDashboard(currentEvent) {
  const activeCharIndex = currentEvent.charIndex;
  const totalChars = originalText.length;
  const progressPercent = Math.round((activeCharIndex / totalChars) * 100);
  
  statProgress.textContent = progressPercent;
  progressBarFill.style.width = progressPercent + '%';
  charsTypedBadge.textContent = activeCharIndex;
  
  const acc = Math.round((stats.correctKeystrokes / Math.max(1, stats.totalKeystrokes)) * 100);
  statAccuracy.textContent = acc;
  
  updateETA();
  renderVisualizer(activeCharIndex, currentEvent);
}

function updateETA() {
  if (typingTimeline.length === 0 || playbackIndex >= typingTimeline.length) {
    statEta.textContent = '00:00';
    return;
  }
  const currentOffset = typingTimeline[playbackIndex].offset;
  const endOffset = typingTimeline[typingTimeline.length - 1].offset;
  const remainingMs = (endOffset - currentOffset) / speedMultiplier;
  const totalSec = Math.ceil(remainingMs / 1000);
  const min = Math.floor(totalSec / 60);
  const sec = totalSec % 60;
  statEta.textContent = `${min.toString().padStart(2, '0')}:${sec.toString().padStart(2, '0')}`;
}

function updateSpeedAndWPM() {
  if (stats.elapsedSeconds <= 0) return;
  const currentTypedChars = parseInt(charsTypedBadge.textContent) || 0;
  const wpm = Math.round((currentTypedChars * 12) / stats.elapsedSeconds);
  statWpm.textContent = wpm;
}

function renderVisualizer(charIndex, event) {
  visualizer.innerHTML = '';
  let currentBuffer = [];
  for (let idx = 0; idx < playbackIndex; idx++) {
    const ev = typingTimeline[idx];
    if (ev.type === 'press') {
      if (ev.char === 'backspace') {
        currentBuffer.pop();
      } else {
        currentBuffer.push({
          char: ev.char,
          isTypo: ev.isTypo
        });
      }
    }
  }
  currentBuffer.forEach(item => {
    const span = document.createElement('span');
    span.textContent = item.char;
    span.className = 'typed-char ' + (item.isTypo ? 'error' : 'correct');
    visualizer.appendChild(span);
  });
  const cursor = document.createElement('span');
  cursor.className = 'blinking-cursor';
  visualizer.appendChild(cursor);
  const remainingText = originalText.substring(charIndex + 1);
  if (remainingText.length > 0) {
    const pendingSpan = document.createElement('span');
    pendingSpan.textContent = remainingText;
    pendingSpan.className = 'typed-char pending';
    visualizer.appendChild(pendingSpan);
  }
  visualizer.scrollTop = visualizer.scrollHeight;
}

</script>
</body>
</html>
)rawliteral";

#endif
