#ifndef SingleRadSensor_h
#define SingleRadSensor_h

#include <Arduino.h>

class SingleRadSensor
{
    private:
        int pin, led;
        int lastState = LOW, raps = 0, delayBetweenPress = 100;
        unsigned long timeStamp = 0;
        bool button;
    
    public:
        int getRaps();
        SingleRadSensor(int aPin, int LED);
        SingleRadSensor();
        void countRap();
};
#endif