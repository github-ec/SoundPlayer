#include "SoundPlayer.h"

// Instance of the SoundPlayer
SoundPlayer myPlayer;

// Pin definitions based on architecture
#if defined(ARDUINO_ARCH_ESP32)
  constexpr int RX_PIN = 16;
  constexpr int TX_PIN = 17;
#else
  constexpr int RX_PIN = 10;
  constexpr int TX_PIN = 11;
#endif

unsigned long lastAction = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println(F("--- SoundPlayer Test: Folders 01-09 ---"));

  // Initialize player
  #if defined(ARDUINO_ARCH_ESP32)
    myPlayer.init(RX_PIN, TX_PIN, &Serial2);
  #else
    myPlayer.init(RX_PIN, TX_PIN);
  #endif

  if (myPlayer.initFailed()) {
    Serial.println(F("Init failed!"));
    while(1); 
  }

  myPlayer.messagesOn();
  myPlayer.setVolume(20);
  
  // Seed random generator with noise from analog pin
  randomSeed(analogRead(0));
  
  Serial.println(F("Ready. Press any key or wait for auto-play."));
}

void loop() {
  // Essential: Keep checking the player status to free up the 'isReady' state
  myPlayer.handlePlayerStatus();

  // Play a random file every 5 seconds if the player is ready
  if (myPlayer.isReady() && (millis() - lastAction > 5000)) {
    
    // Pick a random folder between 1 and 9
    int randomFolder = random(1, 10); // 1 to 9
    
    // Pick one of the first 10 files in that folder
    int randomFile = random(1, 11);   // 1 to 10
    
    Serial.print(F("Action: Playing Folder "));
    Serial.print(randomFolder);
    Serial.print(F(", File "));
    Serial.println(randomFile);

    // Command: playFolder(folder, file)
    myPlayer.playTitle(randomFolder, randomFile);
    
    lastAction = millis();
  }
}

