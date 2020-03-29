#include <Arduino.h>
#include <FarbsensorTCS34725.h>
#include <Kompass.h>
#include <LiquidCrystal_I2C.h>
#include <ULTRASCHALL.h>
#define MULTIPLEXERADD 0x70

#define PR 3
#define RR 6 //changed from 2
#define PL 5
#define RL 4

void radAusgabe(float moveLeft, float moveRight); // -1, 0, 1 pro Rad alles andere wird nicht gewertet
void measureSteps();
void ausgleich(int move);
void getSerial();
void display();
void radAusgabeBool(int moveLeft, int moveRight); // zwischen -1 und 1
void geradeausAuto(int time);
void geradeaus();
void rechts();
void links();
void rueckwaerts();
void links(int winkel);
void rechts(int winkel);
void rueckwaertsAuto(int time);
void actionManager();
void getAbweichung();
void stop();
void farbSensor1();
void kompass();
void ultraschall1();
//Sensoren
int sensorenanzahl = 2;
FarbsensorTCS34725 farbsensor1;
Winkel kompass11;
Ultraschall ultraschall;

//Display - Temporär
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

//Anzahl der SW-Timer

/*
0 - Sensoren
1 - ULTRASCHALL
2 - GERADEAUSFAHREN
3 - RUECKWAERSTSFAHREN
4 - RECHTSFAHREN
5 - LINKSFAHREN

*/
String inString = "";
//_________________________________________
int deltaStart = 0; //Wichtig vor dem Start festlegen !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//_________________________________________
const int swTimerLength = 25;
int timerArray[swTimerLength];
int actionArray[swTimerLength];
int actionIndex = 0;

#define SENSOREN 0
#define ULTRASCHALL 1
#define GERADEAUSFAHREN 2
#define RUECKWAERSTSFAHREN 3
#define RECHTSFAHREN 4
#define LINKSFAHREN 5
#define DREHEN 6

int farbeFarbsensor1;
int winkelKompass1;
float abstandUltraschall;

//GERADEAUSFAHREN
boolean korrigieren = false;
//ultraschall
int abstand;
float wiggleAbstand = 0.8;
//winkel
int richtung = 0;
int winkelAnfang;

int winkel = 0;
int overflow = 0;
bool neuGestartet = true;
bool fertig = false;

//global wichtige variablen
int wiggleWinkel = 5;
int abweichung = 0;
//SoftwareTImerVariablen
int sensorCyclen = 0;

bool cycle = false;
//1 Cycle -> 2 ms

void setup()
{
  FS_Param_multiAdd_type pParameter;
  pParameter.mulitAdd = MULTIPLEXERADD;
  pParameter.channel = 1;
  pParameter.aAGain = GAIN_4x;
  pParameter.aIntegCycle = INTEG_CYCLE_10;
  pParameter.aLed = 13;
  farbsensor1 = FarbsensorTCS34725(pParameter);
  kompass11.startup();
  lcd.begin();
  ultraschall = Ultraschall(7, 8);
  Serial.begin(9600);
  /******************************************************/
  noInterrupts();
  TCCR2A = (1 << WGM21); //Interuptmodus setzen -> Compare
  OCR2A = 249;           //2ms pro Intervall

  TIMSK2 |= (1 << OCIE2A);

  TCCR2B |= (0 << CS21); //Prescale 1 : 8
  TCCR2B |= (1 << CS20);
  interrupts();

  /******************************************************/
  timerArray[SENSOREN] = 10;
  cycle = true;
}
void loop()
{
  display();
  //Sensoren auslesen
  if (actionArray[SENSOREN] == 1)
  {
    switch (sensorCyclen)
    {
    //Farbsensor
    case 0:
      //farbeFarbsensor1();
      kompass();
      break;
    //ULTRASCHALL
    case 1:
      ultraschall1();
      kompass();
      break;
    //Sonst
    default:
      //irgendwas ist schief gelaufen -->
      sensorCyclen = 0;
      //
    }
    //Action ist beendet
    actionArray[SENSOREN] = 0;
    //beim nächsten mal soll der nächste Sensor ausgelesen werden
    sensorCyclen++;
    //in der nächsten Loop wird der erste Sensor wieder aufegufen
    if (sensorCyclen >= sensorenanzahl)
    {
      sensorCyclen = 0;
      if (cycle)
      {
        cycle = false;
        actionManager();
      }
    }
    //Timer für den SensorTimer aufladen
    timerArray[SENSOREN] = 1;
  }
  if (actionArray[ULTRASCHALL] == 1)
  {

    if (abstandUltraschall > abstand + wiggleAbstand)
    {
      Serial.println("abstandUltraschall");
      Serial.println(abstandUltraschall);
      actionArray[GERADEAUSFAHREN] = 1;
      actionArray[RUECKWAERSTSFAHREN] = 0;
    }
    else if (abstandUltraschall < abstand - wiggleAbstand)
    {
      actionArray[GERADEAUSFAHREN] = 0;
      actionArray[RUECKWAERSTSFAHREN] = 1;
    }
    else
    {
      actionArray[GERADEAUSFAHREN] = 0;
      actionArray[RUECKWAERSTSFAHREN] = 0;
      stop();
      actionIndex++;
      actionManager();
      actionArray[ULTRASCHALL] = 0;
    }
  }
  if (actionArray[DREHEN] == 1)
  {
    //sollte die Funtion neu aufgerufen werden, so wird der zielwinkel gesetzt
    if (neuGestartet)
    {
      richtung = winkelKompass1 + winkel; //Ziel des winkels
      if (richtung < 0)
      {
        richtung += 360;
        overflow = -1;
      }
      else if (richtung > 359)
      {
        overflow = 1;
        richtung -= 360;
      }
      neuGestartet = false;
      fertig = false;
    }
    if (!fertig)
    {

      switch (overflow)
      {
      case -1:
        if ((winkelKompass1 >= 0) && (winkelKompass1 < richtung) || winkelKompass1 > richtung + wiggleWinkel)
        {
          links();
        }
        else
        {
          fertig = true;
        }
        break;
      case 0:
        if (winkel > 0)
        {
          if (winkelKompass1 < richtung)
          {
            rechts();
          }
          else
          {
            fertig = true;
          }
        }
        else
        {
          if (winkelKompass1 > richtung)
          {
            links();
          }
          else
          {
            fertig = true;
          }
        }
        break;
      case 1:
        if ((winkelKompass1 <= 360) && (winkelKompass1 > richtung) || winkelKompass1 < richtung - wiggleWinkel)
        {
          rechts();
        }
        else
        {
          fertig = true;
        }
        break;
      default:
        if (winkel > 0)
        {
          if (winkelKompass1 < richtung)
          {
            rechts();
          }
          else
          {
            fertig = true;
          }
        }
        else
        {
          if (winkelKompass1 > richtung)
          {
            links();
          }
          else
          {
            fertig = true;
          }
        }
      }
    }
    else
    //Ziel ereicht
    {
      fertig = false;
      neuGestartet = true;
      richtung = 0;
      actionArray[DREHEN] = 0;
      stop();
      actionIndex++;
      actionManager();
    }
  }

  if (actionArray[GERADEAUSFAHREN] == 1)
  {
    //geschwindigkeit festlegen
    if (korrigieren)
    //mit korrigieren
    {

      if (abweichung > wiggleWinkel) //evtl. SWT hinzufügen abweichung ändert sich alle 45 Cycles also 90 ms; daher evtl. Kompass öfters auslesen
      {
        rechts();
      }
      else if (abweichung < -wiggleWinkel)
      {
        links();
      }
      else
      {
        geradeaus();
      }
    }
    else
    //ohne korrigieren
    {
      geradeaus();
    }
  }

  if (actionArray[RUECKWAERSTSFAHREN] == 1)
  {
    //geschwindigkeit festlegen
    if (korrigieren)
    //mit korrigieren
    {

      if (abweichung > wiggleWinkel) //evtl. SWT hinzufügen abweichung ändert sich alle 45 Cycles also 90 ms; daher evtl. Kompass öfters auslesen
      {
        rechts();
      }
      else if (abweichung < -wiggleWinkel)
      {
        links();
      }
      else
      {
        rueckwaerts();
      }
    }
    else
    //ohne korrigieren
    {
      rueckwaerts();
    }
  }
  //Bewegen
}

void getAbweichung()
{
  if (abs(richtung - int(winkelKompass1)) <= 180)
  {
    abweichung = richtung - int(winkelKompass1);
  }
  else if (richtung - int(winkelKompass1) > 0)
  {
    abweichung = richtung - int(winkelKompass1) - 360;
  }
  else if (richtung - int(winkelKompass1) < 0)
  {
    abweichung = richtung - int(winkelKompass1) + 360;
  }
}

void actionManager()
{
  switch (actionIndex)
  {
  //steht NIE alleine
  case 0:
    actionArray[ULTRASCHALL] = 1;
    abstand = 10;
    // case 1:
    //   actionArray[ULTRASCHALL] = 1;
    //   abstand = 30;
    //   korrigieren = true;
    // case 2:
    //   actionArray[DREHEN] = 1;
    //   winkel = 180;
    // case 3:
    //   actionArray[ULTRASCHALL] = 1;
    //   abstand = 10;
  }
}
ISR(TIMER2_COMPA_vect)
{
  for (int i = 0; i < swTimerLength; i++)
  {
    if (timerArray[i] > 0)
    {
      timerArray[i] -= 1;
      if (timerArray[i] == 0)
      {
        actionArray[i] = 1;
      }
    }
  }
}
void stop()
{
  radAusgabeBool(0, 0);
}
void geradeaus()
{
  radAusgabeBool(1, 1);
}
void rechts()
{
  radAusgabeBool(1, -1);
}
void links()
{
  radAusgabeBool(-1, 1);
}
void rueckwaerts()
{
  radAusgabeBool(-1, -1);
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
  // ausgleich(moveRight);
}
void radAusgabe(float moveLeft, float moveRight) // zwischen -1 und 1
{
  analogWrite(PL, int(abs(moveLeft) * 255));
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

  analogWrite(PR, int(abs(moveRight) * 255));
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
void farbSensor1()
{
  farbeFarbsensor1 = farbsensor1.farbeErkennen();
}
void kompass()
{
  winkelKompass1 = kompass11.winkel() + deltaStart;
  if (winkelKompass1 > 359)
  {
    winkelKompass1 -= 360;
  }
  else if (winkelKompass1 < 0)
  {
    winkelKompass1 += 360;
  }
  getAbweichung();
}
void ultraschall1()
{
  abstandUltraschall = ultraschall.getDistanz();
}
void measureSteps()
{
  // befL = digitalRead(ML);
  // if ((!befL) && (aftL != befL))
  // {
  //   stepL++;
  //   Serial.println("stepL:  ");
  //   Serial.println(stepL);
  //   Serial.println("stepLDelta:  ");
  //   Serial.println(zeit / stepL);
  //   timeL = zeit;
  // }
  // aftL = befL;

  // befR = digitalRead(MR);
  // if ((befR) && (aftR != befR))
  // {
  //   stepR++;
  //   Serial.println("stepR:  ");
  //   Serial.println(stepR);
  //   Serial.println("stepRDelta:  ");
  //   Serial.println(zeit / stepR);
  //   timeR = zeit;
  // }
  // aftR = befR;
}
// void ausgleich(int move)
// {
//   if (millis() - ST1 < 180)
//   {
//     if (move == -1)
//     {
//       digitalWrite(PR, HIGH);
//       digitalWrite(RR, HIGH);
//     }
//     else if (move == 0)
//     {
//       digitalWrite(PR, LOW);
//       digitalWrite(RR, LOW);
//     }
//     else if (move == 1)
//     {
//       digitalWrite(PR, HIGH);
//       digitalWrite(RR, LOW);
//     }
//     ST2 = zeit;
//   }
//   else
//   {
//     if (millis() - ST2 < 15)
//     {
//       digitalWrite(PR, LOW); //stehen bleiben für 200ms
//       digitalWrite(RR, LOW);
//     }
//     else
//     {
//       ST1 = zeit;
//     }
//   }
// }
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
      // Serial.print("Schnelle:");
      // richtung = inString.toInt();
      // Serial.println(richtung);

      // clear the string for new input:
      inString = "";
    }
  }
}
void display()
{

  lcd.setCursor(0, 0);
  lcd.print("A: ");
  lcd.print(abweichung);
  lcd.print("   ");
  lcd.setCursor(8, 0);
  lcd.print("E: ");
  lcd.print(abstandUltraschall);
  lcd.print("   ");
  lcd.setCursor(0, 1);
  lcd.print("K: ");
  lcd.print(winkelKompass1);
  lcd.print("   ");
  lcd.setCursor(8, 1);
  lcd.print("R: ");
  lcd.print(richtung);
  // lcd.print("D:");
  // lcd.print(abweichung);
  // lcd.print("   ");
}