#define INPUTPIN1 11
#define INPUTPIN2 12
#define LED 4


int radius = 10;//cm
int umfang = 40;//??
int unterteilungen = 4;

//LINKS
int lastState1 = 0;
//int border1 = 300;
int raps1 = 0;

//RECHTS
int lastState2 = 0;
//int border2 = 300;
int raps2 = 0;



//checkStateButton()
int delayBetweenPresses = 100;
unsigned long timeStamp = 0;

void setup() 
{
  pinMode(LED,OUTPUT);
  pinMode(INPUTPIN, INPUT_PULLUP);
  Serial.begin(9600);

  delay(1000);
}

void loop() 
{
  checkStateButton();
  //checkState();
  Serial.print("Distanz: "); Serial.println(raps *umfang / unterteilungen);
}



void checkState()
{
  int state;
  digitalWrite(LED, HIGH);
  delay(1000);
  Serial.println(digitalRead(INPUTPIN));
  if(digitalRead(INPUTPIN))
    state = 1;
  else
    state = 0;
  if(state != lastState)
    raps++;
  lastState = state;
  //digitalWrite(LED, LOW); 
}


void checkStateButton()
{
  int state;
  if(timeStamp + delayBetweenPresses < millis())
  {
    if(digitalRead(INPUTPIN))
    {
    timeStamp = millis();
    state = LOW;
    }
    else
      state = HIGH;
  }
  if(state == HIGH && lastState == LOW)
    raps++;
  lastState = state;
  //digitalWrite(LED, LOW); 
}
