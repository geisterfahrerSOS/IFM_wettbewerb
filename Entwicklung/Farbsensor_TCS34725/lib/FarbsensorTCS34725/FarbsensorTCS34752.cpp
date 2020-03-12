#include <FarbsensorTCS34725.h>
//PON | AEN ???


//Konstruktor
FarbsensorTCS34725::FarbsensorTCS34725(){};
//Konstruktor - für Einmalmessung
FarbsensorTCS34725::FarbsensorTCS34725(uint8_t add, int led, uint8_t aGain, uint8_t integCycle)
{
    //I2C-Verbindung aufbauen
    i2c = I2C(add, led);
    //Gain setzen
    setAGain(aGain);
    //Integration Time setzen 
    setIntegCycle(integCycle);
    //Wait time setzen(minimum)
    setWTime(WTIME_2_4);
    //12x wTime nicht aktiv 
    setWLong(false);
    //Schlaf-Modus
    setPon(false);
    //keine Farberkennung
    setAEN(false);
    //Wartezeit-funktion aus
    setWen(false);
    //Sensor hochfahren
    start();
};
//Konstrukor für Messung nach delay(wTime)
FarbsensorTCS34725::FarbsensorTCS34725(uint8_t add, int led, uint8_t wTime, bool wLong, uint8_t aGain, uint8_t integCycle)
{
    //I2C-Verbindung aufbauen
    i2c = I2C(add, led);
    //Gain setzen
    setAGain(aGain);
    //Integration Time setzen 
    setIntegCycle(integCycle);
    //Wait time setzen
    setWTime(wTime);
    //Farberkennung
    setAEN(true);
    //Idle-Modus
    setPon(true);
    //Wartezeit-funktion ein
    setWen(true);
    //12x wTime
    setWLong(wLong);
    //Sensor hochfahren
    start();
};
//setzt den Sensor in den IDLE-Modus
void FarbsensorTCS34725::start()
{
    //Sensor hochfahren
    pon = true;
    i2c.chipWrite(PON | aen | wen, COMMAND_BIT | ENABLE);
    
    //???????
    delay(10);
};
//setzt den Sensor in den SLEEP-Modus
void FarbsensorTCS34725::stop()
{
    //Sensor herunterfahren
    pon = false;
    i2c.chipWrite(NPON | aen | wen, COMMAND_BIT | ENABLE);
};
//erkennt die Farbe
int FarbsensorTCS34725::farbeErkennen()
{
    bool ponSet, aAenSet;
    //wenn der Sensor im Sleep-Modus ist, in den Idle-Modus setzen
    if(!pon)
    {
        ponSet = true;
        start();
    }
    //Wenn Farbenlesen nicht aktiv ist, dass Farvbenlessen aktivieren
    if(!aen)
    {
        aAenSet = true;
        setAEN(true);
    }
    //wenn Zyclenmessung nicht aktiv
    if(!wen)
    {
    uint8_t result;    
    //Warten, bis Farbmessung beendet
    do
    {
      result = i2c.chipRead(STATUS);
      result = result & 0b1;
    }
    while(result ==1);
    }
    //Farbmessen ausstellen
    if(ponSet)
    {
        ponSet = false;
        stop();
    }
    //Farbe lesen
    color.setColor(map(i2c.readSensor(RDATAL, RDATAH), 0, maxVal, 0, 255),
                 map(i2c.readSensor(GDATAL, GDATAH), 0, maxVal, 0, 255),
                 map(i2c.readSensor(BDATAL, BDATAH), 0, maxVal, 0, 255),
                 map(i2c.readSensor(CDATAL, CDATAH), 0, maxVal, 0, 255)
    );
    //Farbe überprüfen
    color.checkColor();
    //Farbe ausgeben
    color.printColor();

    //Wenn Farbenlesen nicht aktiv war, dass Farvbenlessen deaktiwieren
    if(aAenSet)
    {
        aAenSet = false;
        setAEN(false);
    }
    //Farbe zurückgeben
    return color.getColor();
};
//setzt den Integraion Cycle
void FarbsensorTCS34725::setIntegCycle(uint8_t integCycle)
{
    integCycle = integCycle;
    switch(integCycle)
    {
        case INTEG_CYCLE_1:
        maxVal = 1024;
        break;
        case INTEG_CYCLE_10:
        maxVal = 10240;
        break;
        case INTEG_CYCLE_42:
        maxVal = 43008;
        break;
        case INTEG_CYCLE_64:
        maxVal = 65535;
        break;
        case INTEG_CYCLE_256:
        maxVal = 65535;
        break;
        default: 
        maxVal = 1024;
    }
    i2c.chipWrite(integCycle, COMMAND_BIT | ATIME);  
};
//setzt den Gain //nur die ersten zwei Bits
void FarbsensorTCS34725::setAGain(uint8_t aGain)
{
    aGain = aGain;
    i2c.chipWrite(aGain, COMMAND_BIT | CONTROL);
};
//setzt die Wartezeit
void FarbsensorTCS34725::setWTime(uint8_t wTime)
{
    wTime = wTime;
    i2c.chipWrite(wTime, COMMAND_BIT | WTIME);
};
//setzt, ob die wartezeit 12x so hoch ist
void FarbsensorTCS34725::setWLong(bool wLong)
{
    wLong = wLong;
    if(wLong)
    {
        i2c.chipWrite(WLONG, COMMAND_BIT | CONFIG);
    }else
    {
        i2c.chipWrite(NWLONG, COMMAND_BIT | CONFIG);
    }
    
};
//setzt, ob die Wartefunktion aktiviert wird
void FarbsensorTCS34725::setWen(bool wen)
{
    wen = wen;
    if(wen)
    {
        i2c.chipWrite(pon | aen | WEN, COMMAND_BIT | ENABLE);
    }else
    {
        i2c.chipWrite(pon | aen | NWEN, COMMAND_BIT | ENABLE);
    }
};
//Setzt, ob die Farberkennung aktiv ist
void FarbsensorTCS34725::setAEN(bool aen)
{
    aen = aen;
    if(aen)
    {
        i2c.chipWrite(pon | AEN | wen, COMMAND_BIT | ENABLE);
    }else
    {
        i2c.chipWrite(pon | NAEN | wen, COMMAND_BIT | ENABLE);
    }
};
void FarbsensorTCS34725::setPon(bool pon)
{
    pon = pon;
    if(pon)
    {
        i2c.chipWrite(PON | aen | wen, COMMAND_BIT | ENABLE);
    }else
    {
        i2c.chipWrite(NPON | aen | wen, COMMAND_BIT | ENABLE);
    }
};
//setzt, ob die Wartefunktion aktiviert wird und setzt eine zeit
void FarbsensorTCS34725::setWen(bool wen, uint8_t wTime)
{
    setWen(wen);
    setWTime(wTime);
};