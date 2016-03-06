/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Pro or Pro Mini w/ ATmega328 (5V, 16 MHz), Platform=avr, Package=arduino
*/

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
#define __AVR_ATmega328p__
#define __AVR_ATmega328P__
#define ARDUINO 167
#define ARDUINO_MAIN
#define __AVR__
#define __avr__
#define F_CPU 16000000L
#define __cplusplus
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__

#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define __attribute__(x)
#define NOINLINE __attribute__((noinline))
#define prog_void
#define PGM_VOID_P int
            
typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}
#include "c:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino\arduino.h"
#include "c:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\eightanaloginputs\pins_arduino.h" 
#include "C:\_PROJECT\FlyXsense.ino"
#include "C:\_PROJECT\DefaultValues.h"
#include "C:\_PROJECT\FXS_Button.cpp"
#include "C:\_PROJECT\FXS_Button.h"
#include "C:\_PROJECT\FXS_CompensatedVario.h"
#include "C:\_PROJECT\FXS_EEPROMAnything.h"
#include "C:\_PROJECT\FXS_I2C.cpp"
#include "C:\_PROJECT\FXS_I2C.h"
#include "C:\_PROJECT\FXS_KalmanFilter.cpp"
#include "C:\_PROJECT\FXS_KalmanFilter.h"
#include "C:\_PROJECT\FXS_Sounds.cpp"
#include "C:\_PROJECT\FXS_compensatedVario.cpp"
#include "C:\_PROJECT\FXS_config.cpp"
#include "C:\_PROJECT\FXS_config.h"
#include "C:\_PROJECT\FXS_ms4525.cpp"
#include "C:\_PROJECT\FXS_ms4525.h"
#include "C:\_PROJECT\FXS_ms5611.cpp"
#include "C:\_PROJECT\FXS_ms5611.h"
#include "C:\_PROJECT\FXS_sounds.h"
#include "C:\_PROJECT\FXS_toneAC.cpp"
#include "C:\_PROJECT\FXS_toneAC.h"
#include "C:\_PROJECT\TinyGPS.cpp"
#include "C:\_PROJECT\TinyGPS.h"
#endif
