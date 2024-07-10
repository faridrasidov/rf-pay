#include "DEFINES.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const String endpoint = "/chat";

WiFiClient client;

void setup() {
  Serial.begin(9600);
  connectToWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }
  Serial.flush();
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    int colonIndex = data.indexOf(':');
    if (colonIndex != -1) {
        String firstPart = data.substring(0, colonIndex);
        if (firstPart == "Dev1") {
          if (!client.connected()) {
            if (client.connect(SERVER_ADDRESS, SERVER_PORT)) {
              String fata = "GET " + endpoint + "?message="+ data + " HTTP/1.1\r\n" +
                            "Host: " + SERVER_ADDRESS + "\r\n" +
                            "Connection: close\r\n\r\n";
              client.println(fata);
              client.setTimeout(2500);
              for (int u = 0; u<7; u++){
              String response = client.readStringUntil('\n');
              }
              String response = client.readStringUntil('\n');
              // String response = client.readString();
              Serial.println(response);
              client.stop();
            }
          }
        }

        else if (firstPart == "FIX") {
          if (!client.connected()) {
            if (client.connect(SERVER_ADDRESS, SERVER_PORT)) {
              String fata = "GET " + endpoint + "?message="+ data + " HTTP/1.1\r\n" +
                            "Host: " + SERVER_ADDRESS + "\r\n" +
                            "Connection: close\r\n\r\n";
              client.println(fata);
              client.stop();
          }
        }
      }
    }
  }
  delay(100);
}

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
}