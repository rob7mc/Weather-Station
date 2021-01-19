# Weather-Station

This project makes use of Arduino Microcontrollers (UNO & Mega) along with its IDE. The sensors used include a BME280 barometric pressure, temperature and humidity sensor, a soil moisture sensor, a DS18B20 temperature probe sensor and anemometer, rain gauge and wind vane sensors which make use of reed switches. The Arduino Mega and sensors were powered using a 12 Volt battery. The radio frequency technology used to send data between the Arduino Mega at the weather station to the Arduino UNO located elsewhere was the NRF24L01 + PA/LNA module. The collected data from the weather station was stored on a Micro SD card using the Adafruit Micro SD Breakout Board.

The station is low-cost, allows for the wireless transmission of data obtained over a range of 800 metres, has a low power consumption, has a highly modifiable build and is accurate when measuring environmental data.

The motivation for the project was due to the fact that traditional and research grade weather stations are very expensive. It was also noticed that similar previous implementations could be improved upon.

## When running the code:

The IDE version used was 1.8.10. Any required libraries can be found in the Libraries folder. The main code used on the Arduino Mega and UNO can be found in the Main_Code folder. Every other sensors code can be found in their respective folders when testing individual components on either the Arduino Mega or UNO. Note that some pin values may need to be changed when going between the Mega and UNO.
