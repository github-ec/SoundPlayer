# SoundPlayer Class Reference

Wrapper class for the `DFRobotDFPlayerMini` to handle audio playback and serial communication.

`#include <SoundPlayer.h>`

## Description

The `SoundPlayer` class manages communication with the DFPlayer Mini hardware. It handles the abstraction between **HardwareSerial** (ESP32) and **SoftwareSerial** (AVR) and provides high-level methods for playback control and status monitoring.

---

## Public Functions


| Return Type | Name and Description |
| :--- | :--- |
| | **[SoundPlayer](#function-soundplayer)**()<br>Constructor for the SoundPlayer class. |
| void | **[init](#function-init-esp32)**(int rx, int tx, HardwareSerial* hwSerialPtr = &Serial2)<br>*(ESP32 only)* Initializes the player using HardwareSerial. |
| void | **[init](#function-init-avr)**(int rx, int tx)<br>*(AVR only)* Initializes the player using SoftwareSerial. |
| void | **[handlePlayerStatus](#function-handleplayerstatus)**()<br>Updates and checks the player status (should be called in the main loop). |
| void | **[playTitle](#function-playtitle-1)**(int value)<br>Plays a title based on a combined ID value. |
| void | **[playTitle](#function-playtitle-2)**(int cat, int wordNo)<br>Plays a specific title from a category. |
| bool | **[playingTitle](#function-playingtitle)**(int value)<br>Checks if a specific title ID is currently playing. |
| bool | **[isReady](#function-isready)**()<br>Returns `true` if the player is initialized and ready. |
| bool | **[initFailed](#function-initfailed)**()<br>Returns `true` if the initial hardware handshake failed. |
| bool | **[isPlaying](#function-isplaying)**()<br>Returns `true` if audio is currently playing. |
| void | **[setVolume](#function-setvolume)**(byte value)<br>Sets the playback volume (0-30). |
| int | **[getMaxFiles](#function-getmaxfiles)**()<br>Retrieves the total number of files on the SD card. |

---

## Function Documentation

### <a name="function-soundplayer"></a> SoundPlayer()
Initializes the internal state and flags.

### <a name="function-init-esp32"></a> void init(int rx, int tx, HardwareSerial* hwSerialPtr)
**Note: ESP32 Architecture only.**
Initializes the serial communication and the DFPlayer Mini.
* **Parameters:**
    * `rx`: RX Pin number.
    * `tx`: TX Pin number.
    * `hwSerialPtr`: Pointer to a HardwareSerial instance (defaults to `Serial2`).

### <a name="function-init-avr"></a> void init(int rx, int tx)
**Note: AVR Architecture only.**
Initializes the player using `SoftwareSerial` on the specified pins.
* **Parameters:**
    * `rx`: RX Pin for SoftwareSerial.
    * `tx`: TX Pin for SoftwareSerial.

### <a name="function-playtitle-2"></a> void playTitle(int cat, int wordNo)
Starts playback of a file located in a specific folder/category.
* **Parameters:**
    * `cat`: Category/Folder index.
    * `wordNo`: Index of the file within that category.

### <a name="function-setvolume"></a> void setVolume(byte value)
Sets the volume level. Includes a small delay (100ms) to ensure the hardware processes the command.
* **Parameters:**
    * `value`: Volume level from `0` (mute) to `30` (max).

### <a name="function-handleplayerstatus"></a> void handlePlayerStatus()
This method queries the DFPlayer for its current state and handles error or finished-playing messages. For accurate status tracking, this must be called frequently in the `loop()`.

---

## Debugging

* **`messagesOn()`**: Enables verbose output to the standard `Serial` monitor for debugging.
* **`messagesOff()`**: Disables debug output.

