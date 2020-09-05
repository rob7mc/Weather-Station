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
}
 
void loop() {
  String Data = "Test" // Continously Writes "Test" into "test.txt"
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    myFile.println(Data);
    myFile.close();
  } 
  else {
    Serial.println("Can't open text file to write!");
  }
}
