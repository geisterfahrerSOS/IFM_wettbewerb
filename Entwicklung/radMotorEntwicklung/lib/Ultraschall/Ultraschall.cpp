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
  while(digitalRead(echo) == LOW || stamp + 10 > millis())
  {
    //Warten auf HIGH
	  Serial.println(1);
  }
  if(stamp + 10 > millis())
  {
	  return distance;
  }
  stamp = millis();
  unsigned long timeStampGo = micros();
  while(digitalRead(echo) == HIGH || stamp + 10 > millis())
  {
    //Warten auf LOW
	  Serial.println(2);
  }
  if(stamp + 10 > millis())
  {
	  return distance;
  }
  distance = micros() - timeStampGo;
  distance = distance / 58;  
  return distance;
};
