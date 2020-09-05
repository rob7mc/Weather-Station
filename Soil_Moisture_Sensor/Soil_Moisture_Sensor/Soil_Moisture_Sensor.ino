#define SOIL_PIN A0
#define SOIL_POWER 6
int soilMoisture; 

void setup() {
  Serial.begin(9600);
  pinMode(SOIL_POWER, OUTPUT);
  digitalWrite(SOIL_POWER, LOW);
}

void loop() {
  delay(5000);
  readSoil();
  String soilMoistureString1 = "Soil Moisture: ";
  String soilMoistureString2 = " %";
  String soilMoistureValueString = String(soilMoisture);
  String soilMoistureData = soilMoistureString1 + soilMoistureValueString + soilMoistureString2;
  Serial.println(soilMoistureData);
}

int readSoil() {
  digitalWrite(SOIL_POWER, HIGH);
  delay(10);
  soilMoisture = map(analogRead(SOIL_PIN),0,1023,0,100);
  digitalWrite(SOIL_POWER, LOW);
  return soilMoisture;
}
