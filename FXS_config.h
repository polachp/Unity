#ifndef FXS_CONFIG_h
#define FXS_CONFIG_h

#include "DefaultValues.h"
#include "FXS_ms5611.h"
#include "FXS_ms4525.h"
#include "FXS_sounds.h"
#include "FXS_EEPROMAnything.h"
#include <Arduino.h>
 //this version must be changed when configuration structure changes! (causes reset of params while loading them)
struct Configuration
{
	int SchemaVersion;
	int VarioMode;
	int LiftTreshold;
	int SinkTreshold;
	int Volume;
	int LowSoundVolume;
	int AlarmsVolume;
	bool SoundOn;
	int BaseFreq;
	int LowBaseFreq;
	int RateMultiplier;
	int SpeedCalibrationA;
	int SpeedCalibrationB;
	int Sensitivity;
	int Compesation;
};

extern MS5611 baro;
extern Sounds snd;
#ifdef AIRSPEED
extern MS4525  airspd;
#endif

//VARIOMODE
const byte normal=0;
const byte compensated=1;

//SOUND VOLUME MODE 
const byte highVolume=0;
const byte lowVolume=0;

class ConfigManager 
{
public:
	ConfigManager();
	Configuration data;
	void SetDefaults();
	void LoadConfigToRuntime();
	void ProcessSetCommand(String cmd);
	void SetVarioMode(byte m);
	void Save();
	void Print(Stream &icf);
private:
	void Load();
	
	int GetValue(String s);
};

//#include "HardwareSerial.h"
#endif 




