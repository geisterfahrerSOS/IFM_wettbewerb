#include <Arduino.h>
//#include <SingleRadSensor.h>


void engineLEFT();
void engineRIGHT();
float berechneRichtung(int left, int right);

#define INPUTPIN1 2
#define LED1 9
int lastState1 = LOW, raps1 = 0;
bool forward1 = true;
//button
int delayBetweenPress1 = 200;
unsigned long timeStamp1 = 0;

#define INPUTPIN2 3
#define LED2 10
int lastState2 = LOW, raps2 = 0;
bool forward2 = true;
//button
int delayBetweenPress2 = 200;
unsigned long timeStamp2 = 0;


float degreeByRap[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
float degreePerRap = 1.00;




/*
float distanzRaeder = 10;//in cm
float wheelRadius = 5;  //in cm
float umfangRad = 2 * PI * wheelRadius;
float rapsPerWheel = 6;  //



float distanzPerRap = umfangRad / rapsPerWheel;


SingleRadSensor links, rechts;
*/
void setup() 
{
  Serial.begin(9600);
  //links = SingleRadSensor(INPUTPIN1, LED);
  //rechts = SingleRadSensor(INPUTPIN2, LED);

  pinMode(INPUTPIN1, INPUT_PULLUP);
  pinMode(INPUTPIN2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INPUTPIN1), engineLEFT, RISING);
  attachInterrupt(digitalPinToInterrupt(INPUTPIN2), engineRIGHT, RISING);

  //Lichtschranke
  //attachInterrupt(digitalPinToInterrupt(INPUTPIN1), engineLEFT, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(INPUTPIN2), engineRIGHT, CHANGE);
}


void loop() 
{
  // //Interupt
  // links.checkStateButton();
  // rechts.checkStateButton();
  // /*
  //   Rad1
  //   |\
  //   | \
  //   |  \
  //   |  x\
  //   |  | \
  //   | \|/ \
  //   |______\
  //   Rad2   Rad2
  //   */
  

  
  // float distanz = (links.getRaps() - rechts.getRaps());
  // float winkel = atan(distanzRaeder / distanzPerRap) * (180 / PI) * distanz;
  
  // /*
  // float distanz = (links.getRaps() - rechts.getRaps()) * distanzPerRap;
  // float winkel = atan(distanz / distanzRaeder) * (180 / PI);
  // */
  
  // Serial.println(distanz / distanzRaeder);
  // Serial.println(winkel);
  // Serial.println(winkel / 180 * PI);
  // Serial.println("");
  // delay(1000);

  delay(100);
  Serial.println(berechneRichtung(raps1, raps2));
  Serial.println();
}




void engineLEFT()
{
  if(delayBetweenPress1 + timeStamp1 > millis())
    return;
  timeStamp1 = millis();
  if(forward1)
    raps1++;
  else
    raps1--;
}

void engineRIGHT()
{
  if(delayBetweenPress2 + timeStamp2 > millis())
    return;
  timeStamp2 = millis();
  if(forward2)
    raps2++;
  else
    raps2--;
}

float berechneRichtung(int left, int right)
{
  int sizeOfArray = sizeof(degreeByRap) / sizeof(float);

  int diszanceDriven = left - right;
  int overTime = 0;

  //Mögl. 1
  //return diszanceDriven * degreePerRap;
  //Mögl. 2
  if(diszanceDriven < 0)
  {
    diszanceDriven *= -1;
    while(diszanceDriven >= sizeOfArray)
    {
      overTime++;
      diszanceDriven -= sizeOfArray;
    }
    return -degreeByRap[diszanceDriven] + overTime * -degreeByRap[sizeOfArray - 1];
  }
  else
  {
    while(diszanceDriven >= sizeOfArray)
    {
      overTime++;
      diszanceDriven -= sizeOfArray;
    }
    return degreeByRap[diszanceDriven] + overTime * degreeByRap[sizeOfArray - 1];
  }
  return 0.00;
}
