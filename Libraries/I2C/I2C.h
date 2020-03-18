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
        uint8_t multiplexerAdresse;
        uint8_t channel;
        bool activeMultiplexer;
        int LED;
        //if true LED is on
        bool permLED;
    public:
        //Konsturkor
        I2C();
        I2C(uint8_t aAdd);
        I2C(uint8_t aAdd, int aLed, bool aPermLED);
        //setzt die Adresse des Gerätes
        void setAdress(uint8_t aAdd);
        //gibt die Ardesse des Ger-tes zurück
        int getAdress();
        //gibt den PIN der LED an
        void setLED(int aLed);
        //ließt einen Sensor aus
        int readSensor(uint8_t aReg_lo, uint8_t aReg_hi, uint8_t aCTRL);
        //leißt einen Sensor aus
        int readSensor(uint8_t aReg_lo,uint8_t aReg_hi);
        //schreibt einen INT-Wert in zwei Register
        void chipWrite(uint8_t aReg_lo, uint8_t aReg_hi, int aVal);
        //schreibt einen BYTE-Wert in ein Register
        void chipWrite(uint8_t aVal, uint8_t aReg);
        //Ließt einen BYTE-Wert aus einem Register
        byte chipRead(uint8_t aReg, uint8_t aCTRL);
        //Ließt einen BYTE-Wert aus einem Register
        byte chipRead(uint8_t aReg);
        //Combiniert zwei BYTE-Werte zu einem INT-Wert
        int combineByte(byte aFront, byte aBack);
        //Gibt ein Register mit Registernummer und -wert aus
        void printRegister(byte aReg, uint8_t aCTRL);
        //teilt einen INT-Wert in zwei BYTE-Werte auf
        void splitByte(int aVal, byte *aLo, byte *aHi);
        //bestimmt, ob LED ausgeschaltet werden kann
        void setPermLED(bool aPermLED);
        //setzt den multiplexer
        void setMultiplexer(uint8_t add, uint8_t aChannel);
        //"löscht" den multiplexer
        void clearMultiplexer();
        //wechselt zu den richtigen Kanal
        int switchChannel();

};
#endif
