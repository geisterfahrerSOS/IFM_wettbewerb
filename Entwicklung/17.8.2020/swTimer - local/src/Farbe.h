#ifndef Farbe_h
#define Farbe_h
#include <Arduino.h>
/*
    Enthält Farbwerte, git Farben aus
*/
class Farbe
{
    private:
        //Farbwerte
        byte red;
        byte green;
        byte blue;
        byte clear;
        //Farbe
        int color;
    public:
        //setzt die Farbe
        void setColor(byte aRed, byte aGreen, byte aBlue);
        void setColor(byte aRed, byte aGreen, byte aBlue, byte aClear);
        //Updated die Farbe
        void checkColor();
        //gibt die Farbe zurück
        int getColor();
        //gibt die Farbe aus(Serial)
        void printColor();
        //Konstruktoren
        Farbe(byte aRed, byte aGreen, byte aBlue, byte aClear);
        Farbe(byte aRed, byte aGreen, byte aBlue);
        Farbe();
};
#endif
