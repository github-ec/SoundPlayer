#pragma once
#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"

/**
 * @file SoundPlayer.h
 * @brief Wrapper class for the DFRobotDFPlayerMini to handle audio playback.
 * 
 * Supports both ESP32 (HardwareSerial) and AVR (SoftwareSerial) architectures.
 */

#define DFP_LANGUAGE 1 

#if defined(ARDUINO_ARCH_ESP32)
#elif defined(__AVR__)
  #include <SoftwareSerial.h>
#else
  #error "Unsupported architecture!"
#endif

/**
 * @class SoundPlayer
 * @brief Manages audio playback, player status, and serial communication with the DFPlayer Mini.
 */
class SoundPlayer {
private:
  DFRobotDFPlayerMini myDFPlayer; ///< Internal DFPlayer Mini instance
  
  #if defined(ARDUINO_ARCH_ESP32)
    HardwareSerial* _serialPtr = nullptr; ///< Pointer to ESP32 HardwareSerial
  #else
    SoftwareSerial* _serialPtr = nullptr; ///< Pointer to AVR SoftwareSerial
  #endif

  bool _ready = false;      ///< Player readiness state
  bool _verbose = false;    ///< Toggle for serial debug output
  bool _doesPlay = false;   ///< Current playback status
  bool _initFailed = false; ///< Error flag if hardware initialization fails

  /**
   * @brief Internal helper to print safe text messages.
   * @param msgIdx Index of the message to print.
   * @param newLine Whether to append a line break.
   */
  void printMsg(int msgIdx, bool newLine);

  /**
   * @brief Handles detailed error or status reports from the DFPlayer.
   * @param type Message type (e.g., Error, Timeout).
   * @param value Specific error code or value.
   */
  void printDetail(uint8_t type, int value);                                                              

public:
  /**
   * @brief Constructor for SoundPlayer.
   */
  SoundPlayer();
  
  #if defined(ARDUINO_ARCH_ESP32)
    /**
     * @brief Initializes the player for ESP32.
     * @param rx RX Pin.
     * @param tx TX Pin.
     * @param hwSerialPtr Pointer to the HardwareSerial instance (defaults to Serial2).
     */
    void init(int rx, int tx, HardwareSerial* hwSerialPtr = &Serial2);
  #else
    /**
     * @brief Initializes the player for AVR (Uno/Mega).
     * @param rx RX Pin for SoftwareSerial.
     * @param tx TX Pin for SoftwareSerial.
     */
    void init(int rx, int tx);
  #endif

  /**
   * @brief Regularly updates and checks the player status (should be called in loop).
   */
  void handlePlayerStatus();

  /**
   * @brief Plays a title based on a combined ID value.
   * @param value Encoded ID or title number.
   */
  void playTitle(int value);

  /**
   * @brief Plays a specific title from a category.
   * @param cat Category index.
   * @param wordNo Word/File index within the category.
   */
  void playTitle(int cat, int wordNo);

  /**
   * @brief Checks if a specific title is currently being played.
   * @param value The title ID to check.
   * @return true if currently playing, false otherwise.
   */
  bool playingTitle(int value);

  /** @return true if the player is initialized and ready. */
  bool isReady();

  /** @param value Set the readiness state manually. */
  void setReady(bool value);

  /** @return true if the initial hardware handshake failed. */
  bool initFailed();

  /** @return true if audio is currently playing. */
  bool isPlaying();

  /** @brief Enables serial debug messages. */
  void messagesOn()  { _verbose = true; }

  /** @brief Disables serial debug messages. */
  void messagesOff() { _verbose = false; }

  /**
   * @brief Sets the playback volume.
   * @param value Volume level (0-30).
   */
  void setVolume(byte value) { myDFPlayer.volume(value); delay(100);}

  /**
   * @brief Retrieves the maximum number of files on the SD card.
   * @return Total file count.
   */
  int getMaxFiles();
};
