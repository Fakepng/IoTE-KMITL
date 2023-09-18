#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <PubSubClient.h>

#define DHT_TYPE DHT11

const int DHT_PIN = D1;
const int LED_PIN = D2;
const int LDR_PIN = A0;
const int MQTT_PORT = 1883;

const char* SSID = "Fakepng-IoTE";
const char* PASSWORD = "iotengineering";
const char* MQTT_BROKER = "broker.netpie.io";
const char* MQTT_CLIENT = "---Client ID---";
const char* MQTT_TOKEN = "---Token---";
const char* MQTT_SECRET = "---Secret---";

DHT_Unified dht(DHT_PIN, DHT_TYPE);

WiFiClient espClient;
PubSubClient mqttClient(espClient);

long lastMsg = 0;
int value = 0;
char msg[100];
String DataString;

float temperature;
float humidity;
int light;
int ledStatus;

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (mqttClient.connect(MQTT_CLIENT, MQTT_TOKEN, MQTT_SECRET)) {
      Serial.println("connected");
      mqttClient.subscribe("@msg/operator");
    
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String message;
  for (int i = 0; i < length; i++) {
    message = message + char(payload[i]);
  }

  Serial.println(message);
  if (String(topic) == "@msg/operator") {
    if (message == "ON") {
      digitalWrite(LED_PIN, HIGH);
      ledStatus = HIGH;
      Serial.println("LED ON");
    
    } else if (message == "OFF") {
      digitalWrite(LED_PIN, LOW);
      ledStatus = LOW;
      Serial.println("LED OFF");

    } 
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();

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
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(callback);
  mqttClient.subscribe("@msg/operator");
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }

  mqttClient.loop();
  long now = millis();

  if (now - lastMsg > 5000) {
    lastMsg = now;
    ++value;

    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println("Error reading temperature!");
    
    } else {
      temperature = event.temperature;
      humidity = event.relative_humidity;
      light = analogRead(LDR_PIN);
      int lightPercentage = map(light, 0, 1023, 0, 100);

      DataString = "{\"data\":{\"temperature\":" + (String)temperature + ",\"humidity\":" + (String)humidity + ",\"light\":" + (String)lightPercentage + ",\"led\":" + (String)ledStatus +"}}";
      
      DataString.toCharArray(msg, 100);
      Serial.println("Hello NETPIE2020");
      Serial.print("Publish message: ");
      Serial.println(msg);

      mqttClient.publish("@shadow/data/update", msg);
    }
  }

  delay(1);
}