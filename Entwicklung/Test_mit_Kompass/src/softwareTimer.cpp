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

int compareVal = 31250;

int swTimerLength = 10;
int timerArray[10];
int actionArray[10];


#define HALLO 0
#define BYE 1

void setup()
{
  noInterrupts();           // Alle Interrupts temporär abschalten
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;                // Register mit 0 initialisieren
  OCR2A = compareVal;            // Output Compare Register vorbelegen 
  TCCR2B |= (1 << CS12);    // 256 als Prescale-Wert spezifizieren
  TIMSK2 |= (1 << OCIE1A);  // Timer Compare Interrupt aktivieren
  interrupts();             // alle Interrupts scharf schalten

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
  */
  if(actionArray[HALLO] == 1)
  {
     actionArray[HALLO] = 0;
     Serial.println("HALLO");
     timerArray[1] = 15;
  }
  if(actionArray[BYE] == 1)
  {
    actionArray[BYE] = 0;
    Serial.println("BYE");
    Serial.println();
    timerArray[0] =10;
  }
}

ISR(TIMER2_COMPA_vect)
{
  TCNT2 = 0; 
  Serial.println("1");
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