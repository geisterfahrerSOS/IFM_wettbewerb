#include <I2C.h>

//Konsturkor
I2C::I2C()
{
    Wire.begin();
};
//Konsturkor
I2C::I2C(uint8_t aAdd)
{
    address = aAdd;
    Wire.begin();
};
//Konsturkor
I2C::I2C(uint8_t aAdd, int aLed, bool aPermLED)
{
    permLED = aPermLED;
    address = aAdd;
    LED = aLed;
    pinMode(LED, OUTPUT);
    Wire.begin();
};
//setzt die Adresse des Gerätes
void I2C::setAdress(uint8_t aAdd)
{
    address = aAdd;
};
//gibt die Ardesse des Gerätes zurück
int I2C::getAdress()
{
    return address;
};
//ließt einen Sensor aus
int I2C::readSensor(uint8_t aReg_lo, uint8_t aReg_hi, uint8_t aCTRL)
{
    //LED an
    digitalWrite(LED, HIGH); 
    delay(10);
    //Sensorwert
    int result = combineByte(chipRead(aReg_hi, aCTRL), chipRead(aReg_lo, aCTRL));
    //LED aus
    if(!permLED)
    {
        digitalWrite(LED, LOW);
    }
    //Ausgabe des Sensorwerts
    // Serial.print("0x");
    // Serial.print(reg_hi, HEX);
    // Serial.print(" : ");
    // Serial.println(result);
    //gibt den Wert zurück
    return result;
};
int I2C::readSensor(uint8_t aReg_lo, uint8_t aReg_hi)
{
    //LED an
    digitalWrite(LED, HIGH);
    delay(10);
    //Sensorwert
    int result = combineByte(chipRead(aReg_hi), chipRead(aReg_lo));
    //LED aus
    if(!permLED)
    {
        digitalWrite(LED, LOW);
    }
    //Ausgabe des Sensorwerts
    // Serial.print("0x");
    // Serial.print(reg_hi, HEX);
    // Serial.print(" : ");
    // Serial.println(result);
    //gibt den Wert zurück
    return result;
};
//schreibt einen INT-Wert in zwei Register
void I2C::chipWrite(uint8_t aReg_lo, uint8_t aReg_hi, int aVal)
{
    //kreirt zwei Pointer
    byte *lo = new byte, *hi = new byte;
    //teilt den INT-Wert
    splitByte(aVal, lo, hi);
    //schreibt die Werte in die Register
    chipWrite(*lo, aReg_lo);
    chipWrite(*hi, aReg_hi);
    //löscht die Pointer
    delete lo;
    delete hi;
};
//schreibt einen BYTE-Wert in ein Register
void I2C::chipWrite(uint8_t aVal, uint8_t aReg)
{
    //Adresse des Gerätes
    Wire.beginTransmission(address);
    //Register
    Wire.write(aReg);
    //Wert
    Wire.write(aVal);
    //beenden
    Wire.endTransmission();
};
//Ließt einen BYTE-Wert aus einem Register
byte I2C::chipRead(uint8_t aReg, uint8_t aCTRL)
{
    //damit werte aus dem Register "reg" ausgelesen werden können muss 0x01 in das Controllregister geschrieben werden
    chipWrite(0x01, aCTRL);
    byte ctrl_val = 1;
    while(ctrl_val != 0)
    {
        //Geräte addresse
        Wire.beginTransmission(address);
        //Register addresse
        Wire.write(aCTRL);
        Wire.endTransmission();
        //lesen eines registers
        Wire.requestFrom(address, uint8_t(8));
        while (!Wire.available());
        //auslesen des registers
        ctrl_val = Wire.read();
    }
    //auslesen aus dem Register "reg"
    //Geräte addresse
    Wire.beginTransmission(address);
    //Register addresse
    Wire.write(aReg);
    Wire.endTransmission();

    //lesen eines registers
    Wire.requestFrom(address, uint8_t(1));
    while (!Wire.available());
    
    //auslesen des registers
    return Wire.read();
};
byte I2C::chipRead(uint8_t aReg)
{
    //Geräte addresse
    Wire.beginTransmission(address);
    //Register addresse
    Wire.write(aReg);
    Wire.endTransmission();
    //lesen eines registers
    Wire.requestFrom(address, uint8_t(1));
    while (!Wire.available());
    //auslesen des registers
    return Wire.read();
};
//Combiniert zwei BYTE-Werte zu einem INT-Wert
int I2C::combineByte(byte aFront, byte aBack)
{
    int result = aFront;
    //schiebt den vorderen Teil um 8 Bits nach links
    result <<= 8;
    //fügt den hinteren Teil ein
    result |= aBack;
    return result;
};
//Gibt ein Register mit Registernummer und -wert aus
void I2C::printRegister(byte aReg, uint8_t aCTRL)
{
    Serial.print("0x");
    Serial.print(aReg, HEX);
    Serial.print(" : ");
    Serial.print(chipRead(aReg, aCTRL));
};
//teilt einen INT-Wert in zwei BYTE-Werte auf
void I2C::splitByte(int aVal, byte *aLo, byte *aHi)
{
    //vordere 8-Bits
    int toSplit = aVal;
    toSplit >>= 8;
    *aLo = toSplit;
    //hintere 8-Bits
    aVal <<= 8;
    aVal >>= 8;
    *aHi = aVal;  
};
void I2C::setLED(int aLed)
{
    LED = aLed;
    pinMode(LED, OUTPUT);
};
void I2C::setPermLED(bool aPermLED)
{
    permLED = aPermLED;
};