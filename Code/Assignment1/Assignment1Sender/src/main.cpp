#include <Arduino.h>
#include <SoftwareSerial.h>

int ldrPin = A0;
int txPin = D4;
int rxPin = D3;

SoftwareSerial ledSerial(rxPin, txPin);

void setup() {
  pinMode(ldrPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(rxPin, INPUT);

  Serial.begin(9600);
  ledSerial.begin(9600);
}

void loop() {
  int ldrStatus = analogRead(ldrPin);
  int mappedLdrStatus = map(ldrStatus, 0, 1023, 0, 255);

  Serial.print("Value sent: ");
  Serial.println(mappedLdrStatus);
  ledSerial.print(mappedLdrStatus);
  ledSerial.print("\n");
}