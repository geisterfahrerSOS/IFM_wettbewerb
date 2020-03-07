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
        //gibt dn Wert 0-1023 des Analogen Pins zzrück
        int getData(boolean debug);
    	//gibt die Umdrehungen des Rades zurück??
        long stepRead();
        //gibt die Strecke Zurück
        long umwandlungSteps(long count);
    private:
        int _radPin;
        //ob sich der Wert verändert
        /*
            _befData wird nur in stepRead benötigt, kann also dammit auch dort erstellt werden
        */
        int _befData;
        int _aftData = 250;//Damit beim ersten Durchlauf nicht getriggert wird
        int _threshold;
        int _step = 0;
        long _elapsedTime = 0;
        boolean _triggerd = false;
        float _distanzProStrich = 0;
};
#endif