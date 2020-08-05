#include <Arduino.h>
#include <SingleRadSensor.h>

SingleRadSensor::SingleRadSensor(){}

SingleRadSensor::SingleRadSensor(int aPin, int LED, bool button)
{
    pin = aPin;
    led = LED;
    pinMode(pin, INPUT_PULLUP);
    pinMode(LED, OUTPUT);
    digitalWrite(led, HIGH);
    timeStamp = millis();
    button = button;
    //attachInterrupt(pin, func, HIGH);
}

int SingleRadSensor::getRaps()
{
    return raps;
}