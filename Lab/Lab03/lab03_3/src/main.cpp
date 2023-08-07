#include <Arduino.h>

const int buttonPin = D1;
const int led1Pin = D5;
const int led2Pin = D6;
const int led3Pin = D7;

int enable = 0;
int mode = 1;

long lastDebounceTime = 0;
long debounceDelay = 100;

void IRAM_ATTR ISR() {
  if ( (millis() - lastDebounceTime) > debounceDelay && !enable) {
    enable = 1;
    if (mode == 0) {
      mode = 1;
    } else if (mode == 1) {
      mode = 2;
    } else {
      mode = 0;
    }

    Serial.println(mode);

    lastDebounceTime = millis();
  }
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);

  attachInterrupt(buttonPin, ISR, FALLING);

  Serial.begin(9600);
}

void loop() {
  if (enable == 1) {

    if (mode == 0) {
      digitalWrite(led1Pin, HIGH);
      digitalWrite(led2Pin, LOW);
      digitalWrite(led3Pin, LOW);

      delay(1000);

      digitalWrite(led1Pin, LOW);
      digitalWrite(led2Pin, LOW);
      digitalWrite(led3Pin, LOW);

    } else if (mode == 1) {
      digitalWrite(led1Pin, LOW);
      digitalWrite(led2Pin, HIGH);
      digitalWrite(led3Pin, LOW);

      delay(1000);

      digitalWrite(led1Pin, LOW);
      digitalWrite(led2Pin, LOW);
      digitalWrite(led3Pin, LOW);
    } else if (mode == 2) {
      digitalWrite(led1Pin, LOW);
      digitalWrite(led2Pin, LOW);
      digitalWrite(led3Pin, HIGH);

      delay(1000);

      digitalWrite(led1Pin, LOW);
      digitalWrite(led2Pin, LOW);
      digitalWrite(led3Pin, LOW);
    } 

    enable = 0;
  }
}