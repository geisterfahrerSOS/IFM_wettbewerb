#ifndef radSensor_h
#define radSensor_h
 
#include <Arduino.h>

class RadSensor 
{
    public:
        RadSensor();
        RadSensor(int radAuswahl, int ausloeser, float distanzProStrich);// int geht nur bis 255 also fals max größer zu uint16_t ändern
        //setzt den Analgoen pin
        void begin();
        //gibt dn Wert 0-1023 des Analogen Pins zurück
        int getData(boolean debug);
    	//gibt die Umdrehungen des Rades zurück??
        void stepRead();
        //gibt die Strecke Zurück
        long umwandlungSteps();

        int _step = 0;//Gibt an wie oft die Tipex Markierung am Sensor vorbeistreift

    private:
        int _radPin;
        int _aftData = 250;//Damit beim ersten Durchlauf nicht getriggert wird
        int _threshold;
        long _elapsedTime = 0;
        boolean _triggerd = false;
        float _distanzProStrich = 0;
};
#endif