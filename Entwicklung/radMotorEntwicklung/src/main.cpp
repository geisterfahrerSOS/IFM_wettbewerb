
#include <Arduino.h>
#include <Kompass.h>
#include <LiquidCrystal_I2C.h>
#include <Ultraschall.h>

#define PR 3
#define RR 6 //changed from 2
#define PL 5
#define RL 4

#define ML 11
#define MR 12

/*
2: Richtung rechtes Rad braun
3: AN/AUS rechtes Rad rot
4: Richtung linkes Rad orange
5: AN/AUS linkes Rad gelb
*/
void radAusgabe(float moveLeft, float moveRight); // -1, 0, 1 pro Rad alles andere wird nicht gewertet
void measureSteps();
void ausgleich(int move);
void getAbweichung();
void getSerial();
void display();
void radAusgabeBool(int moveLeft, int moveRight); // zwischen -1 und 1

Winkel kompass;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

boolean befL = false;
boolean aftL = false;
float stepL = 0;
boolean befR = false;
boolean aftR = false;
float stepR = 0;

long ST1 = 0;
long ST2 = 0;

long STDisplayOutput = 0;

long timeR = 0;
long timeL = 0;

long zeit = 0;
long delta = 0;

float schnelle = 0.8;

/////////////////Kompass Steuerung:
int targetDir = 200;
int abweichung = 0;
int wiggle = 10;

String inString = ""; // string to hold input

Ultraschall ultraschall;


void setup()
{
  lcd.begin();
  lcd.backlight();
  kompass = Winkel();
  kompass.startup();
  Serial.begin(9600);
  ultraschall = Ultraschall(7,8);
  // put your setup code here, to run once:
  for (int i = 2; i < 7; i++)
  {
    pinMode(i, OUTPUT);
  }
  for (int i = 11; i < 14; i++)
  {
    pinMode(i, INPUT_PULLUP);
  }
}

void loop()
{
  getAbweichung();
  getSerial();
  // if (abweichung > wiggle)
  // {
  //   radAusgabeBool(1, -1);
  // }
  // else if (abweichung < -wiggle)
  // {
  //   radAusgabeBool(-1, 1);
  // }
  // else
  // {
  //   radAusgabeBool(1, 1);
  // }

  if (abweichung > wiggle)
  {
    radAusgabeBool(schnelle, -schnelle);
  }
  else if (abweichung < -wiggle)
  {
    radAusgabeBool(-schnelle, schnelle);
  }
  else
  {
    radAusgabeBool(schnelle, schnelle);
  }
  display();
  
}

void radAusgabeBool(int moveLeft, int moveRight) // zwischen -1 und 1
{

  if (moveLeft == -1)
  {
    digitalWrite(PL, HIGH);
    digitalWrite(RL, HIGH);
  }
  else if (moveLeft == 0)
  {
    digitalWrite(PL, LOW);
    digitalWrite(RL, LOW);
  }
  else if (moveLeft == 1)
  {
    digitalWrite(PL, HIGH);
    digitalWrite(RL, LOW);
  }

  if (moveRight == -1)
  {
    digitalWrite(PR, HIGH);
    digitalWrite(RR, HIGH);
  }
  else if (moveRight == 0)
  {
    digitalWrite(PR, LOW);
    digitalWrite(RR, LOW);
  }
  else if (moveRight == 1)
  {
    digitalWrite(PR, HIGH);
    digitalWrite(RR, LOW);
  }
}
void radAusgabe(float moveLeft, float moveRight) // zwischen -1 und 1
{
  //analogWrite(PL, int(abs(moveLeft) * 255));
  analogWrite(PL, 255);
  if (moveLeft < 0)
  {
    digitalWrite(RL, HIGH);
  }
  else if (moveLeft == 0)
  {
    digitalWrite(RL, LOW);
  }
  else if (moveLeft > 0)
  {
    digitalWrite(RL, LOW);
  }

  //analogWrite(PR, int(abs(moveRight) * 255));
  analogWrite(PR, 255);
  if (moveRight < 0)
  {
    digitalWrite(RR, HIGH);
  }
  else if (moveRight == 0)
  {
    digitalWrite(RR, LOW);
  }
  else if (moveRight > 0)
  {
    digitalWrite(RR, LOW);
  }
  /*
  switch (moveRight)
  {
  case -1:
    analogWrite(PR, schnelle);
    analogWrite(RR, 255);
    break;
  case 0:
    analogWrite(PR, 0);
    analogWrite(RR, 0);
    break;
  case 1:
    analogWrite(PR, schnelle);
    analogWrite(RR, 0);
    break;
  default:
    break;
  }
  //ausgleich(moveRight);
  */
}

void measureSteps()
{
  befL = digitalRead(ML);
  if ((!befL) && (aftL != befL))
  {
    stepL++;
    Serial.println("stepL:  ");
    Serial.println(stepL);
    Serial.println("stepLDelta:  ");
    Serial.println(zeit / stepL);
    timeL = zeit;
  }
  aftL = befL;

  befR = digitalRead(MR);
  if ((befR) && (aftR != befR))
  {
    stepR++;
    Serial.println("stepR:  ");
    Serial.println(stepR);
    Serial.println("stepRDelta:  ");
    Serial.println(zeit / stepR);
    timeR = zeit;
  }
  aftR = befR;
}
void ausgleich(int move)
{
  if (millis() - ST1 < 180)
  {
    switch (move) //normale Bewegung für 800ms machen
    {
    case -1:
      digitalWrite(PR, HIGH);
      digitalWrite(RR, HIGH);
      break;
    case 0:
      digitalWrite(PR, LOW);
      digitalWrite(RR, LOW);
      break;
    case 1:
      digitalWrite(PR, HIGH);
      digitalWrite(RR, LOW);
      break;
    default:
      break;
    }
    ST2 = zeit;
  }
  else
  {
    if (millis() - ST2 < 15)
    {
      digitalWrite(PR, LOW); //stehen bleiben für 200ms
      digitalWrite(RR, LOW);
    }
    else
    {
      ST1 = zeit;
    }
  }
}

void getAbweichung()
{
  if (abs(targetDir - int(kompass.winkel())) <= 180)
  {
    abweichung = targetDir - int(kompass.winkel());
  }
  else if (targetDir - int(kompass.winkel()) > 0)
  {
    abweichung = targetDir - int(kompass.winkel()) - 360;
  }
  else if (targetDir - int(kompass.winkel()) < 0)
  {
    abweichung = targetDir - int(kompass.winkel()) + 360;
  }
}
void getSerial()
{
  while (Serial.available() > 0)
  {
    int inChar = Serial.read();
    if (isDigit(inChar))
    {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n')
    {
      Serial.print("Schnelle:");
      schnelle = inString.toInt()/100.0;
      Serial.println(schnelle);

      // clear the string for new input:
      inString = "";
    }
  }
}

void display()
{
  if (millis() - STDisplayOutput > 200)
  {
    lcd.setCursor(0, 0);
    lcd.print("S: ");
    lcd.print(schnelle);
    lcd.print("   ");
    lcd.setCursor(8, 0);
    lcd.print("E: ");
    lcd.print(entfernung);
    lcd.print("   ");
    lcd.setCursor(0, 1);
    lcd.print("K: ");
    lcd.print(int(kompass.winkel()));
    lcd.print("   ");
    lcd.setCursor(8, 1);
    lcd.print("D:");
    lcd.print(abweichung);
    lcd.print("   ");
    STDisplayOutput = millis();
  }
}