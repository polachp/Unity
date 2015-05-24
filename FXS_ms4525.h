#ifndef FXS_MS4525_h
#define FXS_MS4525_h

#include <Arduino.h>
//#include "FXS_config.h"
#include "FXS_I2C.h"
#include "FXS_KalmanFilter.h"


struct AIRSPEEDDATA {
  //bool available;          // true if data is available
  float smoothDifPressureAdc ;  // in steps ADC
  float temperature4525;     // in Kelvin
  float rawAirSpeed;       // cm/sec
  int32_t airSpeed; // in cm/sec (no decimal)
  int32_t airSpeedSM; // in cm/sec (no decimal)
  bool airSpeedAvailable ;
  int32_t compensation ; // in km/h (no decimal)
  bool compensationAvailable ;
  bool airspeedReset ;

  unsigned long lastCommand2Micros ; // used to avoid some task (reading voltage sensor, currentsensor, ..) when barometric data should be soon available for reading
  // = micro divided by 2 to avoid overflow on comparison
  float difPressureAdc_zero ;

};

extern float actualPressure ;

class MS4525 {
  public:
	MS4525(uint8_t addr);
    AIRSPEEDDATA airSpeedData ;
    void setup();
	void setCalibration(int a,int b);
    void readSensor();
	float calib_A;
	float calib_B;
  private:
	
    KalmanFilter   kalmanAirspeedSM;
    void SendCommand(byte command);
    uint8_t _addr;
    uint8_t I2CErrorCode4525 ;
    long getData(byte command, byte del);
    bool calibrated4525 ;
    int calibrateCount4525 ;
    int32_t difPressureSum ;
    float smoothAirSpeed;
    uint8_t data[4];
    int32_t difPressureAdc;          // in steps ADC
    int32_t temperature4525Adc ;   // in steps ADC

    float offset4525 ;
    float difPressureAdc_0 ;
    float abs_deltaDifPressureAdc ;

    float expoSmooth4525_adc_auto ;
   
    unsigned long  airSpeedMillis ; //save time when airspeed is made available
    unsigned long  nextAirSpeedMillis ; //next time that airspeed must be available

#ifdef DEBUG
    HardwareSerial* printer;
#endif
}; // end class MS4525

//extern int32_t test1Value ;// used in order to test the transmission of any value
//extern bool test1ValueAvailable ;
//extern int32_t test2Value ;// used in order to test the transmission of any value
//extern bool test2ValueAvailable ;
//extern int32_t test3Value ;// used in order to test the transmission of any value
//extern bool test3ValueAvailable ;

#endif // MS4525



