#include <Arduino.h>
#include <Servo.h>

const int potPin = A0;
const int servoPin = D2;

Servo servo;

void setup() {
  pinMode(potPin, INPUT);
  servo.attach(servoPin, 500, 2400);
}

void loop() {
  int potValue = analogRead(potPin);
  int servoValue = map(potValue, 0, 1023, 500, 2400);
  servo.writeMicroseconds(servoValue);
}