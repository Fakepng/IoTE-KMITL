#include <Arduino.h>

int ledPins[] = {D1, D2, D3, D4, D5};
int potPin = A0;

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(potPin, INPUT);
}

void loop() {
  int potValue = analogRead(potPin);

  for (int i = 0; i < 5; i++) {
    analogWrite(ledPins[(i) % 5], 0);
    analogWrite(ledPins[(i+1) % 5], 1);
    analogWrite(ledPins[(i+2) % 5], 10);
    analogWrite(ledPins[(i+3) % 5], 85);
    analogWrite(ledPins[(i+4) % 5], 255);
    delay(potValue);
  }
}