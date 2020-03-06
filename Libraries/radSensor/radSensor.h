#ifndef radSensor_h
#define radSensor_h
 
#include <Arduino.h>

class RadSensor 
{
    public:
        RadSensor();
        RadSensor(int radAuswahl, int ausloeser, float distanzProStrich);// int geht nur bis 255 also fals max größer zu uint16_t ändern
        void begin();
        int getData(boolean debug);
        long stepRead();
        long umwandlungSteps(long count);
    private:
        int _radPin;
        int _befData;
        int _aftData = 250;//Damit beim ersten Durchlauf nicht getriggert wird
        int _threshold;
        int _step = 0;
        long _elapsedTime = 0;
        boolean _triggerd = false;
        float _distanzProStrich = 0;
};
#endif