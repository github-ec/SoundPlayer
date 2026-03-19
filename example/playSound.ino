#include "SoundPlayer.h"

// Instance of the SoundPlayer
SoundPlayer myPlayer;

// Architecture-dependent pin definitions
#if defined(ARDUINO_ARCH_ESP32)
  constexpr int RX_PIN = 16;
  constexpr int TX_PIN = 17;
#else
  // SoftwareSerial Pins for NANO
  constexpr int RX_PIN = 10;
  constexpr int TX_PIN = 11;
#endif

unsigned long lastAction = 0;

void setup() {
  // Using 9600 for Nano/AVR is often more stable with SoftwareSerial,
  // but we stay at 115200 if your Serial Monitor is set to it.
  Serial.begin(115200);
  delay(1000); // Give hardware time to stabilize

  Serial.println(F("\n--- SoundPlayer Robust Test ---"));

  // Initialize player based on architecture
  #if defined(ARDUINO_ARCH_ESP32)
    myPlayer.init(RX_PIN, TX_PIN, &Serial2);
  #else
    myPlayer.init(RX_PIN, TX_PIN);
  #endif

  // CRITICAL: Disable verbose internal messages to prevent serial garbage
  myPlayer.messagesOff();

  if (myPlayer.initFailed()) {
    Serial.println(F("[ERROR] Player init failed!"));
    while(1); 
  }

  myPlayer.setVolume(20);
  randomSeed(analogRead(0));
  
  Serial.println(F("[SYSTEM] Ready. Playing random folders 01-09..."));
}

void loop() {
  // Update player status (processes feedback from DFPlayer)

  if (!myPlayer.isReady()){
    lastAction = millis();
  }
  // Trigger new sound every 5 seconds if player is ready
  if (myPlayer.isReady() && (millis() - lastAction > 5000)) {
    
    int randomFolder = random(1, 10); // Folders 01 to 09
    int randomFile = random(1, 11);   // Files 01 to 10
    
    Serial.print(F("[ACTION] Playing Folder "));
    Serial.print(randomFolder);
    Serial.print(F(", File "));
    Serial.println(randomFile);

    // Play specific file in folder
    myPlayer.playTitle(randomFolder, randomFile);
    
    lastAction = millis();
  }
  myPlayer.handlePlayerStatus();

  // Small delay to prevent CPU/Serial congestion (like in your working sketch)
  delay(50); 
}
