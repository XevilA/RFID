
#include <SPI.h>

#include <MFRC522.h>

#include <ESP8266WiFi.h>

#include <WiFiClientSecure.h>

#include <Wire.h>

#include <LiquidCrystal_I2C.h>

// Setขา

#define RST_PIN D3

#define SDA_PIN D4

#define RELAY_PIN D1

#define BUZZER_PIN D2

MFRC522 rfid(SDA_PIN, RST_PIN);

LiquidCrystal_I2C lcd(0x27, 16, 2);

// WiFi 

const char *ssid = "SSID;

const char *password = "Password";

// Line Notify Token

const char *lineToken = "notify_token";

// UID ที่อนุญาต

byte allowedUID[4] = {0xDE, 0xAD, 0xBE, 0xEF}; // ตัวอย่าง UID

void setup() {

  Serial.begin(115200);

  SPI.begin();

  rfid.PCD_Init();

  lcd.begin();

  lcd.backlight();

  lcd.setCursor(0, 0);

  lcd.print("Ready...");

  pinMode(RELAY_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);

  digitalWrite(BUZZER_PIN, LOW);

  connectWiFi();

}

void loop() {

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {

    return;

  }

  lcd.clear();

  lcd.setCursor(0, 0);

  String uidStr = "UID: ";

  for (byte i = 0; i < rfid.uid.size; i++) {

    uidStr += String(rfid.uid.uidByte[i], HEX);

    if (i < rfid.uid.size - 1) uidStr += ":";

  }

  lcd.print(uidStr);

  if (isAuthorizedUID(rfid.uid.uidByte, rfid.uid.size)) {

    Serial.println("Access Granted!");

    lcd.setCursor(0, 1);

    lcd.print("Access Granted");

    triggerRelayAndBuzzer();

    sendLineNotify("Access Granted: Name Surname, " + uidStr);

  } else {

    Serial.println("Access Denied!");

    lcd.setCursor(0, 1);

    lcd.print("Access Denied");

    buzzDenied();

    sendLineNotify("Access Denied: " + uidStr);

  }

  rfid.PICC_HaltA();

  rfid.PCD_StopCrypto1();

}

bool isAuthorizedUID(byte *uid, byte size) {

  for (byte i = 0; i < size; i++) {

    if (uid[i] != allowedUID[i]) {

      return false;

    }

  }

  return true;

}

void triggerRelayAndBuzzer() {

  digitalWrite(RELAY_PIN, HIGH);

  digitalWrite(BUZZER_PIN, HIGH);

  delay(1000);

  digitalWrite(RELAY_PIN, LOW);

  digitalWrite(BUZZER_PIN, LOW);

}

void buzzDenied() {

  for (int i = 0; i < 3; i++) {

    digitalWrite(BUZZER_PIN, HIGH);

    delay(200);

    digitalWrite(BUZZER_PIN, LOW);

    delay(200);

  }

}

void connectWiFi() {

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

  Serial.println("\nConnected to WiFi");

}

void sendLineNotify(String message) {

  if (WiFi.status() == WL_CONNECTED) {

    WiFiClientSecure client;

    client.setInsecure();

    if (!client.connect("notify-api.line.me", 443)) {

      Serial.println("Line Notify connection failed.");

      return;

    }

    String payload = "message=" + message;

    String request = String("POST /api/notify HTTP/1.1\r\n") +

                     "Host: notify-api.line.me\r\n" +

                     "Authorization: Bearer " + String(lineToken) + "\r\n" +

                     "Content-Type: application/x-www-form-urlencoded\r\n" +

                     "Content-Length: " + payload.length() + "\r\n\r\n" +

                     payload;

    client.print(request);

    Serial.println("Line Notify sent: " + message);

  } else {

    Serial.println("WiFi not connected.");

  }

}
