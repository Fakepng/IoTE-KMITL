#include <Arduino.h>
#include <ESP8266WiFi.h>

const int SERVER_PORT = 8000;
const char* SSID = "Fakepng-IoT";
const char* PASSWORD = "asdfghjkl";

WiFiServer server(SERVER_PORT);

void setup() {
  Serial.begin(9600);
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(SSID);
  
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");

    while (1) {
      while (client.available()) {
        String data = client.readString();
        Serial.println(data);
      }

      if (server.hasClient()) {
        return;
      }
    }
  }
}