#include "FXS_config.h"

extern bool Contains(String s, String search);

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
		snd.SoundUp2();
	}else{
		data.VarioMode = compensated;
		snd.SoundUp2();
		snd.SoundUp2();
	}
	Save();
}

void ConfigManager::LoadConfigToRuntime()
{
	Load();
	baro.varioData.sensitivity = map(data.Sensitivity,0,100,30,140);
	snd.Setup(data.LiftTreshold,data.SinkTreshold);   
	snd.SoundOn = data.SoundOn;
	snd.Volume = data.Volume;
	if (snd.Volume<10) snd.BaseFreq = data.LowBaseFreq; // in case of lower sount than max we use lower base freq mode
#ifdef AIRSPEED
	airspd.setCalibration(data.SpeedCalibrationA,data.SpeedCalibrationB);  
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
	data.Sensitivity = 50;
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
		snd.SoundUp2();
		snd.SoundUp2();
		snd.SoundUp2(); 
		snd.SoundUp2();
		return;
	}

	if (Contains(sentence,"UNLT"))
	{
		value = GetValue(sentence);
		data.LiftTreshold=abs(value);
		snd.Setup(data.LiftTreshold,data.SinkTreshold);   
		return;
	}

	if (Contains(sentence,"UNST"))
	{
		value = GetValue(sentence);
		data.SinkTreshold=abs(value)*-1;
		snd.Setup(data.LiftTreshold,data.SinkTreshold);   
		return;
	}

	if (Contains(sentence,"UNBF"))
	{
		value = GetValue(sentence);
		value = constrain(value,500,3000);
		data.BaseFreq=value;
		snd.BaseFreq=value;
		return;
	}

	if (Contains(sentence,"UNLBF"))
	{
		value = GetValue(sentence);
		value = constrain(value,500,3000);
		data.LowBaseFreq=value;
		snd.BaseFreq=value;
		return;
	}

	if (Contains(sentence,"UNLV"))
	{
		value = GetValue(sentence);
		value = constrain(value,1,9);
		data.LowSoundVolume =value;
		return;
	}

	if (Contains(sentence,"UNAV"))
	{
		value = GetValue(sentence);
		value = constrain(value,9,10);
		data.AlarmsVolume =value;
		return;
	}

	if (Contains(sentence,"UNBR"))
	{
		value = GetValue(sentence);
		value = constrain(value,70,130);
		data.RateMultiplier =value;
		return;
	}

	if (Contains(sentence,"UNSEN"))
	{
		value = GetValue(sentence);
		value = constrain(value,0,100);
		data.Sensitivity = value;
		baro.varioData.sensitivity = map(value,0,100,30,140);
		return;
	}

	if (Contains(sentence,"UNSPA"))
	{
		value = GetValue(sentence);
		data.SpeedCalibrationA = value;
#ifdef AIRSPEED
		airspd.setCalibration(data.SpeedCalibrationA,data.SpeedCalibrationB);
#endif
		return;
	}

	if (Contains(sentence,"UNSPB"))
	{
		value = GetValue(sentence);
		data.SpeedCalibrationB = value;
#ifdef AIRSPEED
		airspd.setCalibration(data.SpeedCalibrationA,data.SpeedCalibrationB);
#endif
		return;
	}

	if (Contains(sentence,"UNS"))
	{
		byte lastVolume= snd.Volume;
		snd.Volume = data.AlarmsVolume;
		snd.PlayLKSound(GetValue(sentence));
		snd.Volume = lastVolume;
	}
}

int ConfigManager::GetValue(String s)
{
	int i = s.indexOf(",");
	int ii = s.indexOf("*");
	return s.substring(i+1,ii).toInt();
}

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


	icf.print("SpeedCalibrationA:");
	icf.println(data.SpeedCalibrationA);
	icf.print("SpeedCalibrationB:");
	icf.println(data.SpeedCalibrationB);
}

