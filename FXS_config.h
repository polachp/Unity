#ifndef FXS_CONFIG_h
#define FXS_CONFIG_h

#define SCHEMAVERSION 280415

struct Configuration
{
  int SchemaVersion;
  int Mode;
  int VarioClimbRateStart;
  int VarioSinkRateStart;
  int Volume;
  bool SoundOn;
  int BaseFreq;
  float SpeedCalibrationA;
  float SpeedCalibrationB;
};

#define VARIO
#define GPS
#define AIRSPEED

//Hardcoded defaults
#define SENSITIVITY_MIN 90
#define SENSITIVITY_MAX 150
#define SMOOTHING_DTE_MIN 100
#define SMOOTHING_DTE_MAX 1000
#define SMOOTHING_DTE_MIN_AT 100
#define SMOOTHING_DTE_MAX_AT 1000

#define CLIMBSOUNDTRESHOLD    15
#define CLIMBOFFSOUNDTRESHOLD 5
#define SINKSOUNDTRESHOLD    -200

#define SPEEDCALIBRATION_A 0.95
#define SPEEDCALIBRATION_B 0

#define LOWSOUNDVOLUME 8
#define LOWSOUNVOLUMEFREQMULTIPLIER 0.3
#define DEFAULTSOUNDBASEFREQ 2600 //550
#define DEFAULTSOUNDSPEEDFACTOR 1 //550

#define VARIODATASENDINTERVAL 500

#define SOUNDPIN1    9
#define SOUNDPIN2    10
#define SERIAL_SPEED 9600
#define I2C_MS5611_Add 0x77
#define I2C_4525_Add   0x28
#define SOUNDPIN    5
#define BUTTONPIN 2
#define I2C_MS5611_Add 0x77
#define I2C_4525_Add   0x28
#define PIN_LED        13 


#include "HardwareSerial.h"
#endif 



