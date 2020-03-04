#include "radSensor.h" 
#include <Arduino.h>

RadSensor::RadSensor(uint8_t radAuswahl, uint8_t ausloeser)//(Pin, Auslöser Wert)
{
    _radPin = radAuswahl;
    _threshold = ausloeser;
}

void RadSensor::begin() //im setup ausführen
{
    pinMode('\016' + _radPin, INPUT_PULLUP);
}

long RadSensor::stepRead()
{
    _befData = getData(false);
    if ((_befData > _threshold) && (_aftData < _threshold))
    {
        _step++;
        Serial.println(_step);
    }
    _aftData = _befData;
    return _step;
}

uint8_t RadSensor::getData(boolean debug)
{
    uint8_t data = 0;
    data = analogRead('\016' + _radPin);

    if (debug)
    {
        Serial.print("Pin ");
        Serial.print(_radPin);
        Serial.print(":  ");
        Serial.print(data);
        Serial.println("  ");
    }
    return data;
}