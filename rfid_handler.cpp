#include "rfid_handler.h"
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN D3
#define SDA_PIN D4

MFRC522 rfid(SDA_PIN, RST_PIN);

// UID ที่อนุญาต
String allowedUID = "DEADBEEF"; // แก้ไขเป็น UID จริง

void rfidSetup() {
  rfid.PCD_Init();
}

bool readRFID(String &cardUID) {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return false;
  }

  cardUID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    cardUID += String(rfid.uid.uidByte[i], HEX);
  }
  cardUID.toUpperCase();
  
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  return true;
}

bool isAuthorizedUID(String cardUID) {
  return (cardUID == allowedUID);
}
