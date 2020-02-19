#include <Arduino.h>
#include <Wire.h>    //include Wire.h library
#include "Kompass.h"

#define ADDRESS1 0x1D //LSM303D Ultra compact high performance e-Compass \ 0x1E wenn SAO mit ground verbunden wird \3D accelerometer and 3D magnetometer module
#define ADDRESS2 0x6B //L3GD20H MEMS motion sensor: three-axis digital output gyroscope
#define OUT_X_L_M 0x08
#define OUT_X_H_M 0x09
#define OUT_Y_L_M 0x0A
#define OUT_Y_H_M 0x0B
#define OUT_Z_L_M 0x0C
#define OUT_Z_H_M 0x0D
#define CTRL7 0x26

Winkel::Winkel()
{
    
}
void Winkel::startup()
{

    Wire.begin();
    chipWrite(ADDRESS1, CTRL7, 0x0);
    Serial.begin(9600);
}
float Winkel::winkel()
{

    float headingDegrees;

    headingDegrees = 360 - (atan2(calc(read12bit(ADDRESS1, OUT_X_H_M, OUT_X_L_M)), calc(read12bit(ADDRESS1, OUT_Y_H_M, OUT_Y_L_M))) + 0.052) * 180 / PI;

    if (headingDegrees < 0)
    {
        headingDegrees += 360;
    }
    if (headingDegrees > 360)
    {
        headingDegrees -= 360;
    }
    Serial,println("MOISSS");
    return headingDegrees;
}

float Winkel::calc(float mag)
{
    float magnetCalc = 0;

    if (mag > 32768)
    {
        magnetCalc = (mag - 65536);
    }
    else
    {
        magnetCalc = mag;
    }
    return magnetCalc;
}

uint16_t Winkel::read12bit(uint8_t add, byte regh, byte regl)
{
    uint8_t low;
    uint8_t high;
    uint16_t readWert;

    Wire.beginTransmission(add);
    Wire.write(byte(regh));
    Wire.endTransmission();

    Wire.requestFrom(add, 1);
    while (!Wire.available())
    {
    }

    high = Wire.read();

    readWert = high;
    readWert = readWert << 8;

    Wire.beginTransmission(add);
    Wire.write(byte(regl));
    Wire.endTransmission();

    Wire.requestFrom(add, 1);
    while (!Wire.available())
    {
    }
    low = Wire.read();

    readWert |= low;
    return readWert;
}

void Winkel::chipWrite(uint8_t add, uint8_t reg, uint8_t data)
{
  Wire.beginTransmission(add);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}
