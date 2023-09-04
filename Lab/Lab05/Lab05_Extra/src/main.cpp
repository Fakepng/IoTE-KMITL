#include <Arduino.h>
#include <Servo.h>

const int buttonPin = D1;
const int servoPin = D5;
const int longPressTime = 3000;

int lastButtonState = HIGH;
int currentButtonState;
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;

Servo servo;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(servoPin, OUTPUT);

  servo.attach(servoPin, 500, 2400);
}

void loop() {
  currentButtonState = digitalRead(buttonPin);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    pressedTime = millis();
  } else if (lastButtonState == LOW && currentButtonState == HIGH) {
    releasedTime = millis();

    long pressDuration = releasedTime - pressedTime;

    if (pressDuration < longPressTime) {
      servo.write(0);
    } else {
      servo.write(180);
    }
  }

  lastButtonState = currentButtonState;
  
}