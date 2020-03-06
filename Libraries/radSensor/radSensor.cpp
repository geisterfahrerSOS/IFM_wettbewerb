#include "radSensor.h"
#include <Arduino.h>

RadSensor::RadSensor(){};
RadSensor::RadSensor(int radAuswahl, int ausloeser, float distanzProStrich) //(Pin, Auslöser Wert, distanzProStrich in mm)
{
    _radPin = radAuswahl;
    _threshold = ausloeser;
    _distanzProStrich = distanzProStrich;
}

void RadSensor::begin() //im setup ausführen
{
    pinMode('\016' + _radPin, INPUT_PULLUP);
}

long RadSensor::stepRead()
{
    _befData = getData(true);
    if ((_befData > _threshold) && (_aftData < _threshold) && (!_triggerd))
    {
        _elapsedTime = millis();
        _triggerd = true;
    }
    else if (_triggerd)
    {
        if (_befData > _threshold)
        {
            if ((millis() - _elapsedTime) > 50)
            {
                _step++;
                _triggerd = false;
            }
        }
        else
        {
            _triggerd = false;
        }
    }
    _aftData = _befData;
    return _step;
}

long RadSensor::umwandlungSteps(long count)
{
    return count*_distanzProStrich;
}

int RadSensor::getData(boolean debug)
{
    int data = 0;
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