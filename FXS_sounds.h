#ifndef SOUNDS_h
#define SOUNDS_h

#include <Arduino.h>
#include "FXS_toneAC.h"
#include "DefaultValues.h"
#include <stdlib.h>

class Sounds
{
private:
	unsigned long _now;
	int lowSound(int32_t climbRate);
	int period(int32_t climbRate);
	int beepDuration(int32_t climbRate);
	int freq;
	unsigned long  beep;
	unsigned long  cadence;
	void SlideSound(int start,int high,int raiseStep,int delayTime);

	const byte T1 = 150;
	const int MAX = 500;
	const byte T2 = 40;
	const byte MID = 120;
	const byte T3 = 10;
	const byte MIN = 50;
	
	//#define T1 150
	//#define MAX 500
	//#define X 400
	//#define T2 40
	//#define MID 120
	//#define T3 10
	//#define MIN 50

public:
	int X;
	
	Sounds();
	boolean SoundOn;
	int BaseFreq;
	uint8_t Volume;
	void SetSound(boolean enabled);
	void VarioSound(int32_t climbRate);

	void SpecialPlay(int note, int ms);
	void Play(int note, int ms);

	void SoundUp();
	void SoundUp2();
	void SoundDn();
	void SoundDn2();
	void Alarm(byte rings);
	void Click(int tone);
	void Sonar(int tone);

	void PlayLKSound(int soundCode);
	void PlayBeeps(int note, int ms, int repeats, int pause);
};


#endif





















