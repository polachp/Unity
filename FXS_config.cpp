#include "FXS_config.h"

extern bool Contains(String s, String search);
extern void SleepMode();

ConfigManager::ConfigManager()
{

}

void ConfigManager::Load()
{
	EEPROM_readAnything(0, data);
	if (data.SchemaVersion != SCHEMAVERSION )
	{
		SetDefaults();
	}
}
void ConfigManager::Save()
{
	EEPROM_writeAnything(0, data);
}

void ConfigManager::SetVarioMode(byte m)
{
	if (m == normal) 
	{
		data.VarioMode = normal;
		snd.SoundUp();
	}else{
		data.VarioMode = compensated;
		snd.SoundUp2();
		snd.SoundUp2();
	}
	Save();
}

int MapSensitivity(int sen)
{
	return map(sen, 0, 100, 30, 120);
}

void ConfigManager::LoadConfigToRuntime()
{
	Load();
	baro.varioData.sensitivity = MapSensitivity(data.Sensitivity);
	snd.SoundOn = data.SoundOn;
	snd.Volume = data.Volume;
	if (snd.Volume<10) snd.BaseFreq = data.LowBaseFreq; // in case of lower sount than max we use lower base freq mode
#ifdef AIRSPEED
	airspd.setCalibration(data.SpeedCalibrationA,data.SpeedCalibrationB);  
	SetVarioMode(data.VarioMode);
#endif
#ifdef ALLOWMODESWITCH
	SetVarioMode(data.VarioMode);
#else
	SetVarioMode(normal);
#endif
}

void ConfigManager::SetDefaults()
{
	data.SchemaVersion = SCHEMAVERSION;
	data.VarioMode = normal;
	data.SoundOn = true;
	data.BaseFreq = DEFAULTSOUNDBASEFREQ;
	data.LowBaseFreq = DEFAULTLOWSOUNDBASEFREQ;
	data.LowSoundVolume = LOWSOUNDVOLUME;
	data.AlarmsVolume= ALARMS_VOLUME;
	data.LiftTreshold = DEFAULT_LIFT_TRESHOLD;
	data.SinkTreshold = DEFAULT_SINK_TRESHOLD;
	data.Volume = LOWSOUNDVOLUME;
	data.RateMultiplier = DEFAULT_SOUND_RATE_MULTIPLIER;
	data.SpeedCalibrationA= SPEEDCALIBRATION_A;
	data.SpeedCalibrationB= SPEEDCALIBRATION_B;
	data.Sensitivity = 65;
	data.Compesation = COMPENSATION;
	EEPROM_writeAnything(0, data);
}

void ConfigManager::ProcessSetCommand(String sentence)
{
	int value = 0;
	if (Contains(sentence,"UNSAVE"))
	{
		Save();
		snd.Sonar(3);
	    return;
	}

	if (Contains(sentence,"UNRESET"))
	{
		SetDefaults();
		Save();
		return;
	}

	if (Contains(sentence,"UNLT"))
	{
		value = GetValue(sentence);
		data.LiftTreshold=abs(value);
		return;
	}

	if (Contains(sentence,"UNST"))
	{
		value = GetValue(sentence);
		data.SinkTreshold=abs(value)*-1;
		return;
	}

	if (Contains(sentence,"UNBF"))
	{
		value = GetValue(sentence);
		value = constrain(value,350,3000);
		data.BaseFreq=value;
		snd.BaseFreq=value;
		return;
	}

	if (Contains(sentence, "UNCMP"))
	{
		value = GetValue(sentence);
		data.Compesation = constrain(value, 80, 140);
		return;
	}


	if (Contains(sentence,"UNLBF"))
	{
		value = constrain(GetValue(sentence), 350, 3000);
		data.LowBaseFreq=value;
		snd.BaseFreq=value;
		return;
	}

	if (Contains(sentence,"UNLV"))
	{
		value = constrain(GetValue(sentence), 1, 9);
		data.LowSoundVolume =value;
		return;
	}

	if (Contains(sentence,"UNAV"))
	{
		value = constrain(GetValue(sentence), 9, 10);
		data.AlarmsVolume =value;
		return;
	}

	if (Contains(sentence,"UNBR"))
	{
		value = constrain(GetValue(sentence), 70, 130);
		data.RateMultiplier =value;
		return;
	}

	if (Contains(sentence,"UNSEN"))
	{
		value = constrain(GetValue(sentence), 0, 100);
		data.Sensitivity = value;
		baro.varioData.sensitivity = MapSensitivity(data.Sensitivity);;
		return;
	}

	if (Contains(sentence,"UNSLP"))
	{
	    SleepMode();
		return;
	}

	if (Contains(sentence,"UNSPA"))
	{
		value = constrain(GetValue(sentence),50,150);
		data.SpeedCalibrationA = value;
#ifdef AIRSPEED
		airspd.setCalibration(data.SpeedCalibrationA,data.SpeedCalibrationB);
#endif
		return;
	}

	if (Contains(sentence,"UNSPB"))
	{
		value = constrain(GetValue(sentence), -20, 20);
		data.SpeedCalibrationB = value;
#ifdef AIRSPEED
		airspd.setCalibration(data.SpeedCalibrationA,data.SpeedCalibrationB);
#endif
		return;
	}
	
	byte lastVolume;
	if (Contains(sentence, "UNSB")) //"$UNSB,1600,500,3,50*"  tone, ms,repeats, pause
	{
		lastVolume = snd.Volume;
		snd.Volume = data.AlarmsVolume;
		int a = sentence.indexOf(",");
		int aa = sentence.indexOf(",", a + 1);
		int tone = sentence.substring(a + 1, aa).toInt();
	
		a = sentence.indexOf(",", aa + 1);
		int ms = sentence.substring(aa + 1, a).toInt();
		
		aa = sentence.indexOf(",", a + 1);
		int rep = sentence.substring(a + 1, aa).toInt();

		a = sentence.indexOf("*", aa + 1);
		int pause = sentence.substring(aa + 1, a).toInt();

		snd.PlayBeeps(tone,ms,rep,pause);

		snd.Volume = lastVolume;
		return;
	}

	if (Contains(sentence,"UNS"))
	{
		lastVolume= snd.Volume;
		snd.Volume = data.AlarmsVolume;
		snd.PlayLKSound(GetValue(sentence));
		snd.Volume = lastVolume;
		return;
	}

	
}

int ConfigManager::GetValue(String s)
{
	int i = s.indexOf(",");
	int ii = s.indexOf("*");
	return s.substring(i+1,ii).toInt();
}

#ifdef PRINTCONFIG
void ConfigManager::Print(Stream &icf)
{
	icf.print("schema:");
	icf.println(data.SchemaVersion);

	icf.print("mode:");
	icf.println(data.VarioMode);

	icf.print("soundon:");
	icf.println(data.SoundOn);

	icf.print("basefreq:");
	icf.println(data.BaseFreq);

	icf.print("lowbasefreq:");
	icf.println(data.LowBaseFreq);

	icf.print("LowSoundVolume:");
	icf.println(data.LowSoundVolume);

	icf.print("Volume:");
	icf.println(data.Volume);

	icf.print("LiftTreshold:");
	icf.println(data.LiftTreshold);

	icf.print("SinkTreshold:");
	icf.println(data.SinkTreshold);

	icf.print("Beeprate:");
	icf.println(data.RateMultiplier);

	icf.print("Sensitivity:");
	icf.println(data.Sensitivity);

	icf.print("Compensation:");
	icf.println(data.Compesation);
	icf.print("SpeedCalibrationA:");
	icf.println(data.SpeedCalibrationA);
	icf.print("SpeedCalibrationB:");
	icf.println(data.SpeedCalibrationB);
}

#endif
