#include "SoundPlayerEC.h"

SoundPlayer::SoundPlayer() { 
  _serialPtr = nullptr; 
}

void SoundPlayer::printMsg(int msgIdx, bool newLine) {
  const __FlashStringHelper* ptr = nullptr;
  #if (DFP_LANGUAGE == 0)
    switch (msgIdx) {
      case 0: ptr = F("DFPlayer Init fehlgeschlagen!"); break;
      case 1: ptr = F("\nWarte auf Dateisystem-Scan "); break;
      case 2: ptr = F("\nPlayer bereit: Anzahl Dateien auf SD: "); break;
      case 3: ptr = F("\nTimeout: Player antwortet nicht."); break;
      case 4: ptr = F("DFP: Zeitueberschreitung!"); break;
      case 5: ptr = F("DFP: Wiedergabe beendet!"); break;
      case 6: ptr = F("DFP: Fehler "); break;
      default: return;
    }
  #else
    switch (msgIdx) {
      case 0: ptr = F("DFPlayer init failed!"); break;
      case 1: ptr = F("\nWaiting for file system scan "); break;
      case 2: ptr = F("\nPlayer ready: Number of files on SD: "); break;
      case 3: ptr = F("\nTimeout: Player does not respond."); break;
      case 4: ptr = F("DFP: Time Out!"); break;
      case 5: ptr = F("DFP: Finished!"); break;
      case 6: ptr = F("DFP: Error "); break;
      default: return;
    }
  #endif
  if (newLine) Serial.println(ptr);
  else Serial.print(ptr);
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
  delay(1000); 

  if (!_serialPtr || !myDFPlayer.begin(*_serialPtr, false, false)) {
    printMsg(0, true); 
    return;
  }

  while(_serialPtr->available()) _serialPtr->read(); 

  printMsg(1, false); 
  
  int fileCount = -1;
  unsigned long startWait = millis();
  
  while (fileCount == -1 && (millis() - startWait < 30000)) {
    Serial.print(F("|."));
    fileCount = myDFPlayer.readFileCounts();
    if (fileCount == -1) {
       delay(1000);
       while(_serialPtr->available()) _serialPtr->read(); 
    }
  }
  
  if (fileCount != -1) {
    while(_serialPtr->available()) _serialPtr->read(); 
    printMsg(2, false);
    Serial.println(fileCount);
    _ready = true;
    _initFailed = false;
  } else {
    printMsg(3, true);
    return;
  }

  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(20);
  delay(200); 
  while(_serialPtr->available()) _serialPtr->read(); 

  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  delay(200);
  while(_serialPtr->available()) _serialPtr->read(); 
}

void SoundPlayer::handlePlayerStatus() {
  if (!_initFailed && myDFPlayer.available()) {
    uint8_t type = myDFPlayer.readType();
    int value = myDFPlayer.read();
    printDetail(type, value);
    if (type == DFPlayerPlayFinished) { 
      _doesPlay = false; 
      _ready = true; 
    }
  }
}

void SoundPlayer::playTitle(int value) {
  if (!_initFailed && value > 0) { 
    myDFPlayer.playMp3Folder(value); 
    _doesPlay = true; 
    _ready = false; 
    delay(100);
  }
}

void SoundPlayer::playTitle(int cat, int wordNo) {
  if (!_initFailed) { 
    myDFPlayer.playFolder(cat, wordNo); 
    _doesPlay = true; 
    _ready = false; 
    delay(100);
  }
}

bool SoundPlayer::playingTitle(int value) { 
  playTitle(value); 
  return _doesPlay; 
}

void SoundPlayer::printDetail(uint8_t type, int value) {
  if (!_verbose) return;
  switch (type) {
    case TimeOut:              printMsg(4, true); break;
    case DFPlayerPlayFinished: printMsg(5, true); break;
    case DFPlayerError:        
      printMsg(6, false); 
      Serial.println(value); 
      break;
    default: break;
  }
}

int SoundPlayer::getMaxFiles() { 
  return _initFailed ? -1 : myDFPlayer.readFileCounts(); 
}

bool SoundPlayer::initFailed() { return _initFailed; }
bool SoundPlayer::isReady()    { return _ready && !_initFailed; }
bool SoundPlayer::isPlaying()  { return _doesPlay; }
void SoundPlayer::setReady(bool v) { _ready = v; }
