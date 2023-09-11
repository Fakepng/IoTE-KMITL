#include <Arduino.h>
#include <ESP8266WiFi.h>

const char* SSID = "Fakepng-IoT";
const char* PASSWORD = "asdfghjkl";

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  String web = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  web += "<!DOCTYPE HTML>\r\n";
  web += "<html>\r\n";
  web += "<body>\r\n";
  web += "<h1>Hello World! I am Krit</h1>\r\n";
  web += "</body>\r\n";
  web += "</html>\r\n";

  client.print(web);
  delay(1);
  Serial.println("Client disconnected");
}