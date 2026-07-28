# ESP32 Human Typer

A hardware-based human-like auto-typing system using an ESP32 as a Bluetooth LE HID Keyboard, controlled via a Web Serial interface. 

This system compiles keystroke hold times, transition speeds (digraphs), and typo/correction dynamics from real human typing logs to generate a highly authentic, non-repetitive typing profile that is virtually indistinguishable from a human coder.

---

## Repository Structure

*   `index.html` - The Web Control Center UI (features real-time statistics dashboard and typing visualizer).
*   `style.css` - Dark-mode glassmorphic interface styles.
*   `app.js` - Human-like typing simulation scheduler and Web Serial controller.
*   `typing_model_params.json` - Keystroke biometric metrics compiled from human typing logs.
*   `esp32_autotyper/esp32_autotyper.ino` - ESP32 Arduino firmware.

---

## Getting Started

For full upload, pairing, and operation instructions, please refer to the detailed guide in the walkthrough document.
