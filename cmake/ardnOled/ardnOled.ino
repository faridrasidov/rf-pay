#include <SoftwareSerial.h>
#include <MFRC522.h>
#include "DEFINES.h"

#define RELAY1_PIN 7
#define SS_PIN 10
#define RST_PIN 9
#define RX_PIN 5
#define TX_PIN 6
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define oLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define SPK_Pin 8
bool cardDetect = false;

MFRC522 rfReader(SS_PIN, RST_PIN);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, oLED_RESET);
SoftwareSerial espSerial(RX_PIN, TX_PIN);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  executeDisplay(display, 2, 30, 5, "RF-Pay");
  executeDisplay(display, 1, 20, 22, "by Farid Rashidov");
  display.display();

  pinMode(RELAY1_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, HIGH);

  Serial.begin(9600);
  Serial.setTimeout(100);
  espSerial.begin(9600);
  espSerial.setTimeout(100);

  SPI.begin();
  rfReader.PCD_Init();
  espSerial.readString();
  delay(2000);
}

void loop() {
  int i = 0;
  byte nuidPICC[4], buffer[18];
  byte size = sizeof(buffer);
  String incomingData ,firstData, secondData;
  String machName = "Dev1";
  if (cardDetect) {
    display.clearDisplay();
    executeDisplay(display, 1, 34, 0, "Card Found");
    display.display();
    espSerial.readString();
    executeDisplay(display, 1, 50, 8, "Wait");
    display.display();
    String cardId = getHex(rfReader.uid.uidByte, rfReader.uid.size);
    espSerial.print(machName);
    espSerial.print(":");
    espSerial.print(cardId);
    pendingTone(SPK_Pin);
    delay(3500);

    incomingData = espSerial.readStringUntil('.');
    int colonIndex = incomingData.indexOf(':');
    firstData = incomingData.substring(0, colonIndex);
    secondData = incomingData.substring(colonIndex + 1);

    if (firstData == "200") {
      display.clearDisplay();
      executeDisplay(display, 1, 40, 0, "Success");
      String resMoney = "Money : " + secondData;
      executeDisplay(display, 1, 20, 16, resMoney);
      display.display();
      digitalWrite(RELAY1_PIN, LOW);
      successTone(SPK_Pin);
      delay(1000);
      digitalWrite(RELAY1_PIN, HIGH);
      display.clearDisplay();
      rfReader.PICC_HaltA();
      rfReader.PCD_StopCrypto1();
    }

    else if (firstData == "400") {
      display.clearDisplay();
      executeDisplay(display, 1, 25, 0, "Balance Error");
      String resMoney = "Money : " + secondData;
      executeDisplay(display, 1, 20, 16, resMoney);
      display.display();
      warningTone(SPK_Pin);
      display.clearDisplay();
      rfReader.PICC_HaltA();
      rfReader.PCD_StopCrypto1();
    }

    else if (firstData == "800") {
      display.clearDisplay();
      executeDisplay(display, 1, 20, 0, "Not Registered");
      display.display();
      warningTone(SPK_Pin);
      display.clearDisplay();
      rfReader.PICC_HaltA();
      rfReader.PCD_StopCrypto1();
    }

    else {
      espSerial.print("FIX:");
      espSerial.print(machName);
      espSerial.print("/");
      espSerial.print(cardId);
      display.clearDisplay();
      executeDisplay(display, 1, 15, 8, "Server's Down Or");
      executeDisplay(display, 1, 15, 16, "Device Not Conn");
      display.display();
      problemTone(SPK_Pin);
      display.clearDisplay();
      rfReader.PICC_HaltA();
      rfReader.PCD_StopCrypto1();
    }
    cardDetect = false;
  }

  else {
    display.clearDisplay();
    executeDisplay(display, 1, 15, 8, "Waiting For Card");
    display.display();
    i = 30;
    while (i < 90) {
      if (rfReader.PICC_IsNewCardPresent() && rfReader.PICC_ReadCardSerial()) {
        cardDetect = true;
        break;
      }
      executeDisplay(display, 2, i+6, 20, "~");
      display.display();
      delay(400);
      i += 20;
    }
    executeDisplay(display, 2, 46, 20, "   ");
    display.display();
  }
}