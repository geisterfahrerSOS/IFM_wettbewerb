#include "radSensor.h"
#include <Arduino.h>

RadSensor::RadSensor(){};
RadSensor::RadSensor(int radAuswahl, int ausloeser, float distanzProStrich) //(Pin, Auslöser Wert, distanzProStrich in mm)
{
    _radPin = radAuswahl;
    _threshold = ausloeser;
    _distanzProStrich = distanzProStrich;
    //pinMode('\016' + _radPin, INPUT_PULLUP);
}

/*
    kann in dem Kostruktor mit RadPin, etc. geschrieben werden, wenn der Konstruktor im setup aufgefufen werden
*/
void RadSensor::begin() //im setup ausführen
{
    pinMode('\016' + _radPin, INPUT_PULLUP);
}

long RadSensor::umwandlungSteps()
{
    stepRead();
    return _step * _distanzProStrich;
}

//void RadSensor::stepRead()
long RadSensor::stepRead()
{
    int _befData = getData(true);
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
    //wenn stepRead eine void Funktion wird, kann das return ausgeklammert werden
    return _step;
}

//gibt den Wert des ausgewählten Pins zurück
int RadSensor::getData(boolean debug)
{
    int data = analogRead('\016' + _radPin);

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