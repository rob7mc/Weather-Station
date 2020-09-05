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

#define READINGS_DELAY 600000 // xxx second delay on the readings. 5,000 = 5 secs, 300,000 = 5 mins, 720,000 = 12 mins

void setup() {
  Serial.begin(9600);
  pinMode(PIN_ANEMOMETER, INPUT);
  digitalWrite(PIN_ANEMOMETER, HIGH);
  digitalWrite(PIN_RAINGAUGE, HIGH);
  attachInterrupt(digitalPinToInterrupt(PIN_ANEMOMETER), countAnemometer, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_RAINGAUGE), countRainGauge, FALLING);
}

void loop() {
  calcWindDir();
  String windDirString1 = "Wind Direction: ";
  String windDirValueString1 = String(strVals[windDir]);
  String windDirData = windDirString1 + windDirValueString1;
  Serial.println(windDirData);

  calcWindSpeed();
  String windSpeedString1 = "Wind Speed: ";
  String windSpeedString2 = ".";
  String windSpeedString3 = " kph";
  String windSpeedValueString1 = String(windSpeed1);
  String windSpeedValueString2 = String(windSpeed2);
  String windSpeedData = windSpeedString1 + windSpeedValueString1 + windSpeedString2 + windSpeedValueString2 + windSpeedString3;
  Serial.println(windSpeedData);

  calcRainFall();
  String rainFallString1 = "Rain Fall: ";
  String rainFallString2 = ".";
  String rainFallString3 = " mm";
  String rainFallValueString1 = String(rainFall1);
  String rainFallValueString2 = String(rainFall2);
  String rainFallData = rainFallString1 + rainFallValueString1 + rainFallString2 + rainFallValueString2 + rainFallString3;
  Serial.println(rainFallData);

  delay(READINGS_DELAY);
}

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
