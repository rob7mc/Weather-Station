#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define ALTITUDE 0
Adafruit_BME280 bme;
float temperature;
float humidity;
float pressure;

void setup(void) {
  Serial.begin(9600);
  bool status;
  status = bme.begin(0x76);
}

void loop() {
  delay(5000);
  
  getTemperature();
  String temperatureString1 = "Temperature: ";
  String temperatureString2 = " C";
  String temperatureValueString = String(temperature,2);
  String temperatureData = temperatureString1 + temperatureValueString + temperatureString2;
  Serial.println(temperatureData);

  getHumidity();
  String humidityString1 = "Humidity: ";
  String humidityString2 = " %";
  String humidityValueString = String(humidity,2);
  String humidityData = humidityString1 + humidityValueString + humidityString2;
  Serial.println(humidityData);

  getPressure();
  String pressureString1 = "Pressure: ";
  String pressureString2 = " hPa";
  String pressureValueString = String(pressure,2);
  String pressureData = pressureString1 + pressureValueString + pressureString2;
  Serial.println(pressureData);
}

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
