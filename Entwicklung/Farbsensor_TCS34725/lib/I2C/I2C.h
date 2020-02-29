#ifndef I2C_h
#define I2C_h
#include <Arduino.h>
#include <Wire.h>
/*
    stellt eine Verbindung mit meheren Geräten her
*/
class I2C
{
    private:
        //speichert die Adresse des Gerätes
        uint8_t address;
        int LED;
    public:
        //Konsturkor
        I2C();
        I2C(uint8_t add);
        I2C(uint8_t add, int led);
        //setzt die Adresse des Gerätes
        void setAdress(uint8_t add);
        //gibt die Ardesse des Ger-tes zurück
        int getAdress();
        //gibt den PIN der LED an
        void setLED(int led);
        //ließt einen Sensor aus
        int readSensor(uint8_t reg_lo, uint8_t reg_hi, uint8_t CTRL);
        //leißt einen Sensor aus
        int readSensor(uint8_t reg_lo,uint8_t reg_hi);
        //schreibt einen INT-Wert in zwei Register
        void chipWrite(uint8_t reg_lo, uint8_t reg_hi, int val);
        //schreibt einen BYTE-Wert in ein Register
        void chipWrite(uint8_t val, uint8_t reg);
        //Ließt einen BYTE-Wert aus einem Register
        byte chipRead(uint8_t reg, uint8_t CTRL);
        //Ließt einen BYTE-Wert aus einem Register
        byte chipRead(uint8_t reg);
        //Combiniert zwei BYTE-Werte zu einem INT-Wert
        int combineByte(byte front, byte back);
        //Gibt ein Register mit Registernummer und -wert aus
        void printRegister(byte reg, uint8_t CTRL);
        //teilt einen INT-Wert in zwei BYTE-Werte auf
        void splitByte(int val, byte *lo, byte *hi);
};
#endif
