#include <Arduino.h>
#include <Ultraschall.h>

Ultraschall::Ultraschall(){};
Ultraschall::Ultraschall(int aEcho, int aTrigger)
{
  echo = aEcho;
  trigger = aTrigger;
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
};

float Ultraschall::getDistanz()
{
  float distance = 32767;
  digitalWrite(trigger, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigger, LOW);
  unsigned long stamp = millis();
  while(digitalRead(echo) == LOW && stamp + 50 > millis())
  {
    //Warten auf HIGH
  }
  if(stamp + 10 < millis())
  {
	  return distance;
  }
  unsigned long stamp2 = millis();
  unsigned long timeStampGo = micros();
  while(digitalRead(echo) == HIGH && stamp2 + 50 > millis())
  {
    //Warten auf LOW
  }
  if(stamp + 10 < millis())
  {
	  return distance;
  }
  distance = micros() - timeStampGo;
  distance = distance / 58 - 1;  
  return distance;
};
