#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

/*
bits definiert die Größe des Zählerregister in Bits, etwa 16 für einen 16-Bit-Timer.
maxcount entspricht dem maximalen Zahlenwerts 2bits.
prescale ist der oben erläuterte konfigurierbare Prescalewert, also die Zahl der Taktzyklen bis ein weiteres Inkrementieren des Timeregisters erfolgt.
cpufreq repräsentiert die CPU-Frequenz. Der Taktzyklus berechnet sich folglich aus 1 / cpufreq.
initcount ist der vorbelegte Startwert des Zählregisters.
count ist die notwendige Zahl von Inkrementierungen, um einem Timeroverflow auszulösen. Es gilt: count = maxcount - initcount.
deltaT bezeichnet das gewünschte Zeitintervall bis zum Auslösen des Timer-Overflows. Man könnte auch definieren: deltaT = 1 / timerfreq (gewünschte Zahl von Timer Overflows pro Sekunde).
Es gilt prescale / cpufreq * count = deltaT

=> count = deltaT * cpufreq / prescale

=> maxcount - initcount = deltaT * cpufreq / prescale

=> initcount = maxcount - deltaT * cpufreq / prescale

Beispielsrechnung: Alle 0,5 Sekunden soll ein Timer-Overflow-Interrupt stattfinden.

Wir verwenden einen 16-Bit-Timer: bits = 16 => maxcount = 216 = 65536.
Wir benötigen einen Timer Overflow pro halbe Sekunde. deltaT = 0,5 sec = 1 / timerfreq
Die Taktfrequenz des Arduino-Board beträgt cpufreq = 16 MHz = 16.000.000 Hz
Als Prescale-Wert liegt prescale = 256 vor.

*/
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
/*
#define ledPin 13

long compare1 = 0;
long compare2 = 0;
long deltaTime = 0;
long i = 0;
long t1 = 0;
void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); // Ausgabe LED festlegen

  // Timer 1
  noInterrupts(); // Alle Interrupts temporär abschalten
  TCCR2A = 0;
  TCCR2B = 0;

  TCNT2 = 0;                           // Timer nach obiger Rechnung vorbelegen
  TCCR2B |= (1 << CS22) | (1 << CS20); // 1024 als Prescale-Wert spezifizieren
  TIMSK2 |= (1 << TOIE2);              // Timer Overflow Interrupt aktivieren
  interrupts();                        // alle Interrupts scharf schalten
  t1 = micros();
}
// Hier kommt die selbstdefinierte Interruptbehandlungsroutine
// für den Timer Overflow
ISR(TIMER2_COMPA_vect)
{

  // Serial.println(i);
  i++;
  compare2 = micros();
  deltaTime = compare2 - compare1;
  // Serial.println(deltaTime);
  TCNT2 = 0;                                    // Zähler erneut vorbelegen
  digitalWrite(ledPin, digitalRead(ledPin) ^ 1); // LED ein und aus
  compare1 = compare2;
}
//(clockspeed/ Prescaler value) * zeit = 255; | /
//    zeit = 255/(clockspeed/ Prescaler value)
void loop()
{
  // Serial.println(t1);
  if (micros() > t1 + 1000000)
  {
    Serial.println("Zählerwert:  ");
    Serial.println(i);
    i=0;
    t1 = micros();
  }
  // Wir könnten hier zusätzlichen Code integrieren
}
/*
int compareVal = 31250;

const int swTimerLength = 10;
int timerArray[swTimerLength];
int actionArray[swTimerLength];

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

#define HALLO 0
#define BYE 1

void setup()
{
  noInterrupts(); // Alle Interrupts temporär abschalten
  TCCR2A = 0;
  TCCR2B = 0;

  TCNT1 = 34286;           //Timer vorbelegen
  TCCR1B |= (1 << CS12);   //256 als Prescale Wert festlegen
  TIMSK1 |= (1 << TOIE1);  // Timer Overflow Interrupt aktivieren

  TCNT2 = 0;               // Register mit 0 initialisieren
  OCR2A = compareVal;      // Output Compare Register vorbelegen
  TCCR2B |= (1 << CS12);   // 256 als Prescale-Wert spezifizieren
  TIMSK2 |= (1 << OCIE1A); // Timer Compare Interrupt aktivieren
  interrupts();            // alle Interrupts scharf schalten
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  timerArray[0] = 10;
}

void loop()
{
  /*
  if(millis() > timeStamp + timeGap)
  {
    timeStamp = millis();
    decreaseSoftwareTimer();
  }
  
  if (actionArray[HALLO] == 1)
  {
    actionArray[HALLO] = 0;
    Serial.println("HALLO");
    lcd.setCursor(0, 0);
    lcd.println("HALLO");

    timerArray[1] = 15;
  }
  if (actionArray[BYE] == 1)
  {
    actionArray[BYE] = 0;
    Serial.println("BYE");
    lcd.setCursor(0, 0);
    lcd.println("BYE");
    Serial.println();
    timerArray[0] = 10;
  }
}
ISR(TIMER1_OVF_vect)        
{
  Serial.println("MOIS");
  TCNT1 = 34286;
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
}*/

/*
ISR(TIMER2_COMPA_vect)
{
  TCNT2 = 0;
  // Serial.println("1");
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
*/