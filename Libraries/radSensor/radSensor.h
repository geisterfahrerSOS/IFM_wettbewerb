#ifndef radSensor_h
#define radSensor_h
 
#include <Arduino.h>

class RadSensor 
{
    public:
        RadSensor(uint8_t radAuswahl, uint8_t ausloeser);
        void begin();
        uint8_t getData(boolean debug);
        long stepRead();
    private:
        uint8_t _radPin;
        uint8_t _befData;
        uint8_t _aftData;
        uint8_t _threshold;
        uint8_t _step = 0;
};
 
#endif