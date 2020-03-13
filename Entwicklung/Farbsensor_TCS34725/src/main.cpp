//https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf
#include <Arduino.h>
#include <FarbsensorTCS34725.h>
/*

AEN, ect.


Part number identification
0x44 = TCS34721 and TCS34725
0x4D = TCS34723 and TCS34727

SLEEP --> I2C Start --> IDLE --> Power Managment Feture enabled && RGBC Feture enabled? --> SLEEP - delay(x) --> RGBC Init --> RGBC ADC --> IDLE --> !PON?? --> SLEEP
                                                                                                                                                 --> PON?? --> IDLE                    
                             --> !Power Managment Feture enabled && RGBC Feture enabled? --> RGBC Init --> RGBC ADC --> IDLE --> !PON?? --> SLEEP
                                                                                                                             --> PON?? --> IDLE  
*/
FarbsensorTCS34725 sensor;

void setup() {
  Serial.begin(9600);
  sensor = FarbsensorTCS34725(0x29, 13, GAIN_4x, INTEG_CYCLE_10);
  while(!Serial);
}

void loop()
{
  Serial.print("Farbe: ");
  sensor.farbeErkennen();
  delay(1000);
}
