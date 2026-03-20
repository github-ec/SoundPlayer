# SoundPlayer Example: Robust Random Playback

This sketch demonstrates how to use the `SoundPlayer` class to play random audio files from an SD card. It is designed to work cross-platform on both **ESP32** and **Arduino AVR (Nano/Uno)**.

## Features
- **Cross-Platform Support:** Automatically selects the correct Serial type (Hardware vs. Software Serial) based on the board.
- **Robust Initialization:** Includes error handling and hardware stabilization delays.
- **Non-Blocking Logic:** Uses `millis()` for timing to ensure the `handlePlayerStatus()` method is called frequently.

## Hardware Setup


| Component | ESP32 Pins | AVR (Nano/Uno) Pins |
| :--- | :--- | :--- |
| **DFPlayer RX** | GPIO 17 (TX_PIN) | D11 (TX_PIN) |
| **DFPlayer TX** | GPIO 16 (RX_PIN) | D10 (RX_PIN) |
| **VCC** | 5V | 5V |
| **GND** | GND | GND |

*Note: A 1k Ohm resistor on the RX line of the Player is recommended to reduce noise.*

## Code Overview

### 1. Configuration & Initialization
The sketch defines pins based on the architecture and initializes the `SoundPlayer`. 
- **ESP32:** Uses `HardwareSerial` (defaults to `Serial2`).
- **AVR:** Uses `SoftwareSerial` on pins 10 and 11.

### 2. Setup Phase
- Sets the Serial monitor to **115200 Baud**.
- Calls `myPlayer.init()` to establish communication with the DFPlayer Mini.
- Disables internal verbose messages (`messagesOff`) to keep the console clean.
- Sets an initial volume of **20** (Scale 0-30).

### 3. Main Loop Logic
The loop performs two main tasks:
- **Status Updates:** Continuously calls `myPlayer.handlePlayerStatus()` to process feedback (e.g., when a song finishes).
- **Timed Action:** Every 5 seconds, if the player is ready, it selects a random folder (1-9) and a random file (1-10) and triggers playback using `playTitle(folder, file)`.

## Requirements
- `SoundPlayer` Library (Header and CPP files).
- `DFRobotDFPlayerMini` Library.
- An SD card formatted in FAT32 with folders named `01` to `09` and files named `001.mp3` to `010.mp3`.
