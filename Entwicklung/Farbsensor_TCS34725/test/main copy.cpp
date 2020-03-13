//https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf
#include <Arduino.h>
//#include <FarbsensorTCS34725.h>
#include <I2C.h>
/*
Part number identification
0x44 = TCS34721 and TCS34725
0x4D = TCS34723 and TCS34727

SLEEP --> I2C Start --> IDLE --> Power Managment Feture enabled && RGBC Feture enabled? --> SLEEP - delay(x) --> RGBC Init --> RGBC ADC --> IDLE --> !PON?? --> SLEEP
                                                                                                                                                 --> PON?? --> IDLE                    
                             --> !Power Managment Feture enabled && RGBC Feture enabled? --> RGBC Init --> RGBC ADC --> IDLE --> !PON?? --> SLEEP
                                                                                                                             --> PON?? --> IDLE  
*/
//FarbsensorTCS34725 sensor;
I2C i2c;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  i2c = I2C(0x29, 13);
  i2c.chipWrite(0b00011 , 0x80);
  i2c.chipWrite(0b01,  0x80 | 0x0F);	
  i2c.chipWrite(0xf6, 0x80 | 0x01);	
  //sensor = FarbsensorTCS34725(0x29, 13, GAIN_4x, INTEG_CYCLE_10);
}

void loop()
{
    i2c.chipWrite(0b11, 0x80);		
    //wait reading is done	
    uint8_t result;    	
    do	
    {	
      result = i2c.chipRead(0x13);	
      result = result & 0b1;	
      //Serial.println(i2c.chipRead(0x13));
      //Serial.print("Result: "); Serial.println(result);
    }	
    while(result == 0);	
    delay(10);
    int red = i2c.readSensor(0x80 | 0x16, 0x80 | 0x17);
    int green = i2c.readSensor(0x80 | 0x18, 0x80 | 0x19);
    int blue = i2c.readSensor(0x80 | 0x1A, 0x80 | 0x1B);
    red = map(red, 0, 1024, 0, 255);
    green = map(green, 0, 1024, 0, 255);
    blue = map(blue, 0, 1024, 0, 255);

    Serial.println(red);
    Serial.println(green);
    Serial.println(blue);


  //???
  i2c.chipWrite(0b01, 0x80);
  Serial.println();
  //Serial.print("Farbe: ");
  //sensor.farbeErkennen();
  delay(1000);
}
