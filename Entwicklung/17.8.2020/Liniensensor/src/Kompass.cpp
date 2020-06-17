#include <Arduino.h>
#include <Wire.h> //include Wire.h library
#include <Kompass.h>
#define ADDRESS1 0x1D //LSM303D Ultra compact high performance e-Compass \ 0x1E wenn SAO mit ground verbunden wird \3D accelerometer and 3D magnetometer module
#define ADDRESS2 0x6B //L3GD20H MEMS motion sensor: three-axis digital output gyroscope
//Magnetometer
#define OUT_X_L_M 0x08
#define OUT_X_H_M 0x09
#define OUT_Y_L_M 0x0A
#define OUT_Y_H_M 0x0B
#define OUT_Z_L_M 0x0C
#define OUT_Z_H_M 0x0D
#define CTRL1_M 0x20
#define CTRL7_M 0x26
//Accelerometer
#define OUT_X_L_A 0x28
#define OUT_X_H_A 0x29
#define OUT_Y_L_A 0x2A
#define OUT_Y_H_A 0x2B
#define OUT_Z_L_A 0x2C
#define OUT_Z_H_A 0x2D
//Gyrosensor
#define OUT_X_L_G 0x28
#define OUT_X_H_G 0x29
#define OUT_Y_L_G 0x2A
#define OUT_Y_H_G 0x2B
#define OUT_Z_L_G 0x2C
#define OUT_Z_H_G 0x2D
#define CTRL1_G 0x20
#define CTRL4_G 0x23

Winkel::Winkel() {}
//Inklination von Friedrichshafen liegt bei ca. 62 grad
//der Winkel ist zwischen der Erdoberfläche und den magnetischen Feldlinien die in den Boden eintauchen
void Winkel::startup()
{
    Wire.begin();
    chipWrite(ADDRESS1, CTRL7_M, 0x0);        //Magnetic Sensor to configure CTRL 5 default: 3,125 Hz
    chipWrite(ADDRESS1, CTRL1_M, 0b00010111); //3,125 Hz Acceleration Rate
    chipWrite(ADDRESS2, CTRL1_G, 0b00001111); //12,5 Hz Output Gyro
    chipWrite(ADDRESS2, CTRL4_G, 0b10110000); //Full Sclale 2000 dps (degrees per second)
    //perhaps add controls for STATUS of axis
    Serial.begin(9600);
}
/////////////////////////
//Magnetometer
float Winkel::getMagX()
{
    return calc(read16bit(ADDRESS1, OUT_X_H_M, OUT_X_L_M));
}

float Winkel::getMagY()
{
    return calc(read16bit(ADDRESS1, OUT_Y_H_M, OUT_Y_L_M));
}
//Maximaler Z-Wert bei -22 grad roll bei 2332 = 49 Gauss
float Winkel::getMagZ()
{
    return calc(read16bit(ADDRESS1, OUT_Z_H_M, OUT_Z_L_M));
}

float Winkel::getWinkel()
{
    float kompassWinkel = 0;

    kompassWinkel = 360 - (atan2(getMagX(), getMagY()) + 0.052) * 180 / PI;

    if (kompassWinkel < 0)
    {
        kompassWinkel += 360;
    }
    if (kompassWinkel > 360)
    {
        kompassWinkel -= 360;
    }
    // Serial.println(calc(read16bit(ADDRESS1, OUT_Y_H_M, OUT_Y_L_M)));
    kompassWinkel -= winkelOffset[(int)(kompassWinkel * 0.1)];
    return arrayManager(kompassWinkel);
}

///////////////////////////////
//Accelerometer
float Winkel::getAccelX()
{
    return _adjustedAccelX * (calc(read16bit(ADDRESS1, OUT_X_H_A, OUT_X_L_A)) + _offsetX);
}

float Winkel::getAccelY()
{
    return _adjustedAccelY * (calc(read16bit(ADDRESS1, OUT_Y_H_A, OUT_Y_L_A)) + _offsetY);
}

float Winkel::getAccelZ()
{
    return _adjustedAccelZ * (calc(read16bit(ADDRESS1, OUT_Z_H_A, OUT_Z_L_A)) + _offsetZ);
}

float Winkel::getPitch()
{
    float pitch = 0;
    pitch = 180 * atan(getAccelY() / sqrt(pow(getAccelX(), 2) + pow(getAccelZ(), 2))) / M_PI;
    return pitch;
}

float Winkel::getRoll()
{
    float roll = 0;
    roll = 180 * atan(getAccelX() / sqrt(pow(getAccelY(), 2) + pow(getAccelZ(), 2))) / M_PI;
    return roll;
}

float Winkel::getYaw()
{
    float yaw = 0;
    yaw = 180 * atan(getAccelZ() / sqrt(pow(getAccelX(), 2) + pow(getAccelZ(), 2))) / M_PI;
    return yaw;
}
/////////////////////////
//Gyrosensor
float Winkel::getGyroX()
{
    return calc(read16bit(ADDRESS2, OUT_X_H_G, OUT_X_L_G));
}
float Winkel::getGyroY()
{
    return calc(read16bit(ADDRESS2, OUT_Y_H_G, OUT_Y_L_G));
}
float Winkel::getGyroZ() //Angaben in Grad pro sekunde
{
    return (calc(read16bit(ADDRESS2, OUT_Z_H_G, OUT_Z_L_G)) + gyroOffsetZ) * gyroAdjustZ; //0.054 adjust value; 5.86 offsetZ
}
float Winkel::gyroBerechnung()
{
    if (state)
    {
        gyroGesWinkelRAW += data;
        data = getGyroZ();
        zahl++;
        return -1 * gyroGesWinkelRAW * (millis() - initialTimeCompare) / zahl * 0.001;
    }
    else
    {
        data = getGyroZ();
        zahl = 0;
        gyroGesWinkelRAW = 0;
        timeDelta = 0;
        initialTimeCompare = millis();
        state = true;
    }
    return 0;
}

void Winkel::fixPassivDrift()
{
    float minDrift; //gyroOffsetZ bester Wert
    float offset = 1000; //kleinstes offset / 1000 -> unwahrschinlich, dass es einen größeren Wert gibt
    float tmpOffset;
    
    //float gyroOffsetZ = 5.8559; 
    for(float i = 5.85; i <= 5.9; i = i + 0.0001)
    {
        state = false;
        gyroOffsetZ = i;
        gyroBerechnung();
        delay(100);
        tmpOffset = gyroBerechnung();
        if(offset > tmpOffset || offset > -tmpOffset)
        {
            offset = tmpOffset;
            minDrift = i;
        }
        Serial.println(minDrift);
    }
    gyroOffsetZ = minDrift;
    Serial.println(minDrift);
};



// else if (state == 2)
// {
//     // timeDelta = micros() - timeCompare;
//     // Serial.print("timeDelta:  ");
//     // Serial.println(timeDelta);
//     Serial.print("gyroOffsetZ:  ");
//     Serial.println(gyroOffsetZ,4);

//     Serial.print("Zeit pro Durchlauf (ms):  ");
//     Serial.println((millis() - initialTimeCompare) / zahl);

//     Serial.print("Ergebnis RAW:  ");
//     Serial.println(gyroGesWinkelRAW / zahl);

//     Serial.print("WinkelGesamt mit Average RAW:  ");
//     Serial.println(gyroGesWinkelRAW * (millis() - initialTimeCompare) / zahl * 0.001,4);
//     state = 0;
// }

//größter und kleinster Wert entfernen und dann den Durchschnitt der übrigen Werte ausgeben
float Winkel::arrayManager(float input)
{
    bool upper = false;
    for (int i = 1; i < ANZAHL; i++)
    {
        winkelArray[i - 1] = winkelArray[i];
    }
    winkelArray[4] = input;
    for (int i = 0; i < ANZAHL; i++) //prüfen ob die Werte beidseitig des 0||360 Punkts vorkommen
    {
        if (winkelArray[i] >= 270 && winkelArray[i] < 360)
        {
            upper = true;
        }
        if (upper && winkelArray[i] >= 0 && winkelArray[i] < 90)
        {
            winkelArray[i] += 360;
        }
    }
    int min = 0;
    int max = 0;
    for (int i = 0; i < ANZAHL; i++)
    {
        if (winkelArray[i] >= winkelArray[max])
        {
            max = i;
        }
        if (winkelArray[i] <= winkelArray[min])
        {
            min = i;
        }
    }
    float gesamt = 0;
    for (int i = 0; i < ANZAHL; i++)
    {
        if ((i != min) && (i != max)) //evtl. if(i!=(min||max))
        {
            gesamt += winkelArray[i];
        }
    }

    float winkelBerech = gesamt / (ANZAHL - 2);

    if (winkelBerech < 0)
    {
        winkelBerech += 360;
    }
    if (winkelBerech > 360)
    {
        winkelBerech -= 360;
    }
    return winkelBerech;
}
float Winkel::calc(float mag)
{
    float magnetCalc = 0;

    if (mag > 32768)
    {
        magnetCalc = (mag - 65536);
    }
    else
    {
        magnetCalc = mag;
    }
    return magnetCalc;
}
uint8_t Winkel::chipRead(uint8_t add, byte reg)
{
    Wire.beginTransmission(add);
    Wire.write(byte(reg));
    Wire.endTransmission();

    Wire.requestFrom(add, 1);
    while (!Wire.available())
    {
    }

    return Wire.read();
}
uint16_t Winkel::read16bit(uint8_t add, byte regh, byte regl)
{
    uint8_t low;
    uint8_t high;
    uint16_t readWert;

    Wire.beginTransmission(add);
    Wire.write(byte(regh));
    Wire.endTransmission();

    Wire.requestFrom(add, 1);
    while (!Wire.available())
    {
    }

    high = Wire.read();

    readWert = high;
    readWert = readWert << 8;

    Wire.beginTransmission(add);
    Wire.write(byte(regl));
    Wire.endTransmission();

    Wire.requestFrom(add, 1);
    while (!Wire.available())
    {
    }
    low = Wire.read();

    readWert |= low;
    return readWert;
}

void Winkel::chipWrite(uint8_t add, uint8_t reg, uint8_t data)
{
    Wire.beginTransmission(add);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}
