#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "WString.h"
#ifndef DEFINES_H
#define DEFINES_H

void executeDisplay(Adafruit_SSD1306 &display, float textSize, int cursorX, int cursorY, String message);

String getHex(byte *buffer, byte bufferSize);

#endif // DEFINES_H