#include <Arduino.h>

int ledPin1 = D1;
int ledPin2 = D2;
int ledPin3 = D3;

int statePin = D8;

int ledState1 = LOW;
int ledState2 = LOW;
int ledState3 = LOW;

int problemNo = 1;

unsigned long perviousMillis = 0;

const long interval = 100;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(statePin, INPUT);
}

void loop() {
  if (digitalRead(statePin) == HIGH) {
    problemNo = 2;
  } else {
    problemNo = 1;
  }

  if (problemNo == 1) {
    unsigned long currentMillis = millis();

    if (currentMillis - perviousMillis >= interval) {
      perviousMillis = currentMillis;

      if (ledState1 == LOW) {
        ledState1 = HIGH;
        ledState2 = HIGH;
        ledState3 = HIGH;
      } else {
        ledState1 = LOW;
        ledState2 = LOW;
        ledState3 = LOW;
      }

      digitalWrite(ledPin1, ledState1);
      digitalWrite(ledPin2, ledState2);
      digitalWrite(ledPin3, ledState3);
    }

  } else if (problemNo == 2) {
    unsigned long currentMillis = millis();

    if (currentMillis / interval % 3 == 0) {
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
    } else if (currentMillis / interval % 3 == 1) {
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, LOW);
    } else if (currentMillis / interval % 3 == 2) {
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, HIGH);
    }
  }
  
}