#include <Arduino.h>

int ledPin = D1;
int ldrPin = A0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(ldrPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  int ldrValue = analogRead(ldrPin);
  Serial.println(ldrValue);

  int ledValue = map(ldrValue, 600, 1023, 0, 255);
  analogWrite(ledPin, ledValue);
}