#include "DEFINES.h"

const char* WIFI_SSID = "WIFI_SSID";
const char* WIFI_PASSWORD = "WIFI_PASSWORD";
const char* SERVER_ADDRESS = "SERVER_ADDRESS"; // domain also supported
const int SERVER_PORT = 51511;
const String SERVER_API = "/transfer";

void sendRequest(WiFiClientSecure &client, const String& data, bool notFix) {
  if (!client.connected() && client.connect(SERVER_ADDRESS, SERVER_PORT)) {
    String request = "GET " + SERVER_API + "?message=" + data + " HTTP/1.1\r\n" +
                     "Host: " + SERVER_ADDRESS + "\r\n" +
                     "Connection: close\r\n\r\n";
    client.println(request);

    if (notFix) {
      client.setTimeout(2500);
      while (client.connected() || client.available()) {
        String response = client.readStringUntil('\n');
        if (response.indexOf("rfPayRes") >= 0) {
          response = client.readStringUntil('\n');
          Serial.println(response);
          break;
        }
      }
    }

    client.stop();
  }
}

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
}