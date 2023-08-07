#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 4
#define DHTTYPE DHT11

const int led1Pin = D5;
const int led2Pin = D7;

const int ldrPin = A0;

int temp = 0;
int dhtDelay = 2000;
long previousMillis = 0;

DHT_Unified dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(ldrPin, INPUT);
}

void loop() {
  // sensors_event_t event;
  // dht.temperature().getEvent(&event);
  int light = analogRead(ldrPin);

  if (millis() - previousMillis >= dhtDelay) {
    previousMillis = millis();
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {} else {
      temp = event.temperature;
    }
  }

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" C, ");
  Serial.print("Light: ");
  Serial.print(light);
  Serial.println("\n");

  if (light >= 1000) {
    digitalWrite(led1Pin, HIGH);
    if (temp >= 35) {
      digitalWrite(led2Pin, HIGH);
    } else {
      digitalWrite(led2Pin, LOW);
    }
  } else {
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
  }
}