
#include <Arduino.h>
#include <Kompass.h>

/*
2: rechtes Rad gruen
3: richtung rechtes Rad gelb
4: linkes Rad lila
5: richtung linkes Rad blau
*/
void radAusgabe(boolean seite, boolean richtung); // rechts true vorwaerts true
Winkel kompass1;
boolean state = false;

boolean befL = false;
boolean aftL = false;
int stepL = 0;
boolean befR = false;
boolean aftR = false;
int stepR = 0;
void setup()
{
  kompass1 = Winkel();
  Serial.begin(9600);
  // put your setup code here, to run once:
  for (int i = 2; i < 6; i++)
  {
    pinMode(i, OUTPUT);
  }
  pinMode(13, INPUT_PULLUP);
  Serial.println("test");
}

void loop()
{

  befL = digitalRead(11);
  //Serial.println(befL);
  if ((!befL) && (aftL != befL))
  {
    stepL++;
    Serial.println("stepL:  ");
    Serial.println(stepL++);
  }
  aftL = befL;

  // befR = digitalRead(12);
  // //Serial.println(befR);
  // if ((befR) && (aftR != befR))
  // {
  //   stepR++;
  //   Serial.println("stepR:  ");
  //   Serial.println(stepR++);
  // }
  // aftR = befR;
  //Serial.println(kompass1.winkel());
  state = digitalRead(13);
  while (state==LOW)
  {
    state = digitalRead(13);
    Serial.print("stepL:  ");
    Serial.print(stepL);
    Serial.print("stepR:  ");
    Serial.println(stepR);
  }
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  // put your main code here, to run repeatedly:
  delay(10);
}

void radAusgabe(boolean seite, boolean richtung) // rechts true vorwaerts true
{
  if (seite)
  {
    if (richtung)
    {
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
    }
    else
    {
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
    }
  }
  else
  {
    if (richtung)
    {
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
    }
    else
    {
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
    }
  }
}