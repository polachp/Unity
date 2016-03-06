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
	int32_t vario_climb_rate_start;
	int32_t vario_sink_rate_start ;
	void SlideSound(int start,int high,int raiseStep,int delayTime);

public:
	boolean continous;
	Sounds();
	boolean SoundOn;
	int BaseFreq;
	uint8_t Volume;
	void Setup(int32_t climb_rate_start, int32_t sink_rate_start);
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





















