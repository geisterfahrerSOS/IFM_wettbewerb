#ifndef FarbsensorADJD_S311_h
#define FarbsensorADJD_S311_h
#include <Arduino.h>
#include <I2C.h>
#include <Farbe.h>
/*
    Liest Farbe, braucht Farbe.h
*/
class FarbsensorADJD_S311
{
    private:
        //Pin der LED
        int LED;
        //Farbe-Objekt
        Farbe color;
        //Verbingung zu dem Farbsensor
        I2C i2c;
    public:
        //Konstruktoren
        Farbsensor();
        Farbsensor(int aLed);
        //messen der Farbe
        int farbeErkennen();
};
#endif