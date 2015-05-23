#include "FXS_config.h"

ConfigManager::ConfigManager()
{
	
}

void ConfigManager::ReadFromMemory()
{
	EEPROM_readAnything(0, data);
	if (data.SchemaVersion != SCHEMAVERSION )
	{
		SetDefaults();
	}
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
	EEPROM_writeAnything(0, data);
}

void ConfigManager::LoadConfigToRuntime()
{
	ReadFromMemory();
	snd.SoundOn = data.SoundOn;
	snd.Volume = data.Volume;
	if (snd.Volume<10) snd.BaseFreq = data.LowBaseFreq; // in case of lower sount than max we use lower base freq mode
#ifdef AIRSPEED
	//airspd.setCalibration(config.SpeedCalibrationA,config.SpeedCalibrationB);  
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
	data.LiftTreshold = DEFAULT_LIFT_TRESHOLD;
	data.SinkTreshold = DEFAULT_SINK_TRESHOLD;
	data.Volume = 10;
	data.SpeedCalibrationA= SPEEDCALIBRATION_A;
	data.SpeedCalibrationB= SPEEDCALIBRATION_B;

	EEPROM_writeAnything(0, data);
}

