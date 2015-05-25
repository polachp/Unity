#include "FXS_Sounds.h"
#include "FXS_config.h"
extern ConfigManager config;

Sounds::Sounds()
{ 
	SoundOn = true;
	Volume = 10;
	vario_climb_rate_start = DEFAULT_LIFT_TRESHOLD;
	vario_sink_rate_start = DEFAULT_SINK_TRESHOLD;
	BaseFreq= DEFAULTSOUNDBASEFREQ;
}

void Sounds::Setup(int32_t climb_rate_start, int32_t sink_rate_start) {
	vario_climb_rate_start = climb_rate_start;
	vario_sink_rate_start = sink_rate_start;
	CompensationFreqVarioStartRate = 	CompensationFreqVarioStartRate * vario_climb_rate_start*vario_climb_rate_start;
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

void Sounds::VarioSound(int32_t climbRate) {
	basic(climbRate);
}

void Sounds::basic(int32_t climbRate) {
	_now = millis();
	/*if ( climbRate <=100  )
	{
	toneAC(freq,Volume, 5000,true);
	return;
	}*/

	if (((_now - beep) > cadence) && SoundOn)   
	{
		startBeep = millis();
		beep = _now;
		if (climbRate >= vario_climb_rate_start)
		{
			i=0;
			freqShift=0;
			cadence = period(climbRate);
			endBeep = millis() + length(climbRate);
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
			i++;
			if (climbRate <=100 && i%6==0)
			{
				freqShift=freqShift+1;
			}
			freq= map(climbRate,0,800,BaseFreq,BaseFreq+FREQ_RAISE);
			toneAC(freq+freqShift,Volume, 5000,true);
		}
		else{
			noToneAC();
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
	int minPer = 70;
	int value =((1.3f/(climbRate+50))*40000)*(config.data.RateMultiplier/100.0f);
	if (value < minPer)
		return minPer;
	else
		return value;
}

int minLength = 50;
int Sounds::length(int32_t climbRate) {

	int value =(1.5f/(climbRate+80))*30000*(config.data.RateMultiplier/100.0f); 
	if (value < minLength)
		return minLength;
	else
		return value;
}

//////////////////////////////////////
//PLAY SOUNDS
///////////////////////////////////////
void Sounds::Play(int note, int ms) {
	toneAC(note, Volume, ms, true);
}

void Sounds::SlideSound(int start,int high,int raiseStep,int delayTime){
	if (start<high)
	{
		for (int aa = start; aa <= high ; aa = aa + raiseStep)
		{
			toneAC( aa,Volume , 5000,true);      
			delay(delayTime);
		}
	}else{
		for (int aa = high; aa >= start ; aa = aa - raiseStep)
		{
			toneAC( aa,Volume , 5000,true);         
			delay(delayTime);
		}
	}
	noToneAC();
}

void Sounds::SoundUp()                 //play only once welcome beep after turning on arduino vario
{
	for (int aa = 1000; aa <= 3000; aa = aa + 25)
	{
		toneAC( aa,Volume, 60,true);         // play beep on pin 9 (note,duration), it is louder if we move aplitude phase
		delay(4);
	}
	noToneAC();
}

void Sounds::SoundDn() {
	for (int freq = 3000; freq >= 1000; freq =freq - 25) {  
		toneAC(freq,Volume,60,true); 
		delay(4);    
	}
	noToneAC();
}

void Sounds::Click(int tone)                 //play only once welcome beep after turning on arduino vario
{
	toneAC(tone,Volume,25,true);
	delay(25);
	noToneAC();
}

void Sounds:: SoundUp2()                 //play only once welcome beep after turning on arduino vario
{
	for (int aa = 1000; aa <= 3100; aa = aa + 80)
	{
		toneAC( aa,Volume,60,true);         // play beep on pin 9 (note,duration), it is louder if we move aplitude phase
		delay(4);
	}
	noToneAC();
}

void Sounds::SoundDn2() {
	for (int freq = 3100; freq >= 1000; freq =freq - 80) {  
		toneAC(freq,Volume,60,true); 
		delay(4);    
	}
	noToneAC();
}

void Sounds::Alarm(byte rings) {

	for (int i = 0; i < rings; i++)
	{
		for (int aa = 2000; aa <= 2700; aa = aa + 100)
		{
			toneAC(aa,Volume,60,true);     
			delay(15);
		}
	}
	noToneAC();
}

void Sounds::Sonar(int level) 
{  
	int freq = 2400+(level*80);
	toneAC( freq,Volume,100,true);delay(80+(level*15));
	SlideSound( freq, 4000, 15, 1);
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
		Sonar(1);
		break;
	case 14:
		Sonar(2);
		break;
	case 15:
		Sonar(3);
		break;
	case 16:
		Sonar(4);
		break;
	case 17:
		Sonar(5);
		break;

	default:
		Alarm(1);
		break;
	}
}
