#include "FXS_Sounds.h"

Sounds::Sounds()
{ 
	SoundOn = true;
	Volume = 10;
	vario_climb_rate_start = CLIMBSOUNDTRESHOLD;
	vario_sink_rate_start = SINKSOUNDTRESHOLD;
	BaseFreq= DEFAULTSOUNDBASEFREQ;
}

void Sounds::Setup(int32_t climb_rate_start, int32_t sink_rate_start) {
	vario_climb_rate_start = climb_rate_start;
	vario_sink_rate_start = sink_rate_start;
}

void Sounds::SetSound(boolean enabled) {
	SoundOn = enabled;
	if (enabled) SoundUp();
	else 
	{
		SoundDn();
	}
}

//////////////////////////////////////
//VARIO SOUNDS
///////////////////////////////////////
unsigned long i = 0;
unsigned long freqShift = 0;
unsigned long startBeep = 0;
unsigned long endBeep = 0;

int32_t lastclimbrate=0;

void Sounds::VarioSound(int32_t climbRate) {
	_tempo = millis();
	basic(climbRate);
}
bool accSignalized= false;
void Sounds::basic(int32_t climbRate) {

	/*if ( climbRate <=100  )
	{
	toneAC(freq,Volume, 5000,true);

	return;
	}*/

	if (((_tempo - beep) > cadence) && SoundOn)                      // make some beep
	{
		startBeep = millis();
		beep = _tempo;
		if (climbRate >= vario_climb_rate_start)
		{
			i=0;
			freqShift=0;
			cadence = period(climbRate);
			endBeep = millis() + length(climbRate);
			freq = BaseFreq + ((climbRate*climbRate)-(vario_climb_rate_start*vario_climb_rate_start))/250; 
			if (freq > BaseFreq +400) 
				freq = BaseFreq+400;
			toneAC(freq,Volume, 5000,true);
		}
	}
	else
	{ 
		if (!SoundOn) return;

		if ( climbRate <= vario_sink_rate_start ) 
		{ 
			toneAC(lowSound(climbRate),Volume, 1000,true);
			return;
		}

		// this makes sound more "plastic"
		if (millis() < endBeep  && climbRate >= vario_climb_rate_start)
		{
			if ( climbRate <=100 && i%2==0)//100  )
			{
				freqShift=freqShift+1;
				i++;
			}
			freq = (BaseFreq + ((climbRate*climbRate)-(vario_climb_rate_start*vario_climb_rate_start))/250)+freqShift;
			toneAC(freq+freqShift,Volume, 5000,true);
		}
		else{
			noToneAC();
			accSignalized=true;
		}
	}
}

int Sounds::lowSound(int32_t climbRate) {
	int minLow = 30;
	int value = 150 + (0.20f * climbRate);
	if (value > minLow)
		return value;
	else
		return minLow;
}

int Sounds::period(int32_t climbRate) {
	int minPer = 140;
	int value =((1.5f/(climbRate+50))*40000)*0.9*DEFAULTSOUNDSPEEDFACTOR;
	if (value < minPer)
		return minPer;
	else
		return value*DEFAULTSOUNDSPEEDFACTOR;
}

int Sounds::length(int32_t climbRate) {
	int minLength = 60;
	int value =(1.0f/(climbRate+90))*45000*DEFAULTSOUNDSPEEDFACTOR; 
	if (value < minLength)
		return minLength;
	else
		return value*DEFAULTSOUNDSPEEDFACTOR;
}

//////////////////////////////////////
//PLAY SOUNDS
///////////////////////////////////////
void Sounds::Play(int note, int ms) {
	toneAC(note, Volume, ms, true);
}

void Sounds::SoundUp()                 //play only once welcome beep after turning on arduino vario
{
	for (int aa = 1000; aa <= 2600; aa = aa + 25)
	{
		toneAC( aa, 60);         // play beep on pin 9 (note,duration), it is louder if we move aplitude phase
		delay(4);
	}
	noToneAC();
}

void Sounds::SoundDn() {
	for (int freq = 2600; freq >= 1000; freq =freq - 25) {  
		toneAC(freq,60); 
		delay(4);    
	}
	noToneAC();
}

void Sounds::Click(int tone)                 //play only once welcome beep after turning on arduino vario
{
	Play(tone,25);
	delay(25);
	noToneAC();
}

void Sounds:: SoundUp2()                 //play only once welcome beep after turning on arduino vario
{
	for (int aa = 500; aa <= 2600; aa = aa + 80)
	{
		toneAC( aa, 60);         // play beep on pin 9 (note,duration), it is louder if we move aplitude phase
		delay(4);
	}
	noToneAC();
}

void Sounds::SoundDn2() {
	for (int freq = 2600; freq >= 500; freq =freq - 80) {  
		toneAC(freq,60); 
		delay(4);    
	}
	noToneAC();
}

void Sounds::Alarm(byte rings) {

	for (int i = 0; i < rings; i++)
	{
		for (int aa = 300; aa <= 1000; aa = aa + 100)
		{
			toneAC( aa, 60);         // play beep on pin 9 (note,duration), it is louder if we move aplitude phase
			delay(15);
		}
	}
	noToneAC();
}

void Sounds::Sonar(int repeat) 
{
	for (int i = 0; i < repeat; i++)
	{
		for (int aa = 50; aa <= 3500; aa = aa + 130)
		{
			toneAC( aa, 60);         // play beep on pin 9 (note,duration), it is louder if we move aplitude phase
			delay(1);
		}
		delay(100);
	}
	noToneAC();
}

void Sounds::PlayLKSound(int soundCode){
	switch (soundCode)
	{
	case 0:  SoundUp2();SoundUp2();SoundUp2();delay(1000);
		break;
	case 1:
		Click(200);
		break;
	case 2:
		Click(250);
		break;
	case 3:
		Click(300);
		break;
	case 4:
		Click(400);
		break;
	case 5:
		Click(500);
		break;
	case 6: 
		SoundUp();
		SoundDn();
		SoundUp();
		SoundDn();
		delay(1000);
		break;
	case 7:
		SoundUp(); 
		SoundDn();
		delay(800);
		break;
	case 8:
		Alarm(1);
		break;
	case 9:
		Alarm(2);
		break;
	case 10:
		Alarm(4);
		break;
	case 11:
		Alarm(5);
		break;
	case 12:
		SoundUp2();
		break;

	case 13:
		//Alarm(6);
		break;
	default:
		Alarm(1);
		break;
	}
}





