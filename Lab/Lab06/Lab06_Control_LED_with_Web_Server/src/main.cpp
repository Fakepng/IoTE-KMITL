#include <Arduino.h>
#include <ESP8266WiFi.h>

const int LED_PIN = D2;
const char* SSID = "Fakepng-IoT";
const char* PASSWORD = "asdfghjkl";

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println();
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
  
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  
  if (!client) {
    return;
  }

  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  int val;
  if (req.indexOf("/gpio/0") != -1) {
    val = 0;
  } else if (req.indexOf("/gpio/1") != -1) {
    val = 1;
  } else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  digitalWrite(LED_PIN, val);
  client.flush();

  String web = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  web += "<!DOCTYPE HTML>\r\n";
  web += "<html>\r\n";
  web += "<body>\r\n";
  web += "<h1>LED Status</h1>\r\n";
  web += "<p>\r\n";
  if(val==1){
    web += "LED On\r\n";
  }else{
    web += "LED Off\r\n";
  }
  web += "</p>\r\n";
  web += "</body>\r\n";
  web += "</html>\r\n";

  client.print(web);
  delay(1);
  Serial.println("Client disonnected");
}