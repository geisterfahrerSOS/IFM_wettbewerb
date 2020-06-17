#include <Arduino.h>
#include <Kompass.h>
//https://ecksteinimg.de/Datasheet/Pololu/Pololu%20QTRX%20QTR.pdf
//https://www.generationrobots.com/media/capteurs-de-reflexion-infrarouge-QTR-8RC/QTR_arduino_library.pdf


#define PR 9 //POWER
#define RR 4 //DIR

#define PL 10 //POWER
#define RL 6 //DIR

void aaDREHEN();
//int geschwindigkeit;
bool neugestartetDrehen = true, fertig = false;
float winkel;
bool actionManagerAufrufen;
//int geschwindigkeit;
void aaGERADEAUSFAHREN();
int geschwindigkeit;
bool korrigieren;
float wiggleWinkel = 5;
void aaRUECKWAERSTSFAHREN();
//int geschwindigkeit;
void links();
void rechts();
void geradeaus();
void rueckwaerts();
void stop();
void radAusgabeBool(int moveLeft, int moveRight);

void gyrometer_messen();
float gyroData;

Winkel kompass;
float richtung;
void followLine();



void sendData();

#define LEEDON 4
#define switch 13

int space[] = {0, 5};
float sensor; //sensorPos
float middleSensor = 2.5; //The middle of the Sensor line
float wiggle = 0.5;




void setup() {
  Serial.begin(9600);
  pinMode(LEEDON, OUTPUT);
  geschwindigkeit = 100;

  pinMode(13, INPUT);

}

int data[6];
int length = 6;




float whiteBlackBorder = 75;
boolean whiteLine = false;




void loop() 
{
  for(int i = space[0]; i <= space[1]; i++)
  {
    data[i] = analogRead(i);
  }
  if(digitalRead(13) == HIGH)
  {
    sendData();
  }

    int top = 0, bottom = 0;
    float sensor;
    for(int i = 0; i < 6; i++)
    {
      top += i * 1000 * data[i];
      bottom += data[i];
    }
    sensor = float(top) / float(bottom) / 1000.0;
    //Serial.println(sensor);
  for(int i = 0; i < 6; i++)
  {
    if(whiteLine) //wenn die linie weiß ist
    {
      data[i] = 1023 - data[i];
    }
    if(data[i] > whiteBlackBorder)
    {
      data[i] = 1; //Linie - Ja
    }
    else
    {
      data[i] = 0; //Linie - Nein
    }
  }
  if(digitalRead(13) == LOW)
  {
    sendData();
  }

  //followLine();
  delay(1000);
}

void sendData()
{
  String sendString = "";//"SLS;";
  for(int i = 0; i < length; i++)
  {
    sendString += data[i];
    sendString += ";";
  }
  //sendString += "ELS;";
  Serial.println(sendString);
}






// void followLine()
// {
//   if(sensor - wiggle < middleSensor)
//   {
//     rechts();
//   }
//   else if(sensor + wiggle > middleSensor)
//   {
//     links();
//   }
//   else
//   {
//     geradeaus();
//   }
  
// }









// //////////////////////////////////////MOTER//////////////////////////////////////

// void gyrometer_messen()
// {
//     gyroData = kompass.gyroBerechnung();
// }
// /****************************************************************************************************************************/
// void aaGERADEAUSFAHREN()
// {
//     //geschwindigkeit festlegen
//     if (korrigieren)
//     //mit korrigieren
//     {
//       if (gyroData > wiggleWinkel)
//       {
//         links();
//       }
//       else if (gyroData < -wiggleWinkel)
//       {
//         rechts();
//       }
//       else
//       {
//         geradeaus();
//       }
//     }
//     else
//     //ohne korrigieren
//     {
//       geradeaus();
//     }
// }
// /****************************************************************************************************************************/
// /*
// bool korrigieren;
// float wiggleWinkel = 5;
// */
// void aaRUECKWAERSTSFAHREN()
// {
//     //geschwindigkeit festlegen
//     if (korrigieren)
//     //mit korrigieren
//     {
//       if (gyroData > wiggleWinkel)
//       {
//         links();
//       }
//       else if (gyroData < -wiggleWinkel)
//       {
//         rechts();
//       }
//       else
//       {
//         rueckwaerts();
//       } 
//     }
//     else
//     {
//         rueckwaerts();
//     }
// }
// /****************************************************************************************************************************/
// void stop()
// {
//   radAusgabeBool(0, 0);
// }
// /****************************************************************************************************************************/
// void geradeaus()
// {
//   radAusgabeBool(1, 1);
// }
// /****************************************************************************************************************************/
// void rechts()
// {
//   radAusgabeBool(1, -1);
// }
// /****************************************************************************************************************************/
// void links()
// {
//   radAusgabeBool(-1, 1);
// }
// /****************************************************************************************************************************/
// void rueckwaerts()
// {
//   radAusgabeBool(-1, -1);
// }
/****************************************************************************************************************************/
/*
void radAusgabeBool(int moveLeft, int moveRight) // zwischen -1 und 1
{
  if (moveLeft == -1)
  {
    analogWrite(PL, (int) map(geschwindigkeit, 0, 100, 0, 255));
    //digitalWrite(PL, HIGH);
    digitalWrite(RL, HIGH);
  }
  else if (moveLeft == 0)
  {
    analogWrite(PL, 0);
    //digitalWrite(PL, LOW);
    digitalWrite(RL, LOW);
  }
  else if (moveLeft == 1)
  {
    analogWrite(PL, (int) map(geschwindigkeit, 0, 100, 0, 255));
    ///digitalWrite(PL, HIGH);
    digitalWrite(RL, LOW);
  }
  analogWrite(PR, 200);
  if (moveRight == -1)
  {
    analogWrite(PR, (int) map(geschwindigkeit, 0, 100, 0, 255));
    //digitalWrite(PR, HIGH);
    digitalWrite(RR, HIGH);
  }
  else if (moveRight == 0)
  {
    analogWrite(PR, 0);
    //digitalWrite(PR, LOW);
    digitalWrite(RR, LOW);
  }
  else if (moveRight == 1)
  {
    analogWrite(PR, (int) map(geschwindigkeit, 0, 100, 0, 255));
    //digitalWrite(PR, HIGH);
    digitalWrite(RR, LOW);
  }
}

void aaDREHEN()
{
  gyrometer_messen();
    //sollte die Funtion neu aufgerufen werden, so wird der zielwinkel gesetzt
    if (neugestartetDrehen)
    {
      richtung = gyroData + winkel; //Ziel des winkels
      fertig = false;
      neugestartetDrehen = false;
    }
    if (!fertig)
    {
      gyrometer_messen();
        if (winkel > 0)
        {
          if (gyroData < richtung)
          {
            rechts();
          }
          else
          {
            stop();
            fertig = true;
          }
        }
        else
        {
          if (gyroData > richtung)
          {
            links();
          }
          else
          {
            stop();
            fertig = true;
          }
        }
    }
    else
    //Ziel ereicht
    {
      fertig = false;
      neugestartetDrehen = true;
      richtung = 0;
      actionArray[DREHEN] = 0;
      if (actionManagerAufrufen)
      {
        actionIndex++;
        actionManager();
      }
    }    
}
*/
/////////////////////////////////////////////////////////////////////////////////

