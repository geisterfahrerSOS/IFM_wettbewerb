/*
--TO-DO--
DRTIFT GYROSCOP -- fast


FARBSENSOR


RADSENSOREN

DISPLAY montieren
//ULTRASCHALL
//LINIENSENSOr
*/



#include <Arduino.h>
#include <FarbsensorTCS34725.h>
#include <Kompass.h>
#include <LiquidCrystal_I2C.h>
#include <ULTRASCHALL.h>

void display();
void aaSENSOREN();
int sensorenCyclus = 0;
int sensorenCyclen = 2;
void aaULTRASCHALL();
//int geschwindigkeit;
float abstand = 0;
float wiggleAbstand = 0.8;
void aaFARBENFAHREN();
//int geschwindigkeit;
int farbe = -1;
void aaDREHEN();
//int geschwindigkeit;
bool neugestartetDrehen = true, fertig = false;
float winkel = 0;
bool actionManagerAufrufen = false;
//int geschwindigkeit;
void ultraschall_messen();
float abstandUltraschall = 0;
float smooth(float data, float filterVal, float smoothedVal);
float filterValUltra = 0.15;
float smoothedValUltra = 0;
void kompass_messen();
float winkel_Kompass = 0;
float deltaStart = 0; // am Anfang ändern
void actionManager();
void gyrometer_messen();
float gyroData = 0;
void getAbweichung();
float abweichung = 0;
void aaGERADEAUSFAHREN();
int geschwindigkeit = 0;
bool korrigieren = true;
float wiggleWinkel = 5;
void aaRUECKWAERSTSFAHREN();
//int geschwindigkeit;
void farbeFarbsensoren();
int farbe_farbsensor_1, farbe_farbsensor_2;
void getData(boolean debug);
int data[6];
/*
bool korrigieren;
float wiggleWinkel = 5;
*/
void links(int speed);
void rechts(int speed);
void geradeaus(int speed);
void rueckwaerts(int speed);
void stop();
void radAusgabeBool(int moveLeft, int moveRight, int speed);



#define MULTIPLEXERADD 0x70

#define PR 9 //POWER
#define RR 8 //DIR

#define PL 10 //POWER
#define RL 12 //DIR

FarbsensorTCS34725 farbsensor_1, farbsensor_2;
Winkel kompass;
Ultraschall ultraschall;
//Temporär
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

//#define ADDRESS1 0x1D //LSM303D Ultra compact high performance e-Compass \ 0x1E wenn SAO mit ground verbunden wird \3D accelerometer and 3D magnetometer module
//#define ADDRESS2 0x6B //L3GD20H MEMS motion sensor: three-axis digital output gyroscope

// #define OUT_X_L 0x28
// #define OUT_X_H 0x29
// #define OUT_Y_L 0x2A
// #define OUT_Y_H 0x2B
// #define OUT_Z_L 0x2C
// #define OUT_Z_H 0x2D

//größe der SW-TTimer
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
#define FARBENFAHREN 7

#define ACCEL 8
#define NAHESTEDISTANZ 9
#define NAHEDATAEINTRAG 10


//???
int richtung = 0;


bool cycle;

/****************************************************************************************************************************/
void setup()
{
    Serial.begin(9600);
    pinMode(PR, OUTPUT);
    pinMode(PL, OUTPUT);
    pinMode(RL, OUTPUT);
    pinMode(RR, OUTPUT);
    {
        FS_Param_multiAdd_type pParameter;
        pParameter.mulitAdd = MULTIPLEXERADD;
        pParameter.channel = 0;
        pParameter.aAGain = GAIN_4x;
        pParameter.aIntegCycle = INTEG_CYCLE_10;
        pParameter.aLed = 13;
        farbsensor_1 = FarbsensorTCS34725(pParameter);
    }
    {
        FS_Param_multiAdd_type pParameter;
        pParameter.mulitAdd = MULTIPLEXERADD;
        pParameter.channel = 1;
        pParameter.aAGain = GAIN_4x;
        pParameter.aIntegCycle = INTEG_CYCLE_10;
        pParameter.aLed = 12;
        farbsensor_2 = FarbsensorTCS34725(pParameter);
    }

    // kompass.startup();
    
    // kompass.fixPassivDrift();
    // lcd.begin();
    // ultraschall = Ultraschall(2, 3);
    // /******************************************************/
    // noInterrupts();
    // TCCR2A = (1 << WGM21); //Interuptmodus setzen -> Compare
    // OCR2A = 249;           //2ms pro Intervall

    // TIMSK2 |= (1 << OCIE2A);

    // TCCR2B |= (0 << CS21); //Prescale 1 : 8
    // TCCR2B |= (1 << CS20);
    // interrupts();

    // /******************************************************/
    // cycle = true;
    // //nicht vergessen den timerArray festzulegen
    // timerArray[SENSOREN] = 10;
    
    // //timerArray[ACCEL] = 500;
    // //timerArray[NAHEDATAEINTRAG] = 50;
}
/****************************************************************************************************************************/
//   actionArray[GERADEAUSFAHREN] = 1;
    //   korrigieren = true / false;
    //   geschwindigkeit = 0 - 100;

    //   actionArray[RUECKWAERTSFAHREN] = 1;
    //   korrigieren = true / false;
    //   geschwindigkeit = 0 - 100;


    // actionArray[ULTRASCHALL] = 1;
    // abstand = x;
    //korrigieren = true / false;
    //   geschwindigkeit = 0 - 100;
    //actionManagerAufrufen ? true / false;


    //   actionArray[DREHEN] = 1;
    //   winkel =  -360 > x < 360;
    //actionManagerAufrufen ? true / false;

    //  actionArray[FARBEFAHREN] = 1;
    //  farbe = 0 < y > 7;
    // richtug = 1 / 2
    //   geschwindigkeit = 0 - 100;
    //actionManagerAufrufen ? true / false;

void actionManager()
{
  kompass.state = false;
  switch (actionIndex)
  {
  }
}
/****************************************************************************************************************************/
void loop()
{
    farbsensor_1.farbeErkennen();
    farbsensor_2.farbeErkennen();
    Serial.println("");
    delay(1000);
    // display();
    // if(actionArray[ACCEL] == 1)
    // {
    //     //aaACCEL();
    // }
    // if(actionArray[SENSOREN])
    // {
    //     aaSENSOREN();
    // }
    // if (actionArray[ULTRASCHALL] == 1)
    // {
    //     aaULTRASCHALL();
    // }
    // if (actionArray[FARBENFAHREN] == 1)
    // {
    //     aaFARBENFAHREN();
    // }
    // if (actionArray[DREHEN] == 1)
    // {
    //     aaDREHEN();
    // }
    // if(actionArray[GERADEAUSFAHREN] == 1)
    // {
    //     aaGERADEAUSFAHREN();
    // }
    // if(actionArray[RUECKWAERSTSFAHREN] == 1)
    // {
    //     aaRUECKWAERSTSFAHREN();
    // }
}
/****************************************************************************************************************************/
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
/****************************************************************************************************************************/
void display()
{
  //Nie Home benutzen denn es gibt einen 2000ms delay!!!!!!!!
  // lcd.setCursor(0, 0);
  // lcd.print(data[0]);
  // lcd.print(" ");
  // lcd.print(data[1]);
  lcd.setCursor(8, 0);
  lcd.print("E: ");
  lcd.print(abstandUltraschall);
  lcd.print("   ");
  lcd.setCursor(0, 1);
  lcd.print("G: ");
  lcd.print(gyroData);
  lcd.print("   ");
  lcd.setCursor(8, 1);
  lcd.print("R: ");
  lcd.print(richtung);
  // lcd.print("D:");
  // lcd.print(abweichung);
  // lcd.print("   ");
}
/****************************************************************************************************************************/
void ultraschall_messen()
{
  abstandUltraschall = smooth(ultraschall.getDistanz(), filterValUltra, smoothedValUltra);
}
/****************************************************************************************************************************/
float smooth(float data, float filterVal, float smoothedVal) //je größer der filterVal desto mehr werden die vergangenen Werte berücksichtigt
{

  if (filterVal > 1)
  { // check to make sure param's are within range
    filterVal = .99;
  }
  else if (filterVal <= 0)
  {
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal * filterVal);
  return smoothedVal;
}
/****************************************************************************************************************************/
void gyrometer_messen()
{
    gyroData = kompass.gyroBerechnung();
}
/****************************************************************************************************************************/
/*
void aaACCEL()
{
    // Serial.print("X Gyro:  ");
    // Serial.println(kompass.getGyroX());
    // Serial.print("Y Gyro:  ");
    // Serial.println(kompass.getGyroY());
    // Serial.print("Z Gyro:  ");
    // Serial.println(kompass.getGyroZ());
    // Serial.println(kompass.chipRead(ADDRESS2, 0x0F));
    // Serial.print("MagX:  ");
    // Serial.println(kompass.getMagX());
    // Serial.print("MagY:  ");
    // Serial.println(kompass.getMagY());
    // Serial.print("MagZ:  ");
    // Serial.println(kompass.getMagZ());

    // Serial.print("Pitch:  ");
    // Serial.println(kompass.getPitch());
    // Serial.print("Roll:  ");
    // Serial.println(kompass.getRoll());
    // Serial.print("Yaw:  ");
    // Serial.println(kompass.getYaw());

    timerArray[ACCEL] = 500;
    actionArray[ACCEL] = 0; 
}
*/
/****************************************************************************************************************************/
void aaSENSOREN()
{
    switch (sensorenCyclus)
    {
    //Farbsensor
    case 0:
      //farbeFarbsensoren();
      kompass_messen();
      gyrometer_messen();
      break;
    //ULTRASCHALL
    case 1:
      gyrometer_messen();
      ultraschall_messen();
      kompass_messen();
      break;
    //Sonst
    default:
      //irgendwas ist schief gelaufen -->
      sensorenCyclus = 0;
      //
    }


    //Action ist beendet
    actionArray[SENSOREN] = 0;
    //beim nächsten mal soll der nächste Sensor ausgelesen werden
    sensorenCyclus++;
    //in der nächsten Loop wird der erste Sensor wieder aufegufen
    if (sensorenCyclus >= sensorenCyclen)
    {
      sensorenCyclus = 0;
      if (cycle)
      {
        cycle = false;
        actionManager();
      }
    }
    //Timer für den SensorTimer aufladen
    timerArray[SENSOREN] = 10; 
}
/****************************************************************************************************************************/
void kompass_messen()
{
  winkel_Kompass = kompass.getWinkel() + deltaStart;
  if (winkel_Kompass > 359)
  {
    winkel_Kompass -= 360;
  }
  else if (winkel_Kompass < 0)
  {
    winkel_Kompass += 360;
  }
  getAbweichung();
}
/****************************************************************************************************************************/
void getAbweichung()
{
  if (abs(richtung - int(winkel_Kompass)) <= 180)
  {
    abweichung = richtung - int(winkel_Kompass);
  }
  else if (richtung - int(winkel_Kompass) > 0)
  {
    abweichung = richtung - int(winkel_Kompass) - 360;
  }
  else if (richtung - int(winkel_Kompass) < 0)
  {
    abweichung = richtung - int(winkel_Kompass) + 360;
  }
}
/****************************************************************************************************************************/
void aaGERADEAUSFAHREN()
{
    //geschwindigkeit festlegen
    if (korrigieren)
    //mit korrigieren
    {
      if (gyroData > wiggleWinkel)
      {
        links(70);
      }
      else if (gyroData < -wiggleWinkel)
      {
        rechts(70);
      }
      else
      {
        geradeaus(geschwindigkeit);
      }
    }
    else
    //ohne korrigieren
    {
      geradeaus(geschwindigkeit);
    }
}
/****************************************************************************************************************************/
/*
bool korrigieren;
float wiggleWinkel = 5;
*/
void aaRUECKWAERSTSFAHREN()
{
    //geschwindigkeit festlegen
    if (korrigieren)
    //mit korrigieren
    {
      if (gyroData > wiggleWinkel)
      {
        links(70);
      }
      else if (gyroData < -wiggleWinkel)
      {
        rechts(70);
      }
      else
      {
        rueckwaerts(geschwindigkeit);
      } 
    }
    else
    {
        rueckwaerts(geschwindigkeit);
    }
}
/****************************************************************************************************************************/
void stop()
{
  radAusgabeBool(0, 0, 0);
}
/****************************************************************************************************************************/
void geradeaus(int speed)
{
  radAusgabeBool(1, 1, speed);
}
/****************************************************************************************************************************/
void rechts(int speed)
{
  radAusgabeBool(1, -1, speed);
}
/****************************************************************************************************************************/
void links(int speed)
{
  radAusgabeBool(-1, 1, speed);
}
/****************************************************************************************************************************/
void rueckwaerts(int speed)
{
  radAusgabeBool(-1, -1, speed);
}
/****************************************************************************************************************************/
void radAusgabeBool(int moveLeft, int moveRight, int speed) // zwischen -1 und 1
{
  if (moveLeft == -1)
  {
    analogWrite(PL, (int) map(speed, 0, 100, 0, 255));
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
    analogWrite(PL, (int) map(speed, 0, 100, 0, 255));
    ///digitalWrite(PL, HIGH);
    digitalWrite(RL, LOW);
  }
  analogWrite(PR, 200);
  if (moveRight == -1)
  {
    analogWrite(PR, (int) map(speed, 0, 100, 0, 255));
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
/****************************************************************************************************************************/
void farbeFarbsensoren()
{
  farbe_farbsensor_1 = farbsensor_1.farbeErkennen();
  farbe_farbsensor_2 = farbsensor_2.farbeErkennen();
}
/****************************************************************************************************************************/
void getData(boolean debug)
{
  for (int i = 0; i < 6; i++)
  {
    data[i] = analogRead('\016' + i);  
    if (debug)
    {
      Serial.print("Pin ");
      Serial.print(i);
      Serial.print(":  ");
      Serial.print(data[i]);
      Serial.print("  ");
    }
  }
  Serial.println("");
}
/****************************************************************************************************************************/
void aaULTRASCHALL()
{
    if (abstandUltraschall > abstand + wiggleAbstand)
    {
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
      if(actionManagerAufrufen)
      {
        actionIndex++;
        actionManager();
      }
      actionArray[ULTRASCHALL] = 0;
    }
}
/****************************************************************************************************************************/
void aaFARBENFAHREN()
{
    if (farbe == farbe_farbsensor_1)
    //Farbe stimmt überein
    {
      actionArray[FARBENFAHREN] = 0;
      switch (richtung)
      {
      case 1:
        actionArray[GERADEAUSFAHREN] = 0;
        break;

      case 2:
        actionArray[RUECKWAERSTSFAHREN] = 0;
        break;
      default:
        actionArray[GERADEAUSFAHREN] = 0;
        actionArray[RUECKWAERSTSFAHREN] = 0;
      }
      stop();
      if(actionManagerAufrufen)
      {
        actionIndex++;
        actionManager();
      }
    }
    else
    {
      switch (richtung)
      {
      case 1:
        actionArray[GERADEAUSFAHREN] = 1;
        break;
      case 2:
        actionArray[RUECKWAERSTSFAHREN] = 0;
      default:
        //richtung ist falsch
        actionArray[FARBENFAHREN] = 0;
        actionArray[GERADEAUSFAHREN] = 0;
        actionArray[RUECKWAERSTSFAHREN] = 0;
        stop();
        break;
      }
    }
}
/****************************************************************************************************************************/
void aaDREHEN()
{
    //sollte die Funtion neu aufgerufen werden, so wird der zielwinkel gesetzt
    if (neugestartetDrehen)
    {
      richtung = gyroData + winkel; //Ziel des winkels
      fertig = false;
      neugestartetDrehen = false;
    }
    if (!fertig)
    {
        if (winkel > 0)
        {
          if (gyroData < richtung)
          {
            rechts(70);
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
            links(70);
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
/****************************************************************************************************************************/
/****************************************************************************************************************************/
/****************************************************************************************************************************/
/****************************************************************************************************************************/
/****************************************************************************************************************************/
/****************************************************************************************************************************/
/****************************************************************************************************************************/