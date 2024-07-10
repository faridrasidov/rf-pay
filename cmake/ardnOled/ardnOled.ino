#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MFRC522.h>

#define RELAY1_PIN 7
#define SS_PIN 10
#define RST_PIN 9
#define RX_PIN 5
#define TX_PIN 6
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

MFRC522 mfrc522(SS_PIN, RST_PIN);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

SoftwareSerial espSerial(RX_PIN, TX_PIN);
bool cardDetect = false;
int speakerPin = 8;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(RELAY1_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, HIGH);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(30, 5);
  display.println("RF-Pay");

  display.setTextSize(1);
  display.setCursor(20, 20);
  display.println("Device by Farid");

  display.display();
  Serial.begin(9600);
  espSerial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  delay(2000);

}

void loop() {
  int i = 0;
  byte nuidPICC[4];
  byte buffer[18];
  byte size = sizeof(buffer);
  String incomingData;
  String firstData;
  String secondData;
  String MachName = "Dev1";
  if (cardDetect) {
    display.clearDisplay();
    display.setTextSize(1.5);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(34, 0);
    display.print("Card Found");
    display.display();
    espSerial.readString();
    display.setCursor(50, 8);
    display.print("Wait");
    display.display();
    String CardId = getHex(mfrc522.uid.uidByte, mfrc522.uid.size);
    espSerial.print(MachName);
    espSerial.print(":");
    espSerial.print(CardId);
    delay(3500);
    incomingData = espSerial.readStringUntil('.');
    int cloon = incomingData.indexOf(':');
    firstData = incomingData.substring(0, cloon);
    secondData = incomingData.substring(cloon + 1);

    if (firstData == "200") {
      display.clearDisplay();
      display.setTextSize(1.5);
      display.setCursor(40, 0);
      display.print("Success");
      display.setCursor(20, 16);
      display.print("Money : ");
      display.print(secondData);
      display.display();
      digitalWrite(RELAY1_PIN, LOW);
      tone(speakerPin, 1450);
      delay(200);
      noTone(speakerPin);
      delay(800);
      digitalWrite(RELAY1_PIN, HIGH);
      delay(1000);
      display.clearDisplay();
      // espSerial.flush();
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }

    else if (firstData == "400") {
      display.clearDisplay();
      display.setTextSize(1.5);
      display.setCursor(25, 0);
      display.print("Balance Error");
      display.setCursor(20, 16);
      display.print("Money : ");
      display.print(secondData);
      display.display();
      tone(speakerPin, 450);
      delay(200);
      noTone(speakerPin);
      delay(200);
      tone(speakerPin, 450);
      delay(200);
      noTone(speakerPin);
      delay(1400);
      display.clearDisplay();
      // espSerial.flush();
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }

    else if (firstData == "800") {
      display.clearDisplay();
      display.setTextSize(1.5);
      display.setCursor(20, 0);
      display.print("Not Registered");
      display.display();
      tone(speakerPin, 450);
      delay(200);
      noTone(speakerPin);
      delay(200);
      tone(speakerPin, 450);
      delay(200);
      noTone(speakerPin);
      delay(1400);
      display.clearDisplay();
      // espSerial.flush();
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }

    else {
      espSerial.print("FIX:");
      espSerial.print(MachName);
      espSerial.print("/");
      espSerial.print(CardId);
      display.clearDisplay();
      display.setTextSize(1.5);
      display.setCursor(15, 8);
      // display.print("Server's Down Or");
      display.print(firstData);
      display.setCursor(15, 16);
      // display.print("Device Not Conn");
      display.print(secondData);
      display.display();
      tone(speakerPin, 650);
      delay(1000);
      noTone(speakerPin);
      delay(1000);
      display.clearDisplay();
      // espSerial.flush();
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }
    cardDetect = false;
  }

  else {
      display.clearDisplay();
      display.setTextSize(1.5);
      display.setCursor(15, 8);
      display.print("Waiting For Card");
      display.display();
      i = 30;
      while (i < 90) {
        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
          cardDetect = true;
          break;
        }
        display.setTextSize(2);
        display.setCursor(i + 6, 20);
        display.println("~");
        display.display();
        delay(400);
        i += 20;
      }
      display.setCursor(46, 20);
      display.print("   ");
      display.display();
    }
}

String getHex(byte *buffer, byte bufferSize) {
  String hexString = "";
  for (byte i = 0; i < bufferSize; i++) {
    hexString += String(buffer[i] < 0x10 ? "0" : "");
    hexString += String(buffer[i], HEX);
  }
  hexString.toUpperCase();
  hexString += '\n';
  return hexString;
}