#include <FarbsensorTCS34725.h>
#include <kompass1.h>
#include <LiquidCrystal_I2C.h>
#include <Ultraschall.h>
#define MULTIPLEXERADD 0x70

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
void drehen(int winkel);
void actionManager();
void getAbweichung();
//Sensoren
int sensorenanzahl = 3;
FarbsensorTCS34725 farbsensor1;
Winkel kompass11;
Ultraschall ultraschall;

//Display - Temporär
LiquidCrystal_I2C lcd1 = LiquidCrystal_I2C(0x27, 16, 2);

//Anzahl der SW-Timer
const int swTimerLength = 25;
/*
0 - Sensoren
1 - ULTRASCHALL
2 - geradeausFahren
3 - rueckwaertsfahren
4 - rechtsfahren
5 - linksfahren

*/
//_________________________________________
int deltaStart = 0; //Wichtig vor dem Start festlegen !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//_________________________________________
int timerArray[swTimerLength];
int actionArray[swTimerLength];

#define SENSOREN 0
#define Ultraschall 1
#define geradeausFahren 2
#define rueckwaertsfahren 3
#define rechtsfahren 4
#define linksfahren 5

int farbeFarbsensor1;
int winkelkompass11;
int abstandUltraschall;

boolean korrigieren = false;
int abstand;
int winkelAnfang;
int wiggle = 3;
int richtung = 0;
int abweichung = 0;
//1 Cycle -> 2 ms
int sensorTrigger = 0;



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
    lcd1.begin();
    ultraschall = Ultraschall(6, 7);
    Serial.begin(9600);
    /******************************************************/
    noInterrupts();
    TCCR2A = (1 << WGM21); //Interuptmodus setzen -> Compare
    OCR2A = 249;           //2ms pro Intervall

    TIMSK2 |= (1 << OCIE2A);

    TCCR2B |= (0 << CS21); //Presqale 1 : 8
    TCCR2B |= (1 << CS20);
    interrupts();

    /******************************************************/
    timerArray[SENSOREN] = 15;
}

void loop()
{
    //Sensoren auslesen
    if (actionArray[SENSOREN] == 1)
    {
        switch (sensorTrigger)
        {
        //Farbsensor
        case 0:
            farbeFarbsensor1 = farbsensor1.farbeErkennen();
            break;
        //kompass1
        case 1:
            winkelkompass11 = kompass11.winkel() + deltaStart;
            getAbweichung();
            break;
        //Ultraschall
        case 2:
            abstandUltraschall = ultraschall.getAbstand();
            break;
        //Sonst
        default:
            //irgendwas ist schief gelaufen -->
            sensorTrigger = 0;
            //
        }
        //Action ist beendet
        actionArray[SENSOREN] = 0;
        //beim nächsten mal soll der nächste Sensor ausgelesen werden
        sensorTrigger++;
        //in der nächsten Loop wird der erste Sensor wieder aufegufen
        if (sensorTrigger >= sensorenanzahl)
        {
            sensorTrigger = 0;
        }
        //Timer für den SensorTimer aufladen
        timerArray[SENSOREN] = 15;
    }
    //Weg Berechnen
    if (actionArray[geradeausFahren] == 1)
    {
        //geschwindigkeit festlegen
        if (korrigieren)
        //mit korrigieren
        {
            if (abweichung> wiggle) //evtl. SWT hinzufügen
            {
                rechts();
            }
            else if (abweichung < -wiggle)
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
    //Bewegen
}
void getAbweichung()
{
    if (abs(richtung - int(winkelkompass11)) <= 180)
    {
        abweichung = richtung - int(winkelkompass11);
    }
    else if (richtung - int(winkelkompass11) > 0)
    {
        abweichung = richtung - int(winkelkompass11) - 360;
    }
    else if (richtung - int(winkelkompass11) < 0)
    {
        abweichung = richtung - int(winkelkompass11) + 360;
    }
}
void drehen(int winkel) // negative Werte bedeuten links und positive bedeuten rechts
{
    long richtung = kompass11.winkel();
    long ziel = 0;
    long startTime = millis(); //evtl. TimeOut Modus hinzufügen
    if (richtung + winkel < 0) // angenommen winkel = -40 richtung = 20 dann Ziel = 340
    {
        ziel = 360 + richtung + winkel; //340
        while ((kompass1.winkel() > 0) && (kompass1.winkel() < richtung))
        {
            links();
        }
        while (kompass1.winkel() > ziel)
        {
            links();
        }
    }
    else if (richtung + winkel > 360)
    {
        ziel = richtung + winkel - 360;
        while ((kompass1.winkel() < 360) && (kompass1.winkel() > richtung))
        {
            rechts();
        }
        while (kompass1.winkel() < ziel)
        {
            rechts();
        }
    }
    else
    {
        ziel = richtung + winkel;
        if (winkel > 0)
        {
            while (kompass1.winkel() < ziel)
            {
                rechts();
            }
        }
        else
        {
            while (kompass1.winkel() > ziel)
            {
                rechts();
            }
        }
    }
    radAusgabeBool(0, 0);
}

void actionManager()
{
    switch (i)
    {
        //steht NIE alleine
        case 0:
            actionArray[geradeausFahren] = 1;
            richtung = winkelkompass11;
            korrigieren = true;

        case 1 : actionArray[Ultraschall] = 1;
        abstand = 30;
        richtung = 0; //Norden
    case 2:
        actionArray[drehen] = 1;
        drehen = 180;
    case 3:
        actionArray[ultraschall] = 1;
        abstand = 10;
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

    befR = digitalRead(MR);
    if ((befR) && (aftR != befR))
    {
        stepR++;
        Serial.println("stepR:  ");
        Serial.println(stepR);
        Serial.println("stepRDelta:  ");
        Serial.println(zeit / stepR);
        timeR = zeit;
    }
    aftR = befR;
}
void ausgleich(int move)
{
    if (millis() - ST1 < 180)
    {
        if (move == -1)
        {
            digitalWrite(PR, HIGH);
            digitalWrite(RR, HIGH);
        }
        else if (move == 0)
        {
            digitalWrite(PR, LOW);
            digitalWrite(RR, LOW);
        }
        else if (move == 1)
        {
            digitalWrite(PR, HIGH);
            digitalWrite(RR, LOW);
        }
        ST2 = zeit;
    }
    else
    {
        if (millis() - ST2 < 15)
        {
            digitalWrite(PR, LOW); //stehen bleiben für 200ms
            digitalWrite(RR, LOW);
        }
        else
        {
            ST1 = zeit;
        }
    }
}
int getAbweichung(int richtung)
{
    int abweichung = 0;
    if (abs(richtung - int(kompass1.winkel())) <= 180)
    {
        abweichung = richtung - int(kompass1.winkel());
    }
    else if (richtung - int(kompass1.winkel()) > 0)
    {
        abweichung = richtung - int(kompass1.winkel()) - 360;
    }
    else if (richtung - int(kompass1.winkel()) < 0)
    {
        abweichung = richtung - int(kompass1.winkel()) + 360;
    }
    return abweichung;
}
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
    lcd.print("S: ");
    lcd.print(schnelle);
    lcd.print("   ");
    lcd.setCursor(8, 0);
    lcd.print("E: ");
    lcd.print(ultraschall.getabstand());
    lcd.print("   ");
    lcd.setCursor(0, 1);
    lcd.print("K: ");
    lcd.print(int(kompass1.winkel()));
    lcd.print("   ");
    lcd.setCursor(8, 1);
    // lcd.print("D:");
    // lcd.print(abweichung);
    // lcd.print("   ");
}