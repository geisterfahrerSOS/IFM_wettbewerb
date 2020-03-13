#include <Farbsensor.h>
//definiert die Register
#define CTRL 0x00
#define CONFIG 0x01

#define CAP_RED 0x06
#define INT_RED_LO 0x0A
#define INT_RED_HI 0x0B
#define DATA_RED_LO 0x40
#define DATA_RED_HI 0x41

#define CAP_GREEN 0x07
#define INT_GREEN_LO 0x0C
#define INT_GREEN_HI 0x0D
#define DATA_GREEN_LO 0x42
#define DATA_GREEN_HI 0x43

#define CAP_BLUE 0x08
#define INT_BLUE_LO 0x0E
#define INT_BLUE_HI 0x0F
#define DATA_BLUE_LO 0x44
#define DATA_BLUE_HI 0x45

#define CAP_CLEAR 0x09
#define INT_CLEAR_LO 0x10
#define INT_CLEAR_HI 0x11
#define DATA_CLEAR_LO 0x46
#define DATA_CLEAR_HI 0x47

//Konstruktor
Farbsensor::Farbsensor(){};
//Konstruktor
Farbsensor::Farbsensor(int led)
{
    //kreirt ein Objekt Farbe
    color = Farbe();
    //setzt die Addresse des Farbsensors
    i2c = I2C(0x74, led);

    //setzt die Werte zur Messung
    i2c.chipWrite(0x0E, CAP_RED); // 0x08
    i2c.chipWrite(0x0F, CAP_GREEN); // 0x0C
    i2c.chipWrite(0x06, CAP_BLUE); // 0x05
    i2c.chipWrite(0x0F, CAP_CLEAR); // 0x0A
    i2c.chipWrite(0x2C, INT_RED_LO);
    i2c.chipWrite(0x01, INT_RED_HI);
    i2c.chipWrite(0x2C, INT_GREEN_LO);
    i2c.chipWrite(0x01, INT_GREEN_HI);
    i2c.chipWrite(0x2C, INT_BLUE_LO);
    i2c.chipWrite(0x01, INT_BLUE_HI);
    i2c.chipWrite(0xC8, INT_CLEAR_LO);
    i2c.chipWrite(0x00, INT_CLEAR_HI);
};
//messen der Farbe
int Farbsensor::farbeErkennen()
{
    //setzt die Farbe
    //maped sie zwischen 0 und 255
    color.setColor( map(i2c.readSensor(DATA_RED_LO, DATA_RED_HI, CTRL)     , 0, 1024, 0, 255),
                    map(i2c.readSensor(DATA_GREEN_LO, DATA_GREEN_HI, CTRL) , 0, 1024, 0, 255),
                    map(i2c.readSensor(DATA_BLUE_LO, DATA_BLUE_HI, CTRL)   , 0, 1024, 0, 255),
                    map(i2c.readSensor(DATA_CLEAR_LO, DATA_CLEAR_HI, CTRL) , 0, 1024, 0, 255) 
    );
    //updatet die Farbe
    color.checkColor();
    //gibt die Farbe aus
    color.printColor();
    //gibt die Farbe zurück
    return color.getColor();
};
//Ließt den Sensor