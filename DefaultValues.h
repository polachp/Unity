#ifndef DEFAULTVALUES_h
#define DEFAULTVALUES_h

#define SCHEMAVERSION 20160308

//Hardcoded defaults
#define SENSITIVITY_MIN 50  // mapped 30-100!!

#define SMOOTHING_DTE_MIN 50 // non mapped
#define SMOOTHING_DTE_MAX 150
#define SMOOTHING_DTE_MIN_AT 100
#define SMOOTHING_DTE_MAX_AT 1000
#define COMPENSATION 100

#define DEFAULT_LIFT_TRESHOLD 10
#define DEFAULT_SINK_TRESHOLD -250

#define SPEEDCALIBRATION_A 95
#define SPEEDCALIBRATION_B 0

//SOUND DEFINITIONS
#define ALARMS_VOLUME 10
#define LOWSOUNDVOLUME 9 // low sound volume. High sound volume uses 10 by default
#define DEFAULTSOUNDBASEFREQ 2700 // base sound frequency
#define DEFAULTLOWSOUNDBASEFREQ 500 // base sound frequency for low sound mode
#define DEFAULT_SOUND_RATE_MULTIPLIER 100 // sound beep speed coeff (use 0.5-1.5)
#define FREQ_RAISE 600 //frequency raises from basefreq up to basefreq + FREQ_RAISE on vario value 0-8
#define T1 150
#define MAX 500
#define X 400
#define T2 40
#define MID 120
#define T3 10
#define MIN 50

#define VARIODATASENDINTERVAL 500

#define SOUNDPIN1    9
#define SOUNDPIN2    10
#define SERIAL_SPEED 19200
#define I2C_MS5611_Add 0x77
#define I2C_4525_Add   0x28
#define SOUNDPIN    5
#define BUTTONPIN 2
#define I2C_MS5611_Add 0x77
#define I2C_4525_Add   0x28
#define PIN_LED        13 

#ifdef DEBUG
#define SERIAL_COMM_DELAY 50  // 12 seconds
#else
#define SERIAL_COMM_DELAY 12000  // 12 seconds
#endif


#endif

