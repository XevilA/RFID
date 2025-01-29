#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "rfid_handler.h"

// การตั้งค่าขา
#define RELAY_PIN D1
#define BUZZER_PIN D2

// WiFi Credentials
const char *ssid = "your_wifi_name";
const char *password = "your_wifi_password";

// Line Notify Token
const char *lineToken = "your_line_notify_token";

// LCD I2C (Address: 0x27 หรือ 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// โหมด Developer สำหรับ Debug
#define DEVELOPER_MODE true

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfidSetup();

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("RFID System Ready");

  connectWiFi();
}

void loop() {
  String cardUID;
  if (readRFID(cardUID)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("UID: " + cardUID);

    if (isAuthorizedUID(cardUID)) {
      Serial.println("✅ Access Granted: " + cardUID);
      lcd.setCursor(0, 1);
      lcd.print("Access Granted");
      triggerRelayAndBuzzer();
      sendLineNotify("✅ Access Granted: " + cardUID);
    } else {
      Serial.println("❌ Access Denied: " + cardUID);
      lcd.setCursor(0, 1);
      lcd.print("Access Denied!");
      buzzDenied();
      sendLineNotify("❌ Access Denied: " + cardUID);
    }
  }
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
  Serial.print("🔄 Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Connected to WiFi");
}

void sendLineNotify(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();
    if (!client.connect("notify-api.line.me", 443)) {
      Serial.println("❌ Line Notify connection failed.");
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
    Serial.println("📩 Line Notify sent: " + message);
  } else {
    Serial.println("❌ WiFi not connected.");
  }
}
