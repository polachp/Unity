#ifndef FXS_KALMANFILTER_h
#define FXS_KALMANFILTER_h

class KalmanFilter
{
  private:
    float q_;
    float r_;
    float p_;
    float x_;
    float k_;
    bool  isFirstCall_;
    void measurementUpdate();

  public:
    KalmanFilter(float q, float r);
    KalmanFilter();
    float update(float measurement);
};


#endif //KalmanFilter_h



