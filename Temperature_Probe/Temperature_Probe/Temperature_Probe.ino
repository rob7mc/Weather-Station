#include <OneWire.h> 
#include <DallasTemperature.h>

#define PIN_TEMP_PROBE 2 
OneWire oneWire(PIN_TEMP_PROBE); 
DallasTemperature sensors(&oneWire);
float probeTemperature;

void setup(void) { 
 Serial.begin(9600);
 sensors.begin(); 
} 

void loop(void) { 
  delay(5000);
  getProbeTemperature();
  String TemperatureProbeString1 = "Probe Temperature: ";
  String TemperatureProbeString2 = " C";
  String TemperatureProbeValueString = String(probeTemperature);
  String TemperatureProbeData = TemperatureProbeString1 + TemperatureProbeValueString + TemperatureProbeString2;
  Serial.println(TemperatureProbeData);
}

float getProbeTemperature() {
  sensors.requestTemperatures();
  probeTemperature = sensors.getTempCByIndex(0);
}
