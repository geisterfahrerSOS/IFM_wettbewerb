#include <Arduino.h>
#include <SimpleKalmanFilter.h>


float echerWert = 72;
float fehlerMessung = 4;

SimpleKalmanFilter kalmanFilter;

void setup() 
{
  Serial.begin(9600);
  kalmanFilter = SimpleKalmanFilter(1, fehlerMessung, 0);
   randomSeed(analogRead(0));
}

void loop() {
  Serial.println(kalmanFilter.calcKalmanFilter((float) random(echerWert - fehlerMessung, echerWert + fehlerMessung)));
}
