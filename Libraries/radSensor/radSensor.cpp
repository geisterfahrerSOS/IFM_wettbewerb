#include "radSensor.h"
#include <Arduino.h>

RadSensor::RadSensor(){};
RadSensor::RadSensor(int radAuswahl, int ausloeser) //(Pin, Auslöser Wert)
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