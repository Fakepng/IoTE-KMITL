#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <PubSubClient.h>

#define DHT_TYPE DHT11
const int DHT_PIN = D4;

const int LED_PIN = D0;
const int LDR_PIN = A0;

const char* WIFI_SSID = "Fakepng-IoT";
const char* WIFI_PASSWORD = "iotengineering";

const char* MQTT_BROKER = "broker.hivemq.com";
const char* MQTT_CLIENT = "clientId-fakepngeyttenyrt";
const int MQTT_PORT = 1883;

const int PUBLISH_INTERVAL = 5000;
const int DHT_INTERVAL = 2000;
const int LDR_INTERVAL = 500;

const int BAUD_RATE = 9600;

float temperature;
float humidity;
int ldr;

int ledState = LOW;

long lastLDR = 0;
long lastPublish = 0;
long lastDHT = 0;

DHT_Unified dht(DHT_PIN, DHT_TYPE);

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void connectWifi();
void connectMQTT();
void mqttCallback(char* topic, byte* payload, unsigned int length);

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  digitalWrite(LED_PIN, ledState);
  dht.begin();

  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  mqttClient.subscribe("fakepng/led");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }

  if (!mqttClient.connected()) {
    connectMQTT();
  }

  mqttClient.loop();

  long now = millis();

  if (now - lastLDR > LDR_INTERVAL) {
    lastLDR = now;

    ldr = analogRead(LDR_PIN);
    Serial.print("LDR: ");
    Serial.println(ldr);
  }

  if (now - lastDHT > DHT_INTERVAL) {
    lastDHT = now;

    sensors_event_t dhtSensor;

    dht.temperature().getEvent(&dhtSensor);
    if (isnan(dhtSensor.temperature)) {
      Serial.println("Error reading temperature!");
    } else {
      temperature = dhtSensor.temperature;
    }

    dht.humidity().getEvent(&dhtSensor);
    if (isnan(dhtSensor.relative_humidity)) {
      Serial.println("Error reading humidity!");
    } else {
      humidity = dhtSensor.relative_humidity;
    }

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }
  
  if (now - lastPublish > PUBLISH_INTERVAL) {
    lastPublish = now;

    String payload = "{\"temp\": " + String(temperature) + ", \"humid\": " + String(humidity) + ", \"ldr\": " + String(ldr) + "}";
    char msg[100];
    payload.toCharArray(msg, 100);

    Serial.print("Publish message: ");
    Serial.println(msg);
    mqttClient.publish("fakepng/dht", msg);
  }
}

void connectWifi() {
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (mqttClient.connect(MQTT_CLIENT)) {
      Serial.println("MQTT connected");
      mqttClient.subscribe("fakepng/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String message;
  for (int i = 0; i < length; i++) {
    message = message + char(payload[i]);
  }

  Serial.println(message);
  if (String(topic) == "fakepng/led") {
    if (message == "ON") {
      digitalWrite(LED_PIN, HIGH);
      ledState = HIGH;
      Serial.println("LED ON");
    
    } else if (message == "OFF") {
      digitalWrite(LED_PIN, LOW);
      ledState = LOW;
      Serial.println("LED OFF");

    } 
  }
}