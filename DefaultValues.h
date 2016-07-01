#ifndef DEFAULTVALUES_h
#define DEFAULTVALUES_h

#define SCHEMAVERSION 20

//Hardcoded defaults
#define SENSITIVITY_MIN 35  // mapped 30-100!!

#define SMOOTHING_DTE_MIN 35 // non mapped
#define SMOOTHING_DTE_MAX 80
#define SMOOTHING_DTE_MIN_AT 100
#define SMOOTHING_DTE_MAX_AT 1000
#define COMPENSATION 100

#define DEFAULT_LIFT_TRESHOLD 15
#define DEFAULT_SINK_TRESHOLD -250

#define SPEEDCALIBRATION_A 95
#define SPEEDCALIBRATION_B 0

//SOUND DEFINITIONS
#define ALARMS_VOLUME 10
#define LOWSOUNDVOLUME 9 // low sound volume. High sound volume uses 10 by default
#define DEFAULTSOUNDBASEFREQ 2700 // base sound frequency
#define DEFAULTLOWSOUNDBASEFREQ 550 // base sound frequency for low sound mode
#define DEFAULT_SOUND_RATE_MULTIPLIER 100 // sound beep speed coeff (use 0.5-1.5)
#define FREQ_RAISE 600 //frequency raises from basefreq up to basefreq + FREQ_RAISE on vario value 0-8

#define VARIODATASENDINTERVAL 1000

#define SOUNDPIN1    9
#define SOUNDPIN2    10
#define SERIAL_SPEED 9600 //38400
#define I2C_MS5611_Add 0x77
#define I2C_4525_Add   0x28
#define SOUNDPIN    5
#define BUTTONPIN 2
#define I2C_MS5611_Add 0x77
#define I2C_4525_Add   0x28
#define PIN_LED        13 

#define SOFT_RXPIN 7
#define SOFT_TXPIN 8 
#ifdef DEBUG
#define SERIAL_COMM_DELAY 50  // 12 seconds
#else
#define SERIAL_COMM_DELAY 12000  // 12 seconds
#endif


#endif

