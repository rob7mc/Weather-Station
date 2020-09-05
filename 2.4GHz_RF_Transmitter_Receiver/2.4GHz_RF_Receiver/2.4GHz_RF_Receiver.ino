#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN  7
#define CSN_PIN 8
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    while (radio.available()) {
      char Data[32] = {0};
      radio.read(&Data, sizeof(Data));
      Serial.println(Data);
    }
}
