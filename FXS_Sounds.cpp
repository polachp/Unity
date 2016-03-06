#include "FXS_Sounds.h"
#include "FXS_config.h"
extern ConfigManager config;

Sounds::Sounds()
{
	SoundOn = true;
	Volume = 10;
	vario_climb_rate_start = DEFAULT_LIFT_TRESHOLD;
	vario_sink_rate_start = DEFAULT_SINK_TRESHOLD;
	BaseFreq = DEFAULTSOUNDBASEFREQ;
	continous = false;
}

void Sounds::Setup(int32_t climb_rate_start, int32_t sink_rate_start) {
	vario_climb_rate_start = climb_rate_start;
	vario_sink_rate_start = sink_rate_start;
}

void Sounds::SetSound(boolean enabled) {
	SoundOn = enabled;
	if (enabled) SoundUp();
	else
	  SoundDn();
	
}

//////////////////////////////////////
//VARIO SOUNDS
///////////////////////////////////////
unsigned int i = 0;
unsigned int freqShift = 0;
unsigned long endBeep = 0;

void Sounds::VarioSound(int32_t climbRate) {
	_now = millis();

	if (((_now - beep) > cadence) && SoundOn)
	{
		beep = _now;
		if (climbRate >= vario_climb_rate_start)
		{
			i = 0;
			freqShift = -100;
			cadence = period(climbRate);
			endBeep = millis() + beepDuration(climbRate);
		}
	}

	if (!SoundOn) return;
	if (climbRate <= vario_sink_rate_start)
	{
		freq = map(climbRate, -800, vario_climb_rate_start, 60, 220);
		toneAC(freq, Volume, 1000, true);
		return;
	}

	if (millis() < endBeep  && climbRate >= vario_climb_rate_start)
	{

		if (climbRate < 100 && i < 250)
		{
			i++;
			if (i % 30 == 0)
				freqShift = freqShift + 1;
		}

		freq = map(climbRate, vario_climb_rate_start, 800, BaseFreq, BaseFreq + FREQ_RAISE);
		freq = constrain(freq, 200, BaseFreq + FREQ_RAISE + 100);
		toneAC(freq + freqShift, Volume, 5000, true);
	}
	else{
		noToneAC();
	}
}



int Sounds::period(int32_t climbRate) {
	 int value;
	 float mod = (config.data.RateMultiplier / 100.0f); // speed modifier
	if (climbRate < X)
	{
		value = map(climbRate, vario_climb_rate_start, X, (MAX + T1)*mod, (MID + T2)*mod);
	}
	else
	{
		value = map(climbRate, X, 800, (MID + T2)*mod, MIN + T3);
		if (value < MIN + T3)
			return MIN + T3;
		else
			return value;
	}
	return value;
}

int Sounds::beepDuration(int32_t climbRate) {
	 int value;
	 float mod = (config.data.RateMultiplier / 100.0f);
	if (climbRate < X)
	{
		return value = map(climbRate, vario_climb_rate_start, X, MAX*mod, MID);
	}
	else
	{
		value = map(climbRate, X, 800, MID*mod, MIN);
		if (value < MIN)
			return MIN;
		else
			return value;
	}

}

//////////////////////////////////////
//PLAY SOUNDS
///////////////////////////////////////
void Sounds::Play(int note, int ms) {
	toneAC(note, Volume, ms, true);
}

void Sounds::PlayBeeps(int note, int ms, int repeats, int pause){
	repeats = constrain(repeats, 1, 100);
	 int starttime = millis();
	for (int i = 1; i <= repeats; i++)
	{
		toneAC(note, Volume, constrain(ms, 1, 1000), false);
		delay(constrain(pause, 0, 300));
		if (abs(millis() - starttime) > 4000) return;
	}
}

void Sounds::SlideSound(int start, int high, int raiseStep, int delayTime){
	if (start < high)
	{
		for (int aa = start; aa <= high; aa = aa + raiseStep)
		{
			toneAC(aa, Volume, 5000, true);
			delay(delayTime);
		}
	}
	else{
		for (int aa = high; aa >= start; aa = aa - raiseStep)
		{
			toneAC(aa, Volume, 5000, true);
			delay(delayTime);
		}
	}
	noToneAC();
}

void Sounds::SoundUp()                 //play only once welcome beep after turning on arduino vario
{
	for (int aa = 1000; aa <= 3000; aa = aa + 25)
	{
		toneAC(aa, Volume, 60, true);         // play beep on pin 9 (note,duration), it is louder if we move aplitude phase
		delay(4);
	}
	noToneAC();
}

void Sounds::SoundDn() {
	for (int freq = 3000; freq >= 1000; freq = freq - 25) {
		toneAC(freq, Volume, 60, true);
		delay(4);
	}
	noToneAC();
}

void Sounds::Click(int tone)                 //play only once welcome beep after turning on arduino vario
{
	toneAC(tone, Volume, 25, true);
	delay(25);
	noToneAC();
}

void TaskStart(){
	toneAC(2600, 10, 500, true);
	delay(800);
	toneAC(2600, 10, 500, true);
	delay(800);
	toneAC(2000, 10, 1000, true);
	delay(1000);
}

void Sounds::SoundUp2()                 //play only once welcome beep after turning on arduino vario
{
	for (int aa = 1000; aa <= 3100; aa = aa + 80)
	{
		toneAC(aa, Volume, 60, true);         // play beep on pin 9 (note,duration), it is louder if we move aplitude phase
		delay(4);
	}
	noToneAC();
}

void Sounds::SoundDn2() {
	for (int freq = 3100; freq >= 1000; freq = freq - 80) {
		toneAC(freq, Volume, 60, true);
		delay(4);
	}
	noToneAC();
}

void Sounds::Alarm(byte rings) {

	for (int i = 0; i < rings; i++)
	{
		for (int aa = 2000; aa <= 2700; aa = aa + 100)
		{
			toneAC(aa, Volume, 60, true);
			delay(15);
		}
	}
	noToneAC();
}

void Sounds::Sonar(int level)
{
	int freq = 2400 + (level * 80);
	toneAC(freq, Volume, 100, true); delay(80 + (level * 15));
	SlideSound(freq, 4000, 15, 1);
}

void Sounds::PlayLKSound(int soundCode){
	switch (soundCode)
	{
	case 0:  SoundUp2(); SoundUp2(); SoundUp2(); delay(1000);
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
		TaskStart();
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

