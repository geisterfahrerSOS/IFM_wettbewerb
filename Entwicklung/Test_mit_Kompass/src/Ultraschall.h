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
        Ultraschall(int aEcho, int aTrigger);
        Ultraschall();
        float getDistanz();      
};
#endif