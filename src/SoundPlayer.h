#pragma once
#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"

#define DFP_LANGUAGE 1 

#if defined(ARDUINO_ARCH_ESP32)
#elif defined(__AVR__)
  #include <SoftwareSerial.h>
#else
  #error "Unsupported architecture!"
#endif

class SoundPlayer {
private:
  DFRobotDFPlayerMini myDFPlayer;
  
  #if defined(ARDUINO_ARCH_ESP32)
    HardwareSerial* _serialPtr = nullptr;
  #else
    SoftwareSerial* _serialPtr = nullptr;
  #endif

  bool _ready = false;
  bool _verbose = false;
  bool _doesPlay = false;
  bool _initFailed = false;

  // NEU: Deklaration der sicheren Textausgabe
  void printMsg(int msgIdx, bool newLine);
  void printDetail(uint8_t type, int value);                                                              

public:
  SoundPlayer();
  
  #if defined(ARDUINO_ARCH_ESP32)
    void init(int rx, int tx, HardwareSerial* hwSerialPtr = &Serial2);
  #else
    void init(int rx, int tx);
  #endif

  void handlePlayerStatus();
  void playTitle(int value);
  void playTitle(int cat, int wordNo);
  bool playingTitle(int value);
  bool isReady();
  void setReady(bool value);
  bool initFailed();
  bool isPlaying();
  void messagesOn()  { _verbose = true; }
  void messagesOff() { _verbose = false; }
  void setVolume(byte value) { myDFPlayer.volume(value); }
  int getMaxFiles();
};
