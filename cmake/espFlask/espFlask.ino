#include "DEFINES.h"

WiFiClientSecure client;

void setup() {
  Serial.begin(9600);
  connectToWiFi();
  client.setInsecure();
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
      if (firstPart == "Dev1" || firstPart == "FIX") {
        sendRequest(client, data, firstPart == "Dev1");
      }
    }
  }

  delay(100);
}