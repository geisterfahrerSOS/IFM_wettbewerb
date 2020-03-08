#include <Arduino.h>
#include <Radsteuerung.h>
RadSteuerung Roboter1;
void setup()
{
  // put your setup code here, to run once:
  Roboter1 = Radsteuerung(0, 1, 2, 3);
}

void loop()
{
  // put your main code here, to run repeatedly:
  Roboter1.liveRad();
  if(ButtonPressed)//Einfach mal als beispiel
  {
    Roboter1.geradeFahren(100); //wenn der Button gepresset wird soll er 10cm nach vorne Fahren; wenn der Roboter mit einer command noch nicht 
    //fetig ist wird ein neuer command wahrscheinlich nicht funktionieren
  }
}
