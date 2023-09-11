#include <Arduino.h>
#include <ESP8266WiFi.h>

const int SERVER_PORT = 8000;
const char* SSID = "Fakepng-IoT";
const char* PASSWORD = "asdfghjkl";

IPAddress serverIP = {10,10,0,245};

WiFiServer server(SERVER_PORT);
WiFiClient client;

void setup() {
  Serial.begin(9600);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Connect to TCP Server");
  while (!client.connect(serverIP, SERVER_PORT)) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("");
  Serial.println("Connected to TCP Server");
}

void loop() {
  while(client.available()) {
    uint8_t data = client.read();
    Serial.write(data);
  }

  client.println("Hello from ESP8266");
  delay(1000);
}