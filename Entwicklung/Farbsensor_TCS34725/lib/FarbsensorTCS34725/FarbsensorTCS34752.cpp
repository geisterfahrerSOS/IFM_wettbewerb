#include <FarbsensorTCS34725.h>
//PON | AEN ???


//Konstruktor
FarbsensorTCS34725::FarbsensorTCS34725(){};
//Konstruktor - für Einmalmessung
FarbsensorTCS34725::FarbsensorTCS34725(uint8_t aAdd, int aLed, uint8_t aAGain, uint8_t aIntegCycle)
{
    //I2C-Verbindung aufbauen
    i2c = I2C(aAdd, aLed, false);
    //Sensor hochfahren
    start();
    //Gain setzen
    setAGain(aAGain);
    //Integration Time setzen 
    setIntegCycle(aIntegCycle);
    //Wait time setzen(minimum)
    setWTime(WTIME_2_4);
    //12x wTime nicht aktiv 
    setWLong(false);
    //keine Farberkennung
    setAEN(true);
    //Wartezeit-funktion aus
    setWen(false);
    //Sensor hochfahren
};
//Konstrukor für Messung nach delay(wTime)
FarbsensorTCS34725::FarbsensorTCS34725(uint8_t aAdd, int aLed, uint8_t aWTime, bool aWLong, uint8_t aAGain, uint8_t aIntegCycle)
{
    //I2C-Verbindung aufbauen
    i2c = I2C(aAdd, aLed, true);
    //Sensor hochfahren
    start();
    //Gain setzen
    setAGain(aAGain);
    //Integration Time setzen 
    setIntegCycle(aIntegCycle);
    //Wait time setzen
    setWTime(aWTime);
    //Wartezeit-funktion ein
    setWen(true);
    //Farberkennung
    setAEN(true);
    //Idle-Modus
    //setPon(true);
    //12x wTime
    setWLong(aWLong);
    Serial.println(i2c.chipRead(0x80), BIN);
};
//setzt den Sensor in den IDLE-Modus
void FarbsensorTCS34725::start()
{
    //Sensor hochfahren
    setPon(true);
};
//setzt den Sensor in den SLEEP-Modus
void FarbsensorTCS34725::stop()
{
    //Sensor herunterfahren
    setPon(false);
};
//erkennt die Farbe
int FarbsensorTCS34725::farbeErkennen()
{
    //wenn Zyclenmessung nicht aktiv
    if(!wen)
    {
        Serial.println("NOT WEN");
        uint8_t result;    
        //Warten, bis Farbmessung beendet
        do
        {
            result = i2c.chipRead(STATUS);
            result = result & 0b1;
        }
        while(result == 0);
    }
    //Farbe lesen
    color.setColor(map(i2c.readSensor(RDATAL | COMMAND_BIT, RDATAH | COMMAND_BIT), 0, maxVal, 0, 255),
                   map(i2c.readSensor(GDATAL | COMMAND_BIT, GDATAH | COMMAND_BIT), 0, maxVal, 0, 255),
                   map(i2c.readSensor(BDATAL | COMMAND_BIT, BDATAH | COMMAND_BIT), 0, maxVal, 0, 255),
                   map(i2c.readSensor(CDATAL | COMMAND_BIT, CDATAH | COMMAND_BIT), 0, maxVal, 0, 255)
    );
    //Farbe überprüfen
    color.checkColor();
    //Farbe ausgeben
    color.printColor();
    //Farbe zurückgeben
    return color.getColor();
};
//setzt den Integraion Cycle
void FarbsensorTCS34725::setIntegCycle(uint8_t aIntegCycle)
{
    integCycle = aIntegCycle;
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
void FarbsensorTCS34725::setAGain(uint8_t aAGain)
{
    aGain = aAGain;
    i2c.chipWrite(aGain, COMMAND_BIT | CONTROL);
};
//setzt die Wartezeit
void FarbsensorTCS34725::setWTime(uint8_t AWTime)
{
    wTime = AWTime;
    i2c.chipWrite(wTime, COMMAND_BIT | WTIME);
};
//setzt, ob die wartezeit 12x so hoch ist
void FarbsensorTCS34725::setWLong(bool aWLong)
{
    wLong = aWLong;
    if(wLong)
    {
        i2c.chipWrite(WLONG, COMMAND_BIT | CONFIG);
    }else
    {
        i2c.chipWrite(NWLONG, COMMAND_BIT | CONFIG);
    }
    
};
//setzt, ob die Wartefunktion aktiviert wird
void FarbsensorTCS34725::setWen(bool aWen)
{
    wen = aWen;
    byte tmpAen, tmpPon;
    if(aen){tmpAen = AEN;}else{tmpAen = NAEN;}
    if(pon){tmpPon = PON;}else{tmpPon = NPON;}
    if(wen)
    {
        i2c.chipWrite(WEN | tmpAen | tmpPon, COMMAND_BIT | ENABLE);
        delayMicroseconds(2400);
    }else
    {
        i2c.chipWrite(NWEN | tmpAen | tmpPon, COMMAND_BIT | ENABLE);
    }
};
//Setzt, ob die Farberkennung aktiv ist
void FarbsensorTCS34725::setAEN(bool aAen)
{
    aen = aAen;
    byte tmpWen, tmpPon;
    if(wen){tmpWen = WEN;}else{tmpWen = NWEN;}
    if(pon){tmpPon = PON;}else{tmpPon = NPON;}
    if(aen)
    {
        i2c.chipWrite(tmpWen | AEN | tmpPon, COMMAND_BIT | ENABLE);
        delayMicroseconds(2400);
    }
    else
    {
        i2c.chipWrite(tmpWen | NAEN | tmpPon, COMMAND_BIT | ENABLE);
    }
};
void FarbsensorTCS34725::setPon(bool aPon)
{
    pon = aPon;
    byte tmpWen, tmpAen;
    if(wen){tmpWen = WEN;}else{tmpWen = NWEN;}
    if(aen){tmpAen = AEN;}else{tmpAen = NAEN;}
    if(pon)
    {
        i2c.chipWrite(tmpWen | tmpAen | PON, COMMAND_BIT | ENABLE);
        delayMicroseconds(2400);
    }else
    {
        i2c.chipWrite(tmpWen | tmpAen | NPON, COMMAND_BIT | ENABLE);
    }
};
//setzt, ob die Wartefunktion aktiviert wird und setzt eine zeit
void FarbsensorTCS34725::setWen(bool aWen, uint8_t aWTime)
{
    setWen(aWen);
    setWTime(aWTime);
};