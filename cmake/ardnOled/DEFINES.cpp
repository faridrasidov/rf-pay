#include "WString.h"
#include "DEFINES.h"

void executeDisplay(Adafruit_SSD1306 &display, float textSize, int cursorX, int cursorY, String message) {
    display.setTextSize(textSize);
    display.setCursor(cursorX, cursorY);
    display.print(message);
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