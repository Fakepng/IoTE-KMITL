#include <Arduino.h>
#include <SoftwareSerial.h>

int ledPin = D0;
int txPin = D4;
int rxPin = D3;

SoftwareSerial ldrSerial(rxPin, txPin);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(txPin, OUTPUT);
  pinMode(rxPin, INPUT);

  Serial.begin(9600);
  ldrSerial.begin(9600);
}

void loop() {
  int ldrStatus = ldrSerial.parseInt();
  int mappedLdrStatus = map(ldrStatus, 0, 255, 0, 1023);

  Serial.print("Value received: ");
  Serial.println(mappedLdrStatus);

  if (mappedLdrStatus > 600) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}