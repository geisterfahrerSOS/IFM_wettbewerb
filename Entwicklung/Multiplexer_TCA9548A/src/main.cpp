#include <Arduino.h>
#include <I2C.h>
#include <FarbsensorTCS34725.h>
//https://cdn-reichelt.de/documents/datenblatt/A300/ADAFRUIT_2717_ENG_TDS.pdf
//https://cdn-shop.adafruit.com/datasheets/tca9548a.pdf
/*
ADRESSE: 0x70 (0x70 - 0x77)
Vin - 5V
SCL / SDA -> Master
RST -> reset --> conntect to ground

A0-A2 --> Address of multiplexer
plug in
A0 --> if High address will increase by 1
A1 --> if High address will increase by 2
A2 --> if High address will increase by 4

SDx /SCx --> Slave
no Pullup

*/



#define MultiAdd 0x70
FarbsensorTCS34725 f1;
FarbsensorTCS34725 f2;
I2C i2c;
void setup() {
  Serial.begin(9600);
  Serial.println("GO");
  f1 = FarbsensorTCS34725(MultiAdd, 6, 13, GAIN_4x, INTEG_CYCLE_10);
  f2 = FarbsensorTCS34725(MultiAdd, 7, 13, GAIN_4x, INTEG_CYCLE_10);
  // put your setup code here, to run once:
}

void loop() 
{
  Serial.println("Sensor 1");
  f1.farbeErkennen();
  Serial.println("Sensor 2");
  f2.farbeErkennen();
  // put your main code here, to run repeatedly:
  delay(10000);
}