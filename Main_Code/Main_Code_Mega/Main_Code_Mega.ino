//////////////////////////////////////////////////////////////////////////////////
// Libraries
//////////////////////////////////////////////////////////////////////////////////
// BME280_Barometer_Sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
// DS18B20 Probe
#include <OneWire.h> 
#include <DallasTemperature.h>
// Transmitter
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//////////////////////////////////////////////////////////////////////////////////
// BME280_Barometer_Sensor
//////////////////////////////////////////////////////////////////////////////////
#define ALTITUDE 0
Adafruit_BME280 bme;
float temperature;
float humidity;
float pressure;

//////////////////////////////////////////////////////////////////////////////////
// Soil_Moisture_Sensor
//////////////////////////////////////////////////////////////////////////////////
#define SOIL_PIN A0
#define SOIL_POWER 6
int soilMoisture;

//////////////////////////////////////////////////////////////////////////////////
// DS18B20 Probe
//////////////////////////////////////////////////////////////////////////////////
#define PIN_TEMP_PROBE 2 
OneWire oneWire(PIN_TEMP_PROBE); 
DallasTemperature sensors(&oneWire);
float probeTemperature;

//////////////////////////////////////////////////////////////////////////////////
// Weather Meters
//////////////////////////////////////////////////////////////////////////////////
#define PIN_WINDVANE 5
#define PIN_ANEMOMETER 3
#define PIN_RAINGAUGE 18
#define NUMDIRS 8
unsigned long adc[NUMDIRS] = {26, 45, 77, 118, 161, 196, 230, 256};
char *strVals[NUMDIRS] = {"W","NW","N","SW","NE","S","SE","E"};
byte dirOffset = 0;
byte windDir;
int windSpeed1;
int windSpeed2;
int rainFall1;
int rainFall2;
volatile int numRevsAnemometer = 0;
volatile int numDropsRainGauge = 0;

//////////////////////////////////////////////////////////////////////////////////
// Transmitter
//////////////////////////////////////////////////////////////////////////////////
RF24 radio(7, 8); 
const byte address[6] = "00001";

#define READINGS_DELAY 600000 // xxx second delay on the readings. 5,000 = 5 secs, 300,000 = 5 mins, 720,000 = 12 mins

//////////////////////////////////////////////////////////////////////////////////

void setup() {
  while (!Serial);
    Serial.begin(9600);

  //////////////////////////////////////////////////////////////////////////////////
  // BME280_Barometer_Sensor
  //////////////////////////////////////////////////////////////////////////////////
  bool status;
  status = bme.begin(0x76);

  //////////////////////////////////////////////////////////////////////////////////
  // Soil_Moisture_Sensor
  //////////////////////////////////////////////////////////////////////////////////
  pinMode(SOIL_POWER, OUTPUT);
  digitalWrite(SOIL_POWER, LOW);

  //////////////////////////////////////////////////////////////////////////////////
  // DS18B20 Probe
  //////////////////////////////////////////////////////////////////////////////////
  sensors.begin(); 

  //////////////////////////////////////////////////////////////////////////////////
  // Weather Meters
  //////////////////////////////////////////////////////////////////////////////////
  pinMode(PIN_ANEMOMETER, INPUT);
  digitalWrite(PIN_ANEMOMETER, HIGH);
  digitalWrite(PIN_RAINGAUGE, HIGH);
  attachInterrupt(digitalPinToInterrupt(PIN_ANEMOMETER), countAnemometer, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_RAINGAUGE), countRainGauge, FALLING);

  //////////////////////////////////////////////////////////////////////////////////
  // Transmitter
  //////////////////////////////////////////////////////////////////////////////////
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();
}

//////////////////////////////////////////////////////////////////////////////////

void loop() {
  //////////////////////////////////////////////////////////////////////////////////
  // BME280_Barometer_Sensor
  //////////////////////////////////////////////////////////////////////////////////
  getTemperature();
  String temperatureString1 = "Temperature- ";
  String temperatureString2 = " -C";
  String temperatureValueString = String(temperature,2);
  String temperatureData = temperatureString1 + temperatureValueString + temperatureString2;
  Serial.println(temperatureData);
  char charArrayTemp[32];
  temperatureData.toCharArray(charArrayTemp, sizeof(charArrayTemp));

  getHumidity();
  String humidityString1 = "Humidity- ";
  String humidityString2 = " -%";
  String humidityValueString = String(humidity,2);
  String humidityData = humidityString1 + humidityValueString + humidityString2;
  Serial.println(humidityData);
  char charArrayHum[32];
  humidityData.toCharArray(charArrayHum, sizeof(charArrayHum)); 

  getPressure();
  String pressureString1 = "Pressure- ";
  String pressureString2 = " -hPa";
  String pressureValueString = String(pressure,2);
  String pressureData = pressureString1 + pressureValueString + pressureString2;
  Serial.println(pressureData);
  char charArrayPress[32];
  pressureData.toCharArray(charArrayPress, sizeof(charArrayPress));

  //////////////////////////////////////////////////////////////////////////////////
  // Soil_Moisture_Sensor
  //////////////////////////////////////////////////////////////////////////////////
  readSoil();
  String soilMoistureString1 = "Soil Moisture- ";
  String soilMoistureString2 = " -%";
  String soilMoistureValueString = String(soilMoisture);
  String soilMoistureData = soilMoistureString1 + soilMoistureValueString + soilMoistureString2;
  Serial.println(soilMoistureData);
  char charArraySoil[32];
  soilMoistureData.toCharArray(charArraySoil, sizeof(charArraySoil));

  //////////////////////////////////////////////////////////////////////////////////
  // DS18B20 Probe
  //////////////////////////////////////////////////////////////////////////////////
  getProbeTemperature();
  String TemperatureProbeString1 = "Probe Temperature- ";
  String TemperatureProbeString2 = " -C";
  String TemperatureProbeValueString = String(probeTemperature);
  String TemperatureProbeData = TemperatureProbeString1 + TemperatureProbeValueString + TemperatureProbeString2;
  Serial.println(TemperatureProbeData);
  char charArrayTempProbe[32];
  TemperatureProbeData.toCharArray(charArrayTempProbe, sizeof(charArrayTempProbe));

  //////////////////////////////////////////////////////////////////////////////////
  // Weather Meters
  //////////////////////////////////////////////////////////////////////////////////
  calcWindDir();
  String windDirString1 = "Wind Direction- ";
  String windDirValueString1 = String(strVals[windDir]);
  String windDirData = windDirString1 + windDirValueString1;
  Serial.println(windDirData);
  char charArrayWindDir[32];
  windDirData.toCharArray(charArrayWindDir, sizeof(charArrayWindDir));

  calcWindSpeed();
  String windSpeedString1 = "Wind Speed- ";
  String windSpeedString2 = ".";
  String windSpeedString3 = " -kph";
  String windSpeedValueString1 = String(windSpeed1);
  String windSpeedValueString2 = String(windSpeed2);
  String windSpeedData = windSpeedString1 + windSpeedValueString1 + windSpeedString2 + windSpeedValueString2 + windSpeedString3;
  Serial.println(windSpeedData);
  char charArrayWindSpeed[32];
  windSpeedData.toCharArray(charArrayWindSpeed, sizeof(charArrayWindSpeed));

  calcRainFall();
  String rainFallString1 = "Rain Fall- ";
  String rainFallString2 = ".";
  String rainFallString3 = " -mm";
  String rainFallValueString1 = String(rainFall1);
  String rainFallValueString2 = String(rainFall2);
  String rainFallData = rainFallString1 + rainFallValueString1 + rainFallString2 + rainFallValueString2 + rainFallString3;
  Serial.println(rainFallData);
  char charArrayRainFall[32];
  rainFallData.toCharArray(charArrayRainFall, sizeof(charArrayRainFall));

  //////////////////////////////////////////////////////////////////////////////////
  // Transmitter
  //////////////////////////////////////////////////////////////////////////////////
  // BME280_Barometer_Sensor
  radio.write(&charArrayTemp, temperatureData.length());
  radio.write(&charArrayHum, humidityData.length());
  radio.write(&charArrayPress, pressureData.length());

  // Soil_Moisture_Sensor
  radio.write(&charArraySoil, soilMoistureData.length());

  // DS18B20 Probe
  radio.write(&charArrayTempProbe, TemperatureProbeData.length());

  // Weather Meters
  radio.write(&charArrayWindDir, windDirData.length());
  radio.write(&charArrayWindSpeed, windSpeedData.length());
  radio.write(&charArrayRainFall, rainFallData.length());
  
  delay(READINGS_DELAY);
}

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// BME280_Barometer_Sensor
//////////////////////////////////////////////////////////////////////////////////
float getTemperature() {
  temperature = bme.readTemperature();
}
float getHumidity() {
  humidity = bme.readHumidity();
}
float getPressure() {
  pressure = bme.readPressure();
  pressure = bme.seaLevelForAltitude(ALTITUDE,pressure);
  pressure = pressure/100.0F;
}

//////////////////////////////////////////////////////////////////////////////////
// Soil_Moisture_Sensor
//////////////////////////////////////////////////////////////////////////////////
int readSoil() {
  digitalWrite(SOIL_POWER, HIGH);
  delay(10);
  soilMoisture = map(analogRead(SOIL_PIN),0,1023,0,100);
  digitalWrite(SOIL_POWER, LOW);
  return soilMoisture;
}

//////////////////////////////////////////////////////////////////////////////////
// DS18B20 Probe
//////////////////////////////////////////////////////////////////////////////////
float getProbeTemperature() {
  sensors.requestTemperatures();
  probeTemperature = sensors.getTempCByIndex(0);
}

//////////////////////////////////////////////////////////////////////////////////
// Weather Meters
//////////////////////////////////////////////////////////////////////////////////
void countAnemometer() {
   numRevsAnemometer++;
}
void countRainGauge() {
   numDropsRainGauge++;
}
void calcWindDir() {
   int val = analogRead(PIN_WINDVANE);
   val >>= 2;
   byte reading = val;
   for (windDir=0; windDir<NUMDIRS; windDir++) {
      if (adc[windDir] >= reading) {break;}
   }
   windDir = (windDir + dirOffset) % 8;
}
// 1.492 mph = 2.4 kph
void calcWindSpeed() {
   long speed = 24000;
   speed *= numRevsAnemometer;
   speed /= READINGS_DELAY;
   windSpeed1 = speed / 10;
   windSpeed2 = speed % 10;
   numRevsAnemometer = 0;
}
// 1 bucket = 0.2794 mm
void calcRainFall() {
   long vol = 2794;
   vol *= numDropsRainGauge;
   vol /= READINGS_DELAY;
   rainFall1 = vol / 10000;
   rainFall2 = vol % 10000;
   numDropsRainGauge = 0;
}
