#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHT_TYPE DHT11

const int DHT_PIN = D1;
const char* SSID = "Fakepng-IoT";
const char* PASSWORD = "asdfghjkl";

WiFiServer server(80);

DHT_Unified dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);
  delay(10);

  dht.begin();

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

  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client");

    boolean blank_line = true;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (c == '\n' && blank_line) {
          sensors_event_t event;
          dht.temperature().getEvent(&event);

          float h, t, f;
          if (isnan(event.temperature)) {} else {
            h = event.relative_humidity;
            t = event.temperature;
            f = t * 9 / 5 + 32;

            Serial.print("Humidity: ");
            Serial.print(h);
            Serial.print(" %\t");
            Serial.print("Temperature: ");
            Serial.print(t);
            Serial.print(" *C ");
            Serial.print(f);
            Serial.println(" *F");
          }

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();

          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<meta charset=\"utf-8\">");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
          client.println("<title>ESP8266 Weather Station</title>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h1>ESP8266 Weather Station</h1>");
          client.println("<p>Temperature: " + String(t) + " *C " + String(f) + " *F</p>");
          client.println("<p>Humidity: " + String(h) + " %</p>");
          client.println("</body>");
          client.println("</html>");
          break;
        }

        if (c == '\n') {
          blank_line = true;
        } else if (c != '\r') {
          blank_line = false;
        }
      }
    }

    delay(1);
    client.stop();
    Serial.println("Client disconnected");
  } 
}