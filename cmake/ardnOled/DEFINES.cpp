#include "WString.h"
#include "DEFINES.h"

void executeDisplay(Adafruit_SSD1306 &display, float textSize, int cursorX, int cursorY, String message) {
    display.setTextSize(textSize);
    display.setCursor(cursorX, cursorY);
    display.print(message);
}

void doTone(int speakerPin, int freq, int duration){
  tone(speakerPin, freq);
  delay(duration);
}

void successTone(int speakerPin){
  doTone(speakerPin, 900, 100);
  doTone(speakerPin, 1100, 100);
  doTone(speakerPin, 1300, 100);
  doTone(speakerPin, 1500, 200);
  doTone(speakerPin, 1300, 100);
  doTone(speakerPin, 1500, 500);
  noTone(speakerPin);
}

void pendingTone(int speakerPin){
  doTone(speakerPin, 1000, 300);
  noTone(speakerPin);
}

void warningTone(int speakerPin) {
  doTone(speakerPin, 450, 200);
  noTone(speakerPin);
  delay(200);
  doTone(speakerPin, 450, 200);
  noTone(speakerPin);
  delay(1400);
}

void problemTone(int speakerPin) {
   doTone(speakerPin, 650, 1000);
   noTone(speakerPin);
   delay(1000);
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