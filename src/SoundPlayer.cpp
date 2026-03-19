#include "SoundPlayer.h"

enum { MSG_INIT_FAILED, MSG_SCAN, MSG_READY, MSG_TIMEOUT, MSG_T_OUT, MSG_FINISHED, MSG_ERROR };

#if (DFP_LANGUAGE == 0)
  const char* const DFP_TEXTS[] PROGMEM = {
    "DFPlayer Init fehlgeschlagen!", "\nWarte auf Dateisystem-Scan ", 
    "\nPlayer bereit: Anzahl Dateien auf SD: ", "\nTimeout: Player antwortet nicht.", 
    "DFP: Zeitüberschreitung!", "DFP: Wiedergabe beendet!", "DFP: Fehler "
  };
#else
  const char* const DFP_TEXTS[] PROGMEM = {
    "DFPlayer init failed!", "\nWaiting for file system scan ", 
    "\nPlayer ready: Number of files on SD: ", "\nTimeout: Player does not respond.", 
    "DFP: Time Out!", "DFP: Finished!", "DFP: Error "
  };
#endif

SoundPlayer::SoundPlayer() { _serialPtr = nullptr; }

const char* SoundPlayer::getTxt(int msgIdx) {
  return (const char*)pgm_read_ptr(&DFP_TEXTS[msgIdx]);
}

#if defined(ARDUINO_ARCH_ESP32)
void SoundPlayer::init(int rx, int tx, HardwareSerial* hwSerialPtr) {
  _serialPtr = hwSerialPtr;
  if (_serialPtr) _serialPtr->begin(9600, SERIAL_8N1, rx, tx);
#else
void SoundPlayer::init(int rx, int tx) {
  if (_serialPtr) delete _serialPtr;
  _serialPtr = new SoftwareSerial(rx, tx);
  _serialPtr->begin(9600);
#endif
  _ready = false;
  _initFailed = true;
  delay(500);
  
  if (!_serialPtr || !myDFPlayer.begin(*_serialPtr, false, true)) {
    Serial.println((__FlashStringHelper*)getTxt(MSG_INIT_FAILED));
    return;
  }

  Serial.print((__FlashStringHelper*)getTxt(MSG_SCAN));
  int fileCount = -1;
  unsigned long startWait = millis();
  
  while (fileCount == -1 && (millis() - startWait < 30000)) {
    Serial.print(F("|."));
    fileCount = myDFPlayer.readFileCounts();
    delay(1000);
  }
  
  if (fileCount != -1) {
    Serial.print((__FlashStringHelper*)getTxt(MSG_READY));
    Serial.println(fileCount);
    _ready = true;
    _initFailed = false;
  } else {
    Serial.println((__FlashStringHelper*)getTxt(MSG_TIMEOUT));
    return;
  }

  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(20);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
}

int SoundPlayer::getMaxFiles() { return _initFailed ? -1 : myDFPlayer.readFileCounts(); }
bool SoundPlayer::initFailed() { return _initFailed; }
bool SoundPlayer::isReady()    { return _ready && !_initFailed; }
bool SoundPlayer::isPlaying()  { return _doesPlay; }
void SoundPlayer::setReady(bool v) { _ready = v; }

bool SoundPlayer::playingTitle(int value) { 
  playTitle(value); 
  return _doesPlay; 
}

void SoundPlayer::playTitle(int value) {
  if (!_initFailed && value > 0) { 
    myDFPlayer.playMp3Folder(value); 
    _doesPlay = true; 
    _ready = false; 
  }
}

void SoundPlayer::playTitle(int cat, int wordNo) {
  if (!_initFailed) { 
    myDFPlayer.playFolder(cat, wordNo); 
    _doesPlay = true; 
    _ready = false; 
  }
}

void SoundPlayer::handlePlayerStatus() {
  if (!_initFailed && myDFPlayer.available()) {
    uint8_t type = myDFPlayer.readType();
    printDetail(type, myDFPlayer.read());
    if (type == DFPlayerPlayFinished) { 
      _doesPlay = false; 
      _ready = true; 
    }
  }
}

void SoundPlayer::printDetail(uint8_t type, int value) {
  if (!_verbose) return;
  switch (type) {
    case TimeOut:              Serial.println((__FlashStringHelper*)getTxt(MSG_T_OUT)); break;
    case DFPlayerPlayFinished: Serial.println((__FlashStringHelper*)getTxt(MSG_FINISHED)); break;
    case DFPlayerError:        
      Serial.print((__FlashStringHelper*)getTxt(MSG_ERROR)); 
      Serial.println(value); 
      break;
    default: break;
  }
}
