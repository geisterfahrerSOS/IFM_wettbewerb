//https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf
#ifndef FarbsensorTCS34725_h
#define FarbsensorTCS34725_h

#include <Arduino.h>
#include <I2C.h>
#include <Farbe.h>

#define ADDRESS 0x29
#define COMMAND_BIT 0x80 // Typ der Übertragung

#define ENABLE 0b00 //REG: aktiviert Interupts und Staten
#define AEN 0b10 //BIT: 1 - aktiviert RGBC
#define NAEN 0b00 //deaktiviert RGBC
#define PON 0b1 //Power ON
#define NPON 0b0 //Power off
#define WEN 0b1000 //Wait Time = true
#define NWEN 0b0000 //Wait Time = true

#define ATIME 0x01 //REG: RGBC zeit
#define INTEG_CYCLE_1 0xFF //dann MAX-Val: 1024
#define INTEG_CYCLE_10 0xF6 //dann MAX-Val: 10240
#define INTEG_CYCLE_42 0xD5 //dann MAX-Val: 43008
#define INTEG_CYCLE_64 0xC0 //dann MAX-Val: 65535
#define INTEG_CYCLE_256 0x00 //dann MAX-Val: 65535

#define WTIME 0x03 //REG: Warte Zeit
#define WTIME_2_4 0xFF //2.4 ms
#define WTIME_204 0xAB //204 ms
#define WTIME_614 0x00 //614 ms

#define CONFIG 0x0D //REG: Config
#define WLONG 0b10 // 0x10 //wait cycles are increased by a factor 12× 
#define NWLONG 0b00 // 0x10 //wait cycles are increased by a factor 12×

#define CONTROL 0x0F //Control
#define GAIN_1x 0b00
#define GAIN_4x 0b01
#define GAIN_16x 0b10
#define GAIN_60x 0b11

#define STATUS 0x13
#define AVALID 0b1
#define NVALID 0b0

#define CDATAL 0x14 //Clear data low byte
#define CDATAH 0x15 //Clear data high byte
#define RDATAL 0x16 //Red data low byte
#define RDATAH 0x17 //Red data high byte
#define GDATAL 0x18 //Green data low byte
#define GDATAH 0x19 //Green data high byte
#define BDATAL 0x1A //Blue data low byte
#define BDATAH 0x1B //Blue data high byte
/*
    Libary für den Farbsensor TCS34725
*/
typedef struct FS_Param_multiAdd_type
{
    uint8_t mulitAdd;
    uint8_t channel;
    int aLed;
    uint8_t aAGain;
    uint8_t aIntegCycle;
} FS_Param_multiAdd_t;
typedef struct FS_Param_singleAdd_type
{
    int aLed;
    uint8_t aWTime;
    bool aWLong;
    uint8_t aAGain;
    uint8_t aIntegCycle;
} FS_Param_singleAdd_t;
class FarbsensorTCS34725
{
    public:
        FarbsensorTCS34725();
        //Konstruktor - für Einmalmessung
        FarbsensorTCS34725(int aLed, uint8_t aAGain, uint8_t aIntegCycle);
        //mit Multiplexer
        FarbsensorTCS34725(FS_Param_multiAdd_t pParameter);
        //Konstrukor für Messung nach delay(wTime)
        FarbsensorTCS34725(FS_Param_singleAdd_t pParameter);
        //mit Multiplexer
        FarbsensorTCS34725(uint8_t mulitAdd, uint8_t channel, int aLed, uint8_t aWTime, bool aWLong, 
                            uint8_t aAGain, uint8_t aIntegCycle);
        
        
        //setzt den Integraion Cycle
        void setIntegCycle(uint8_t aIntegCycle);
        //setzt den Gain
        void setAGain(uint8_t aAGain);
        //setzt die Wartezeit
        void setWTime(uint8_t AWTime);
        //setzt, ob die wartezeit 12x so hoch ist
        void setWLong(bool aWLong);
        //setzt, ob die Wartefunktion aktiviert wird
        void setWen(bool aWen);
        //Setzt, ob die Farberkennung aktiv ist
        void setAEN(bool aAen);
        //setzt, ob die Wartefunktion aktiviert wird und setzt eine zeit
        void setWen(bool aWen, uint8_t aWTime);
        //Setzt, ob der Sensor in dem SLEEP- oder IDLE-Modus ist
        void setPon(bool aPon);
        //erkennt die Farbe
        int farbeErkennen();
        //setzt den Sensor in den IDLE-Modus
        void start();
        //setzt den Sensor in den SLEEP-Modus
        void stop();
    private:
        Farbe color;        //Objekt für die Farbe
        I2C i2c;            //I2C-Verbindung
        int maxVal;         //maximaler Wert der RGB Register
        uint8_t wTime;      //Länge der Wartezeit
        uint8_t aGain;      //Gain
        uint8_t integCycle; //menge der Zyklen bei der Messung
        bool wLong;         //bestimmt, ob die Wartezeit x12 genommen wird
        bool pon;           //wenn wahr, dann ist der Sensor aktiv
        bool wen;           //aktiviert die Warte-Funktion
        bool aen;           //RGBC-Feture aktiv
};
#endif