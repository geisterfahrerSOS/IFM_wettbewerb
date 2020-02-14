#ifndef Blink_h
#define Blink_h
 
#include <Arduino.h>
class Winkel
{
    public:
        Winkel();
        void startup();
        float winkel();
        float calc(float mag);
        uint16_t read12bit(uint8_t add, byte regh, byte regl);
        void chipWrite(uint8_t add, uint8_t reg, uint8_t data);
};
 
#endif