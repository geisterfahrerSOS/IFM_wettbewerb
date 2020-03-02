#include <Arduino.h>

void LineSensorOutput(float *pin0, float *pin1, float *pin2, float *pin3, float *pin4, float *pin5, float *pin6, float *pin7);
//void LineSensorOutputArray(float *wert[total]);
float linienOrt(int *input);
float sensorOutput(int pin);
int anzahlAnWerte(int *array);
int arraySumme(int *array);

const int total = 6;

int wert[total];
int threshold = 500;
int steerValue = 20; //Wie viel soll korrigiert werden//Soll nachher noch durch eine variable ersetzt werden
float centerPos = 0; //wird im setup berechnet und gibt mittlere also gewünschte Fahrspur
boolean steerMode = false; //kann man verändern je nach Sensor Positionierung;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  centerPos = (total-1)/2;
  for (int i = 0; i < total; i++)
  {
    pinMode('\016' + i, INPUT_PULLUP);
  }
}

void loop()
{

  for (int i = 0; i < total; i++)
  {
    // Serial.print("Pin ");
    // Serial.print(i);
    // Serial.print(" :  ");
    wert[i] = analogRead('\016' + i);
    //Serial.print(wert[i]);
  }
  Serial.println("");
  Serial.println(linienOrt(wert));

  delay(1000);
}

float sensorOutput(int pin) //gibt den Analogen Pin für den Wert aus
{
  return analogRead('\016' + pin);
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

  return summe / j;
}

float drehCommand(float pos){
  if((pos<centerPos)!=)
  {
    return (centerPos-pos)*steerValue;

  }else if(pos>centerPos)
  {
    return (pos-centerPos)*steerValue;
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