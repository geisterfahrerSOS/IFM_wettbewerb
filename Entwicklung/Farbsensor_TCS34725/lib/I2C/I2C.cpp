#include <I2C.h>

//Konsturkor
I2C::I2C()
{
    Wire.begin();
};
//Konsturkor
I2C::I2C(uint8_t add)
{
    address = add;
    Wire.begin();
};
//Konsturkor
I2C::I2C(uint8_t add, int led)
{
    address = add;
    LED = led;
    pinMode(LED, OUTPUT);
    Wire.begin();
};
//setzt die Adresse des Gerätes
void I2C::setAdress(uint8_t add)
{
    address = add;
};
//gibt die Ardesse des Gerätes zurück
int I2C::getAdress()
{
    return address;
};
//ließt einen Sensor aus
int I2C::readSensor(uint8_t reg_lo, uint8_t reg_hi, uint8_t CTRL)
{
    //LED an
    digitalWrite(LED, HIGH); 
    delay(10);
    //Sensorwert
    int result = combineByte(chipRead(reg_hi, CTRL), chipRead(reg_lo, CTRL));
    //LED aus
    digitalWrite(LED, LOW);
    //Ausgabe des Sensorwerts
    // Serial.print("0x");
    // Serial.print(reg_hi, HEX);
    // Serial.print(" : ");
    // Serial.println(result);
    //gibt den Wert zurück
    return result;
};
int I2C::readSensor(uint8_t reg_lo, uint8_t reg_hi)
{
    //LED an
    digitalWrite(LED, HIGH);
    delay(10);
    //Sensorwert
    int result = combineByte(chipRead(reg_hi), chipRead(reg_lo));
    //LED aus
    digitalWrite(LED, LOW);
    //Ausgabe des Sensorwerts
    // Serial.print("0x");
    // Serial.print(reg_hi, HEX);
    // Serial.print(" : ");
    // Serial.println(result);
    //gibt den Wert zurück
    return result;
};
//schreibt einen INT-Wert in zwei Register
void I2C::chipWrite(uint8_t reg_lo, uint8_t reg_hi, int val)
{
    //kreirt zwei Pointer
    byte *lo = new byte, *hi = new byte;
    //teilt den INT-Wert
    splitByte(1023, lo, hi);
    //schreibt die Werte in die Register
    chipWrite(*lo, reg_lo);
    chipWrite(*hi, reg_hi);
    //löscht die Pointer
    delete lo;
    delete hi;
};
//schreibt einen BYTE-Wert in ein Register
void I2C::chipWrite(uint8_t val, uint8_t reg)
{
    //Adresse des Gerätes
    Wire.beginTransmission(address);
    //Register
    Wire.write(reg);
    //Wert
    Wire.write(val);
    //beenden
    Wire.endTransmission();
};
//Ließt einen BYTE-Wert aus einem Register
byte I2C::chipRead(uint8_t reg, uint8_t CTRL)
{
    //damit werte aus dem Register "reg" ausgelesen werden können muss 0x01 in das Controllregister geschrieben werden
    chipWrite(0x01, CTRL);
    byte ctrl_val = 1;
    while(ctrl_val != 0)
    {
        //Geräte addresse
        Wire.beginTransmission(address);
        //Register addresse
        Wire.write(CTRL);
        Wire.endTransmission();
        //lesen eines registers
        Wire.requestFrom(address, uint8_t(1));
        while (!Wire.available());
        //auslesen des registers
        ctrl_val = Wire.read();
    }
    //auslesen aus dem Register "reg"
    //Geräte addresse
    Wire.beginTransmission(address);
    //Register addresse
    Wire.write(reg);
    Wire.endTransmission();

    //lesen eines registers
    Wire.requestFrom(address, uint8_t(1));
    while (!Wire.available());
    
    //auslesen des registers
    return Wire.read();
};
byte I2C::chipRead(uint8_t reg)
{
    //Geräte addresse
    Wire.beginTransmission(address);
    //Register addresse
    Wire.write(reg);
    Wire.endTransmission();

    //lesen eines registers
    Wire.requestFrom(address, uint8_t(1));
    while (!Wire.available());

    //auslesen des registers
    return Wire.read();
};
//Combiniert zwei BYTE-Werte zu einem INT-Wert
int I2C::combineByte(byte front, byte back)
{
    int result = front;
    //schiebt den vorderen Teil um 8 Bits nach links
    result <<= 8;
    //fügt den hinteren Teil ein
    result |= back;
    return result;
};
//Gibt ein Register mit Registernummer und -wert aus
void I2C::printRegister(byte reg, uint8_t CTRL)
{
    Serial.print("0x");
    Serial.print(reg, HEX);
    Serial.print(" : ");
    Serial.print(chipRead(reg, CTRL));
};
//teilt einen INT-Wert in zwei BYTE-Werte auf
void I2C::splitByte(int val, byte *lo, byte *hi)
{
    //vordere 8-Bits
    int toSplit = val;
    toSplit >>= 8;
    *lo = toSplit;
    //hintere 8-Bits
    val <<= 8;
    val >>= 8;
    *hi = val;  
};
void I2C::setLED(int led)
{
    LED = led;
    pinMode(LED, OUTPUT);
};
