#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); 
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  delay(5000);
  String Data = "Test";
  char charArray[32];
  Data.toCharArray(charArray, sizeof(charArray));
  radio.write(&charArray, Data.length());
}
