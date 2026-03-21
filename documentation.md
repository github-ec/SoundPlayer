# 📘 Documentation: `SoundPlayerEC` Class

## 🧩 Overview

The `SoundPlayerEC` class provides a wrapper for controlling a DFPlayer Mini MP3 module.
It supports initialization, playback, and status handling on microcontrollers such as Arduino (AVR) and ESP32.

---

## ⚙️ Supported Platforms

* ESP32 (HardwareSerial)
* AVR (SoftwareSerial)

Other architectures are not supported.

---

## 🔐 Private Members

### 🔊 Audio Controller

* `DFRobotDFPlayerMini myDFPlayer`
  Instance used to control the DFPlayer Mini module.

### 🔌 Serial Communication

* `HardwareSerial* _serialPtr` (ESP32)
* `SoftwareSerial* _serialPtr` (AVR)
  Pointer to the serial interface used for communication.

### 🧠 State Variables

* `bool _ready`
  Indicates whether the player is initialized and ready.

* `bool _verbose`
  Enables or disables debug/status messages.

* `bool _doesPlay`
  Indicates whether audio is currently playing.

* `bool _initFailed`
  Indicates whether initialization has failed.

---

## 🔒 Private Methods

### `void printMsg(int msgIdx, bool newLine)`

Internal helper method for safe output of status messages.

### `void printDetail(uint8_t type, int value)`

Outputs detailed status information (e.g., error codes or events).

---

## 🚀 Public Methods

### 🔧 Constructor

#### `SoundPlayerEC()`

Creates a new instance without initializing hardware.

---

### ⚡ Initialization

#### ESP32:

```cpp
void init(int rx, int tx, HardwareSerial* hwSerialPtr = &Serial2);
```

#### AVR:

```cpp
void init(int rx, int tx);
```

**Parameters:**

* `rx` – RX pin
* `tx` – TX pin
* `hwSerialPtr` – serial interface (ESP32 only)

**Description:**
Initializes serial communication and the DFPlayer module.
Sets internal state flags such as `_ready` and `_initFailed`.

---

### ▶️ Playback

#### `void playTitle(int value)`

Plays a track by its index.

#### `void playTitle(int cat, int wordNo)`

Plays a track from a specific folder (`cat`) and file number (`wordNo`).

---

### 🔍 Status Queries

#### `bool playingTitle(int value)`

Checks if a specific track is currently playing.

#### `bool isPlaying()`

Returns whether any track is currently playing.

#### `bool isReady()`

Returns whether the player is ready.

#### `bool initFailed()`

Returns whether initialization failed.

---

### 🛠️ State Control

#### `void setReady(bool value)`

Manually sets the ready state.

---

### 🔊 Volume Control

#### `void setVolume(byte value)`

Sets the playback volume.

**Note:**

* Typical range: 0–30
* Includes a `delay(100)` for stabilization

---

### 📢 Debug Output

#### `void messagesOn()`

Enables debug messages.

#### `void messagesOff()`

Disables debug messages.

---

### 🔄 Runtime Handling

#### `void handlePlayerStatus()`

Must be called regularly in the main loop to process player events and state changes.

---

### 📁 File Handling

#### `int getMaxFiles()`

Returns the maximum number of available audio files.

---

## 🧪 Example Usage

```cpp
SoundPlayerEC player;

void setup() {
  player.init(10, 11);
  player.setVolume(20);
}

void loop() {
  player.handlePlayerStatus();

  if (!player.isPlaying()) {
    player.playTitle(1);
  }
}
```

---

## ⚠️ Notes

* The DFPlayer requires a properly formatted SD card (FAT16/FAT32).
* File names and folder structure must follow DFPlayer conventions.
* `handlePlayerStatus()` should be called frequently for proper operation.
