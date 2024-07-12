#include "DEFINES.h"

WiFiClient* client;
WiFiClientSecure* secureClient;

void setup() {
  Serial.begin(9600);
  connectToWiFi();
  
  bool found = false;
  for (int i = 0; i < HTTPS_PORTS; i++) {
    if (HTTPS_PORT[i] == SERVER_PORT) {
      found = true;
      break;
    }
  }

  if (found) {
      secureClient = new WiFiClientSecure();
      secureClient->setInsecure();
      client = secureClient;
  } else {
      client = new WiFiClient();
  }
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
        sendRequest(*client, data, firstPart == "Dev1");
      }
    }
  }

  delay(100);
}