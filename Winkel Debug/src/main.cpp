#include <Arduino.h> //include Arduino.h library

#include "Blink.h"

// #define SDA 4
// #define SCL 5
// #define ADDRESS1 0x1D //LSM303D Ultra compact high performance e-Compass \ 0x1E wenn SAO mit ground verbunden wird \3D accelerometer and 3D magnetometer module
// #define ADDRESS2 0x6B //L3GD20H MEMS motion sensor: three-axis digital output gyroscope
// #define OUT_X_L_M 0x08
// #define OUT_X_H_M 0x09
// #define OUT_Y_L_M 0x0A
// #define OUT_Y_H_M 0x0B
// #define OUT_Z_L_M 0x0C
// #define OUT_Z_H_M 0x0D
// #define WHO_AM_I 0x0F
// #define INT_CTRL_M 0x12
// #define STATUS_M 0x07
// #define CTRL5 0x24
// #define TEMP_ON 0x80
// #define TEMP_OFF 0x00
// #define TEMP_OUT_L 0x05
// #define TEMP_OUT_H 0x06
// #define ZERO_32 0x00000000
// #define OUT_X_L_A 0x28
// #define OUT_X_H_A 0x29
// #define OUT_Y_L_A 0x2A
// #define OUT_X_H_A 0x2B
// #define OUT_Z_L_A 0x2C
// #define OUT_Z_H_A 0x2D
// #define CTRL2 0x21
// #define CTRL6 0x25
// #define CTRL7 0x26

extern chipRead(uint8_t add, uint8_t reg);
extern void chipWrite(uint8_t add, uint8_t reg, uint8_t data);
extern uint16_t read12bit(uint8_t add, byte regh, byte regl);
extern float calc(float mag);
extern float winkel();

Winkel test = Winkel();

void setup()
{
  Serial.begin(9600);
  test.startup();
}
void loop()
{
  Serial.println(test.winkel());
}
