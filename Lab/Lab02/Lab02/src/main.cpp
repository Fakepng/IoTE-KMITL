#include <Arduino.h>

int incomingByte = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.println(incomingByte);
  }
  
  digitalWrite

  delay(500);
}