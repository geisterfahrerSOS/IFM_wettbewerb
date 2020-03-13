//https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf	//https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf

#include <Arduino.h>
#include <Farbe.h>
#include <I2C.h>	
I2C i2c;	


#define WEN 0b1000 //0b1000//BIT: 3 - This bit activates the wait feature. Writing a 1 activates the wait timer. Writing a 0 disables the wait timer.	
#define NWEN 0b000	
#define AEN 0b10 //0b10//BIT: 1 - RGBC enable. This bit actives the two-channel ADC. Writing a 1 activates the RGBC. Writing a 0 disables the RGBC.	
#define NAEN 0b00	
#define PON 0b1 //0b1//BIT: 0 - Power ON. This bit activates the internal oscillator to permit the timers and ADC channels to operate.	
#define NPON 0b0	

#define ATIME 0x01 //RGBC time	
#define INTEG_CYCLE_1 0xFF //MAX: 1024	
#define INTEG_CYCLE_10 0xF6 //MAX: 10240	
#define INTEG_CYCLE_42 0xD5 //MAX: 43008	
#define INTEG_CYCLE_64 0xC0 //MAX: 65535	
#define INTEG_CYCLE_256 0x00 //MAX: 65535	

#define WTIME 0x03 //Wait time IF	
#define WTIME_2_4 0xFF	
#define WTIME_204 0xAB	
#define WTIME_614 0x00	
uint8_t wTime = WTIME_2_4;//RESET: 0xFF	

#define CONFIG 0x0D //Configuration	
#define WLONG 0x10 // 0x10 //BITS: 1 - When asserted, the wait cycles are increased by a factor 12× from that programmed in the WTIME register	

#define CONTROL 0x0F //Control	
#define AGAIN 0x01 //BITS: 1:0	
#define GAIN_1x 0x00	
#define GAIN_4x 0x01	
#define GAIN_16x 0x10	
#define GAIN_60x 0x11	
#define STATUS 0x13	
/*	
BIT: 0 - RGBC Valid. Indicates that the RGBC channels have completed an integration cycle	
*/	

#define CDATAL 0x14 //Clear data low byte	
#define CDATAH 0x15 //Clear data high byte	
#define RDATAL 0x16 //Red data low byte	
#define RDATAH 0x17 //Red data high byte	
#define GDATAL 0x18 //Green data low byte	
#define GDATAH 0x19 //Green data high byte	
#define BDATAL 0x1A //Blue data low byte	
#define BDATAH 0x1B //Blue data high byte	


//NICHT BENÖTIGT:	
// AILTL 0x04 //Clear interrupt low threshold low byte	
// AILTH 0x05 //Clear interrupt low threshold high byte	
// AIHTL 0x06 //Clear interrupt high threshold low byte	
// AIHTH 0x07 //Clear interrupt high threshold high byte	
// PERS 0x0C //Interrupt persistence filter	


/*
Part number identification	Part number identification
0x44 = TCS34721 and TCS34725	0x44 = TCS34721 and TCS34725
SLEEP --> I2C Start --> IDLE --> Power Managment Feture enabled && RGBC Feture e
                             --> !Power Managment Feture enabled && RGBC Feture enabled? --> RGBC Init --> RGBC ADC --> IDLE --> !PON?? --> SLEEP	                             --> !Power Managment Feture enabled && RGBC Feture enabled? --> RGBC Init --> RGBC ADC --> IDLE --> !PON?? --> SLEEP
                                                                                                                             --> PON?? --> IDLE  	                                                                                                                             --> PON?? --> IDLE  
*/
int integrationTime = GAIN_4x;
int aGain = INTEG_CYCLE_10;	
void setup() {
  Serial.begin(9600);
  i2c = I2C(0x00);
  //POWER ON	
  i2c.chipWrite(PON, COMMAND_BIT | ENABLE);	
  //RGBC-READ CLEAR	
  //i2c.chipWrite(AEN, COMMAND_BIT | ENABLE);	
  i2c.chipWrite(aGain, COMMAND_BIT | AGAIN);	
  i2c.chipWrite(integrationTime, COMMAND_BIT | ATIME);	
  if(WEN == 0x1000)	
  {	
    i2c.chipWrite(AEN, COMMAND_BIT | ENABLE);	
    i2c.chipWrite(PON, COMMAND_BIT | ENABLE);	
    i2c.chipWrite(WEN, COMMAND_BIT | ENABLE);	
    i2c.chipWrite(wTime, COMMAND_BIT | WTIME);	
  }	
}


void loop() {
  Farbe farbe;
  long maxVal;	  
  /*	  
  snsor.farbeErkennen();
    Wenn nur einmal die Werte ausgelesen werden,	
     soll gewartet werden, bis fertig gemessen wurde,	
     sonst soll sofot ausgelesen werden, da es bis zu 7,4 sec dauern kann	
  */	
  if(WEN != 0x1000)	
  {	
    i2c.chipWrite(AEN, COMMAND_BIT | ENABLE);	
    delay(3);	
    //wait reading is done	
    uint8_t result;    	
    do	
    {	
      result = i2c.chipRead(STATUS);	
      result = result & 0b0;	
    }	
    while(result ==1);	
    i2c.chipWrite(NAEN, COMMAND_BIT | ENABLE);	
  }	
  switch(integrationTime)	
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
  farbe.setColor(map(i2c.readSensor(RDATAL, RDATAH), 0, maxVal, 0, 255),	
                 map(i2c.readSensor(GDATAL, GDATAH), 0, maxVal, 0, 255),	
                 map(i2c.readSensor(BDATAL, BDATAH), 0, maxVal, 0, 255),	
                 map(i2c.readSensor(CDATAL, CDATAH), 0, maxVal, 0, 255)	
  );	
  farbe.checkColor();	
  farbe.printColor();	
  delay(1000);
}