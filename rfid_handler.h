#ifndef RFID_HANDLER_H
#define RFID_HANDLER_H

#include <MFRC522.h>

void rfidSetup();
bool readRFID(String &cardUID);
bool isAuthorizedUID(String cardUID);

#endif
