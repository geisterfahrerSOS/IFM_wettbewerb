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
  sensor = FarbsensorTCS34725(MULTIPLEXER, 6, 13, GAIN_4x, INTEG_CYCLE_10);
  while(!Serial);
}

void loop()
{
  Serial.print("Farbe: ");
  sensor.farbeErkennen();
  delay(1000);
}
