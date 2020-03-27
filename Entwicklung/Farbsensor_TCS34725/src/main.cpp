//https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf
#include <Arduino.h>
#include <FarbsensorTCS34725.h>
#define MULTIPLEXER 0x70
/*
FARBEN: ORANGE, GELB, SCHWARZ
*/

FarbsensorTCS34725 sensor;

void setup() {
  Serial.begin(9600);
  
  FS_Param_multiAdd_t parameter;
  parameter.mulitAdd = 0x70;
  parameter.channel = 6;
  parameter.aLed = 13;
  parameter.aAGain = GAIN_4x;
  parameter.aIntegCycle = INTEG_CYCLE_10;

  sensor = FarbsensorTCS34725(parameter);
  while(!Serial);
}

void loop()
{
  Serial.print("Farbe: ");
  sensor.farbeErkennen();
  delay(1000);
}
