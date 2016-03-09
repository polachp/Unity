#ifndef FXS_MS5611_h
#define FXS_MS5611_h

#include "DefaultValues.h"
#include "FXS_I2C.h"

struct VARIODATA {
  int32_t temperature;     // in 1/100 Celsius
  int64_t rawPressure ;  // in 1/10000 mBar so = Pa * 10000
  byte SensorState ;
  int32_t rawAltitude ; // in cm * 100
  int32_t absoluteAlt;     // in cm  * 100
  bool absoluteAltAvailable ;
  bool altitudeAt20MsecAvailable   ;  // use to say to readsensors() that an altitude is available and that dte can be calculated.
  float delaySmooth ; // smoothed delay between 2 altitude calculations

  int32_t climbRate;       // in cm /sec = vertical speed
  int32_t climbRateSM;
  bool climbRateAvailable ;
  bool switchClimbRateAvailable ; // use to say to the readsensors loop that that a climbrate is available (to select the one being send)

 // int sensitivityPpm ;      // sensivity to apply when PPM is used. Value has to be divided by 1000 in order to calculate the smoothing parameter
  int sensitivity ;
  unsigned long lastCommand2Micros ; // used to avoid some task (reading voltage sensor, currentsensor, ..) when barometric data should be soon available for reading
  // = micro divided by 2 to avoid overflow on comparison
};

class MS5611 {
  public:
    MS5611(uint8_t addr);
    VARIODATA varioData ;
    void setup();
    void readSensor();
    void resetValues();

  private:
   // KalmanFilter  kalmanVario;
    long result ;
    uint8_t _addr;
    unsigned int _calibrationData[7]; // The factory calibration data of the ms5611
    uint8_t errorI2C ; //error code returned by I2C::Write and I2C::Read; 0 = no error
    bool errorCalibration ; // (true = error)
    void SendCommand(byte command);
    long getData(byte command, byte del);
    unsigned int low, high;
    int32_t D1 ;
    int32_t D2 ;
    int32_t D2Prev ;
    int32_t D2Apply ;
    int64_t dT  ;
    int32_t TEMP  ;
    int64_t OFF, SENS;

    int32_t altitude  ; // in cm * 100
    int32_t altitudeLowPass  ;
    int32_t altitudeHighPass  ;
    
    unsigned long extended2Micros ; // used to temporarilly save micros() >> 1
    unsigned long pressureMicros ; // save time when program send command asking the MS5611 to get the pressure
    unsigned long pressureMicrosPrev1 ; // save the previous pressureMicros
    unsigned long pressureMicrosPrev2 ; // save the previous of the previous pressureMicros
    
    float climbRate2AltFloat  ;
    float climbRateFloat  ;
//    float abs_deltaClimbRate ;

}; // end class MS5611


#endif // MS5611






