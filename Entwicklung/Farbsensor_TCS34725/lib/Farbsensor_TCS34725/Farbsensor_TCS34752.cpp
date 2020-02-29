#include <Farbsensor_TCS34725.h>
Farbsensor_TCS34725::Farbsensor_TCS34725(){};
Farbsensor_TCS34725::Farbsensor_TCS34725(uint8_t add, int led, uint8_t aGain, uint8_t integCycle);
{
    i2c = new I2C(add, led);
    aGain = aGain;
    integCycle = integCycle;
};
Farbsensor_TCS34725::Farbsensor_TCS34725(uint8_t add, int led, int wTime, bool wLong, int aGain, int integCycle)
{
    i2c = new I2C(add, led);
    aGain = aGain;
    integCycle = integCycle;
    wTime = wTime;
    wLong = wLong;
};

void Farbsensor_TCS34725::setIntegCycle(int integCycle)
{
    integCycle = integCycle;
};
void Farbsensor_TCS34725::setAGain(int aGain)
{
    aGain = aGain;
};
void Farbsensor_TCS34725::setWTime(int wTime)
{
    wTime = wTime;
};
void Farbsensor_TCS34725::setWLong(bool wLong)
{
    wLong = wLong;
};
int Farbsensor_TCS34725::farbeErkennen()
{

};