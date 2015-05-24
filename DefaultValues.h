#ifndef DEFAULTVALUES_h
#define DEFAULTVALUES_h
//Hardcoded defaults
#define SENSITIVITY_MIN 90
#define SMOOTHING_DTE_MIN 90
#define SMOOTHING_DTE_MAX 1000
#define SMOOTHING_DTE_MIN_AT 100
#define SMOOTHING_DTE_MAX_AT 1000

#define DEFAULT_LIFT_TRESHOLD 10
#define DEFAULT_SINK_TRESHOLD -200

#define SPEEDCALIBRATION_A 95
#define SPEEDCALIBRATION_B 0

#define LOWSOUNDVOLUME 9 // low sound volume. High sound volume uses 10 by default
#define DEFAULTSOUNDBASEFREQ 2750 // base sound frequency
#define DEFAULTLOWSOUNDBASEFREQ 600 // base sound frequency for low sound mode
#define DEFAULT_SOUND_RATE_MULTIPLIER 100 // sound beep speed coeff (use 0.5-1.5)
#define FREQ_RAISE 800 //frequency raises from basefreq up to basefreq + FREQ_RAISE on vario value 0-8

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


#endif