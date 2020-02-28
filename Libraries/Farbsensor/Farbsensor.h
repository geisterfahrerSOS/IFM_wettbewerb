#ifndef Farbsensor_h
#define Farbsensor_h
#include <Arduino.h>
#include <I2C.h>
#include <Farbe.h>
/*
    Liest Farbe, braucht Farbe.h
*/
class Farbsensor
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
        Farbsensor(int led);
        //messen der Farbe
        int farbeErkennen();
};
#endif