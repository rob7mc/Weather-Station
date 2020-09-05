#include <SD.h>

#define CS_PIN 10
File myFile;
 
void setup() {
  Serial.begin(9600);
  pinMode(CS_PIN, OUTPUT);
  if (!SD.begin(CS_PIN)) {
    Serial.println("Can't recognise SD Card!");
    return;
  }
 
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt");
 
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } 
  else {
    Serial.println("Can't open text file to read!");
  }
}
 
void loop() {}
