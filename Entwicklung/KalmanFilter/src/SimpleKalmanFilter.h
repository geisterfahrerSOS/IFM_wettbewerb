#ifndef SimpleKalmanFilter_h
#define SimpleKalmanFilter_h

class SimpleKalmanFilter
{
    private:
        float kalmanWeight;
        float errorEstimate;
        float errorMesurment;
        float estimate;
    public:
        SimpleKalmanFilter();
        SimpleKalmanFilter(float aErrorEstimate, float aErrorMessurment, float aEstimate);
        float getEstimate();
        float calcKalmanFilter(float aMessurment);
};
#endif