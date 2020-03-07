#include <Arduino.h>
#include <radSensor.h>

RadSensor rad1;
//Wo wird ST1 verändert
long ST1;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  rad1 = RadSensor(5, 900, 25);//umbedigt den threshold Wert einstellen
  rad1.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (millis() - ST1 > 10)
  {
    Serial.println(rad1.umwandlungSteps());
  }
}