#ifndef Kompass_h
#define Kompass_h

#define ANZAHL 5

#include <Arduino.h>
class Winkel
{
public:
    Winkel();
    void startup();

    float getMagX();
    float getMagY();
    float getMagZ();
    float getWinkel();

    float getAccelX();
    float getAccelY();
    float getAccelZ();
    float getPitch();
    float getRoll();
    float getYaw();

    float getGyroX();
    float getGyroY();
    float getGyroZ();

	void fixPassivDrift();
	
    float gyroBerechnung();
    void zuruecksetzenGyro();//setzt die ausgegebenen Werte aud null zurück

    float calc(float mag);
    float arrayManager(float input);

    uint8_t chipRead(uint8_t add, byte reg);
    uint16_t read16bit(uint8_t add, byte regh, byte regl);
    void chipWrite(uint8_t add, uint8_t reg, uint8_t data);

    float winkelArray[5] = {0, 0, 0, 0, 0};
    //10 Grad abschnitt
    int winkelOffset[36] = {0, -4, -6, -8, -10, -10, -12, -12, -14, -14, -14, -13, -12, -9, -5, 0, 3, 9, 13, 18, 21 ,25, 24, 24, 24, 24, 22, 22, 22, 22, 17, 10, 7, 3, 1};

    float _adjustedAccelX = 0.0006164829242;
    float _adjustedAccelY = 0.0006061240612;
    float _adjustedAccelZ = 0.0005910496516;

    float _offsetX = -150;
    float _offsetY = -200;
    float _offsetZ = -850;

    float gyroOffsetZ = 5.8559;
    float gyroAdjustZ = 0.068;
    float gyroGesWinkel = 0;
    float gyroGesWinkelRAW = 0;
    float gyroGesSpeed = 0;
    float gesAufrufe = 0;
    float data = 0;


    long timeCompare = 0;
    long initialTimeCompare = 0;
    long timeDelta = 0;
    bool state = false; //wenn false werden die Werte zurückgestzt die messung wird also bei null geeicht
    float zahl = 0;

private:
};

#endif