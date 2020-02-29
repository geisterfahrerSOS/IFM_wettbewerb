#ifndef Fabrsensor_TCS34725
#define Fabrsensor_TCS34725
//https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf
#include <Arduino.h>
#include <Wire.h>
#include <I2C.h>
#include <Farbe.h>

#define COMMAND_BIT 0x80 // Typ der Übertragung

#define ENABLE 0b00 //REG: aktiviert Interupts und Staten
#define AEN 0b10 //BIT: 1 - aktiviert RGBC
#define NAEN 0b00 //deaktiviert RGBC
#define PON 0b1 //Power ON
#define NPON 0b0 //Power off

#define ATIME 0x01 //REG: RGBC zeit
#define INTEG_CYCLE_1 0xFF //dann MAX-Val: 1024
#define INTEG_CYCLE_10 0xF6 //dann MAX-Val: 10240
#define INTEG_CYCLE_42 0xD5 //dann MAX-Val: 43008
#define INTEG_CYCLE_64 0xC0 //dann MAX-Val: 65535
#define INTEG_CYCLE_256 0x00 //dann MAX-Val: 65535

#define WTIME 0x03 //REG: Warte Zeit
#define WTIME_2_4 0xFF //2.4 ms
#define WTIME_204 0xAB //204 ms
#define WTIME_614 0x00 //614 ms

#define CONFIG 0x0D //REG: Config
#define WLONG 0x10 // 0x10 //BITS: 1 - When asserted, the wait cycles are increased by a factor 12× from that programmed in the WTIME register

#define CONTROL 0x0F //Control
#define AGAIN 0x01 //BITS: 1:0
#define GAIN_1x 0x00
#define GAIN_4x 0x01
#define GAIN_16x 0x10
#define GAIN_60x 0x11
#define STATUS 0x13

#define CDATAL 0x14 //Clear data low byte
#define CDATAH 0x15 //Clear data high byte
#define RDATAL 0x16 //Red data low byte
#define RDATAH 0x17 //Red data high byte
#define GDATAL 0x18 //Green data low byte
#define GDATAH 0x19 //Green data high byte
#define BDATAL 0x1A //Blue data low byte
#define BDATAH 0x1B //Blue data high byte

/*
BIT: 0 - RGBC Valid. Indicates that the RGBC channels have completed an integration cycle
*/
struct Farbsensor_TCS34725
{
    public:
        Farbsensor_TCS34725();
        Farbsensor_TCS34725(uint8_t add, int led, uint8_t aGain, uint8_t integCycle);
        Farbsensor_TCS34725(uint8_t add, int led, uint8_t wTime, bool wLong, uint8_t aGain, uint8_t integCycle);
        void setIntegCycle(int integCycle);
        void setAGain(int aGain);
        void setWTime(int wTime);
        void setWLong(bool wLong);
        int farbeErkennen();
    private:
        Farbe color;
        I2C i2c;
        int WTime, aGain, integCycle, maxVal;
        bool wLong;
};
#endif