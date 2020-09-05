//////////////////////////////////////////////////////////////////////////////////
// Libraries
//////////////////////////////////////////////////////////////////////////////////
// Receiver
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// SD Card
#include <SD.h>
// Time
#include "wiring.c"

// Receiver
#define CE_PIN  7
#define CSN_PIN 8
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

// SD Card
#define CS_PIN 10
File myFile;

// Time
const int Adjust = 20; // 20 seconds added on to time to allow time between upload and next reading

// The delimeter for breaking up data in Excel is -

//////////////////////////////////////////////////////////////////////////////////

void setup() {
  SetMillis();
  while (!Serial);
    Serial.begin(9600);

  // Receiver
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();

  // SD Card
  pinMode(CS_PIN, OUTPUT);
  if (!SD.begin(CS_PIN)) {
    Serial.println("Can't recognise SD Card!");
    return;
  }
}

//////////////////////////////////////////////////////////////////////////////////

void loop() {
  // Receiver
  if (radio.available()) {
    while (radio.available()) {
      char Data[32] = {0};
      radio.read(&Data, sizeof(Data));
      Serial.println(Data);

      // SD Card
      myFile = SD.open("test.txt", FILE_WRITE);
      if (myFile) {
        unsigned long Now = millis()/1000;
        int Seconds = Now%60;
        int Minutes = (Now/60)%60;
        int Hours = (Now/3600)%24;
        String HoursString = String(Print2(Hours)); 
        String ColonString = ":"; 
        String MinutesString = String(Print2(Minutes));  
        String SecondsString = String(Print2(Seconds)); 
        String DashString = " - ";
        String TimeString = HoursString + ColonString + MinutesString + ColonString + SecondsString + DashString;
        myFile.println(TimeString + Data);
        myFile.close();
      } 
      else {
        Serial.println("Can't open text file to write!");
      }
    }
  }
}

void SetMillis () {
  char T[] = __TIME__;
  uint8_t oldSREG = SREG;
  cli();
  unsigned long temp = (unsigned long)((T[0]*10+T[1]-528)*60+T[3]*10+T[4]-528)*60+T[6]*10+T[7]-528+Adjust;
  timer0_millis = (unsigned long)temp*1000;
  SREG = oldSREG;
}

int Print2(int n) { // Adds a leading 0 if it's a single digit number
  if (n<10) {
    String nS1 = String(n);
    String nS2 = "0" + nS1;
    int n = nS2.toInt();
    return n;
  }
  return n;
}
