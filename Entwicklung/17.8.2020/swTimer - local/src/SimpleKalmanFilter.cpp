#include <Arduino.h>
#include <SimpleKalmanFilter.h>
SimpleKalmanFilter::SimpleKalmanFilter(){};
SimpleKalmanFilter::SimpleKalmanFilter(float aErrorEstimate, float aErrorMessurment, float aEstimate)
{
    errorEstimate = aErrorEstimate;
    errorMesurment = aErrorMessurment;
    estimate = aEstimate;
}


float SimpleKalmanFilter::calcKalmanFilter(float aMessurment)
{
  //Kalman Gain
  kalmanWeight = errorEstimate / (errorEstimate + errorMesurment);
  //neue schätzung
  estimate = estimate + kalmanWeight * (aMessurment - estimate);
  //fehler in der schätzung
  errorEstimate = (1.00 - kalmanWeight) * errorEstimate;
  
  
  return estimate;
};

float SimpleKalmanFilter::getEstimate()
{
    return estimate;
};