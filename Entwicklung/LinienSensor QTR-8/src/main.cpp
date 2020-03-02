#include <Arduino.h>

void LineSensorOutput(float *pin0, float *pin1, float *pin2, float *pin3, float *pin4, float *pin5, float *pin6, float *pin7);
//void LineSensorOutputArray(float *wert[total]);
float linienOrt(int *input);
int anzahlAnWerte(int *array);
int arraySumme(int *array);
float drehCommand(float pos);
void arrayFill(boolean trigger, float value, unsigned long time);
boolean compareSet();
void leftShift();
void displayArray2D();
void getData(boolean debug);

const int total = 6;

int wert[total];
int threshold = 500;     //ab wann wird die Dunkelheit als Linie gewertet
int steerValueMain = 20; //Wie viel soll korrigiert werden//Soll nachher noch durch eine variable ersetzt werden
int steerValueLeft = 1;  // verschiedene Steervalue je nach vorherigen werten
int steervalueRight = 1;
float centerPos = 0;       //wird im setup berechnet und gibt mittlere also gewünschte Fahrspur
boolean steerMode = false; //kann man verändern je nach Sensor Positionierung;
//float changeThreshold = 0.2; //Remember to implement__________________Wann wir arrayFill() ausgelöst
float bufferArray[10][2]; //Buffer Array zum transport von shift Werten: 0: start (position in posValue Array); 1: stop (position in posValue Array);
//Time not needed because saved in posValue
float comparePos_1 = 0;
float comparePos_2 = 0;
boolean first_Compare = false;

const int posValueLength = 5;
float posValue[posValueLength][2]; //array für Vergangenheitswerte damit man Aussagen über zukünftige Steeringinputs machen kann;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  centerPos = (total - 1) / 2;
  for (int i = 0; i < total; i++)
  {
    pinMode('\016' + i, INPUT_PULLUP);
  }
}
//1. Idee: wenn sich der Wert von linienOrt verändert wird die Timestamp und die posValue abgespeichert, wie schnell und v.a. um wie viel sich der Wert ändert gibt an
//wie sehr man wider zurücksteuern muss.
//2. Idee: man schaut in regelmäßigen Abständen: wenn man sich von rechts nach links zur mitte bewegt hat, ist es wahrscheinlich dass es sich weiter nach rechts bewegen wird.
//also setzt man die steering value nach links höher.

void loop()
{

  getData(false);
  Serial.println("");
  Serial.print("posValue:  ");
  Serial.println(linienOrt(wert));

  arrayFill(compareSet(), linienOrt(wert), millis());
  displayArray2D();
  // Serial.print("shiftStatus:  ");
  // Serial.println(compareSet());

  delay(2000);
}

void getData(boolean debug)
{
  for (int i = 0; i < total; i++)
  {
    if (debug)
    {
      Serial.print("Pin ");
      Serial.print(i);
      Serial.print(" :  ");
    }
    wert[i] = analogRead('\016' + i);
    if (debug)
    {
      Serial.print(wert[i]);
    }
  }
}

boolean compareSet()
{
  boolean shiftStatus = false;
  comparePos_1 = linienOrt(wert);
  if (first_Compare && (comparePos_1 != comparePos_2))
  {
    shiftStatus = true;
  }
  else
  {
    shiftStatus = false;
    first_Compare = true;
  }
  comparePos_2 = comparePos_1;
  return shiftStatus;
}


float linienOrt(int *input) //gibt Wert aus wo die Linie sich befindet
{
  float summe = 0;
  float j = 0;
  for (int i = 0; i < total; i++)
  {

    if (input[i] < threshold)
    {
      summe += i;
      j++;
    }
  }
  if (j != 0)
  {
    return summe / j;
  }
  else
  {
    return 9;
  }
}

float drehCommand(float pos) //_____________________add steet value parameter or use global Variable steerValueMain
{
  if (pos < centerPos)
  {
    if (steerMode)
    {
      return (pos - centerPos) * steerValueMain; //negativ
    }
    else
    {
      return (centerPos - pos) * steerValueMain; //positiv
    }
  }
  else if (pos > centerPos)
  {
    if (steerMode)
    {
      return (pos - centerPos) * steerValueMain; //positiv
    }
    else
    {
      return (centerPos - pos) * steerValueMain; //negativ
    }
  }
  return 0;
}

void arrayFill(boolean trigger, float value, unsigned long time)
{
  if (trigger)
  {
    for (int i = 0; i < posValueLength - 1; i++)
    {
      posValue[i][0] = posValue[i + 1][0];
      posValue[i][1] = posValue[i + 1][1];
    }
    posValue[posValueLength - 1][0] = value;
    posValue[posValueLength - 1][1] = time;
  }
}

void leftShift()
{
  //int start;
  for (int i = posValueLength - 1; i >= 0; i--)
  {
    if (posValue[i] < posValue[i - 1])
    {
    }
  }
}

void displayArray2D()
{
  for (int i = 0; i < posValueLength; i++)
  {
    Serial.print("posValue:  ");
    Serial.print(posValue[i][0]);
    Serial.print("   time:  ");
    Serial.println(posValue[i][1]);
  }
}

int anzahlAnWerte(int *array)
{
  int gesamt = 0;
  int d = 0;
  for (int i = 0; i < 3; i++)
  {
    d++;
    if (array[i] > 0)
    {
      gesamt++;
    }
    Serial.println(array[i]);
  }
  //Serial.println(d);
  return gesamt;
}
int arraySumme(int *array)
{
  int summe = 0;
  for (int i = 0; i < 3; i++)
  {
    summe += array[i];
  }
  return summe;
}
void LineSensorOutput(float *pin0, float *pin1, float *pin2, float *pin3, float *pin4, float *pin5, float *pin6, float *pin7)
{
  *pin0 = analogRead('\016' + 0);
  *pin1 = analogRead('\016' + 1);
  *pin2 = analogRead('\016' + 2);
  *pin3 = analogRead('\016' + 3);
  *pin4 = analogRead('\016' + 4);
}
void LineSensorOutputArray(float *wert)
{
  for (int i = 0; i < total; i++)
  {
    wert[i] = analogRead('\016' + 0);
  }
}