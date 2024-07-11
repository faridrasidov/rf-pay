#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "WString.h"
#ifndef DEFINES_H
#define DEFINES_H

void executeDisplay(Adafruit_SSD1306 &display, float textSize, int cursorX, int cursorY, String message);
void doTone(int speakerPin,int freq, int duration);

void successTone(int speakerPin);
void pendingTone(int speakerPin);
void warningTone(int speakerPin);
void problemTone(int speakerPin);

String getHex(byte *buffer, byte bufferSize);

#endif // DEFINES_H