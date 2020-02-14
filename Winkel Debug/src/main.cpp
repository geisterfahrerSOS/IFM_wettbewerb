#include <Arduino.h> //include Arduino.h library

#include "Blink.h"

Winkel test = Winkel();

void setup()
{
  Serial.begin(9600);
  test.startup();
}
void loop()
{
  Serial.println(test.winkel());
}
