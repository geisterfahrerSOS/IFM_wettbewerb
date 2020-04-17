/*
    get distance
*/
#ifndef Ultraschall_h
#define Ultraschall_h

#include <Arduino.h>

class Ultraschall
{
    private:

    public:
        int echo, trigger;
        Ultraschall();
        Ultraschall(int aEcho, int aTrigger);
        float getDistanz();      
};
#endif