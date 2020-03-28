int swTimerLength = 10;
int timerArray[10];
int actionArray[10];

#define HALLO 0
#define BYE 1

//1 Cycle -> 2 ms

int time1;
int time2;

long timeStamp;
long timeStampNew;

void setup()
{
  Serial.begin(9600);
  /******************************************************/
  noInterrupts();
  TCCR2A=(1<<WGM21);    //Interuptmodus setzen -> Compare  
  OCR2A=249; //2ms pro Intervall
 
  TIMSK2|=(1<<OCIE2A);
 
  TCCR2B|=(0<<CS21);    //Presqale 1 : 8
  TCCR2B|=(1<<CS20);
  interrupts();  
  
  /******************************************************/
  timeStamp = millis();
  time1 = 500;
  time2 = 1500;

  
  timerArray[0] = time1;
}
void loop()
{
  if(actionArray[HALLO] == 1)
  {
     actionArray[HALLO] = 0;
     Serial.print("HALLO :");
     timeStampNew = millis();
     Serial.print(timeStampNew - timeStamp);
     Serial.println(" ms");
     timeStamp = timeStampNew;

     timerArray[BYE] = time2;
  }
  if(actionArray[BYE] == 1)
  {
    actionArray[BYE] = 0;
    Serial.print("BYE :");
    timeStampNew = millis();
    Serial.print(timeStampNew - timeStamp);
    Serial.println(" ms");
    timeStamp = timeStampNew;
    Serial.println();
    timerArray[HALLO] = time1;
  }
}

/******************************************************/
ISR(TIMER2_COMPA_vect)
{
  for(int i = 0; i < swTimerLength; i++)
  {
    if(timerArray[i] > 0)
    {
      timerArray[i] -= 1;
      if(timerArray[i] == 0)
      {
         actionArray[i] = 1;
      }
    }
  }
}
/******************************************************/