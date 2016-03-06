#include "FXS_KalmanFilter.h"

KalmanFilter::KalmanFilter(float q, float r) : q_(q), r_(r), p_(1.f), x_(-1.f), isFirstCall_(true)
{
}

KalmanFilter::KalmanFilter()  : q_(0.00001f), r_(0.01f), p_(1.f), x_(-1.f), isFirstCall_(true)
{
}

float KalmanFilter::update(float measurement) {
  float result = measurement;
  if (isFirstCall_) {
    isFirstCall_ = false;
    x_ = result;
  }
  else {

    measurementUpdate();
    result = x_ + (measurement - x_) * k_;
    x_ = result;
  }
  return result;
}

void KalmanFilter::measurementUpdate() {
  k_ = 1 - r_ / (p_ + q_ + r_);
  p_ = r_ * k_;
}





