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
#define F_CPU 16000000L
#define ARDUINO 10609
#define ARDUINO_AVR_PRO
#define ARDUINO_ARCH_AVR
#define __cplusplus 201103L
#define __AVR__
#define __inline__
#define __asm__(x)
#define __extension__
#define __inline__
#define __volatile__
#define GCC_VERSION 40801
#define volatile(va_arg) 
#define _CONST
#define __builtin_va_start
#define __builtin_va_end
#define __attribute__(x)
#define NOINLINE __attribute__((noinline))
#define prog_void
#define PGM_VOID_P int
#ifndef __builtin_constant_p
	#define __builtin_constant_p __attribute__((__const__))
#endif
#ifndef __builtin_strlen
	#define __builtin_strlen  __attribute__((__const__))
#endif
#define NEW_H
typedef void *__builtin_va_list;
typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}



#include <arduino.h>
#include <pins_arduino.h> 
#undef F
#define F(string_literal) ((const PROGMEM char *)(string_literal))
#undef PSTR
#define PSTR(string_literal) ((const PROGMEM char *)(string_literal))
#undef cli
#define cli()
#define pgm_read_byte(address_short)
#define pgm_read_word(address_short)
#define pgm_read_word2(address_short)
#define digitalPinToPort(P)
#define digitalPinToBitMask(P) 
#define digitalPinToTimer(P)
#define analogInPinToBit(P)
#define portOutputRegister(P)
#define portInputRegister(P)
#define portModeRegister(P)
#include <..\_Project\FlyXsense.ino>
#include <..\_Project\DefaultValues.h>
#include <..\_Project\FXS_Button.cpp>
#include <..\_Project\FXS_Button.h>
#include <..\_Project\FXS_CompensatedVario.h>
#include <..\_Project\FXS_EEPROMAnything.h>
#include <..\_Project\FXS_I2C.cpp>
#include <..\_Project\FXS_I2C.h>
#include <..\_Project\FXS_Sounds.cpp>
#include <..\_Project\FXS_compensatedVario.cpp>
#include <..\_Project\FXS_config.cpp>
#include <..\_Project\FXS_config.h>
#include <..\_Project\FXS_ms4525.cpp>
#include <..\_Project\FXS_ms4525.h>
#include <..\_Project\FXS_ms5611.cpp>
#include <..\_Project\FXS_ms5611.h>
#include <..\_Project\FXS_sounds.h>
#include <..\_Project\FXS_toneAC.cpp>
#include <..\_Project\FXS_toneAC.h>
#include <..\_Project\LowPower.cpp>
#include <..\_Project\LowPower.h>
#endif
