#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

#ifndef DEFINES_H
#define DEFINES_H

extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;
extern const char* SERVER_ADDRESS;
extern const int SERVER_PORT;
extern const String SERVER_API;

void sendRequest(WiFiClientSecure &client, const String& data, bool notFix);
void connectToWiFi();

#endif