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
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  unsigned long stamp = millis();
  
  
  
  /*
  if(stamp + 10 < millis())
  {
	  return distance;
  }
  */
  stamp = millis();
  unsigned long timeStampGo = micros();
delayMicroseconds(250);
  while(digitalRead(echo) == HIGH /*&& stamp + 10 > millis()*/)
  {
    //Warten auf LOW
	  Serial.println(2);
  }
  /*
  if(stamp + 10 < millis())
  {
	  return distance;
  }
  */
  distance = micros() - timeStampGo;
  distance = distance / 58;  
  return distance;
};
