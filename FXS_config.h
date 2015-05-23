#ifndef FXS_CONFIG_h
#define FXS_CONFIG_h

#include "DefaultValues.h"
#include "FXS_ms5611.h"
#include "FXS_ms4525.h"
#include "FXS_sounds.h"
#include "FXS_EEPROMAnything.h"

#define SCHEMAVERSION 1 //this version must be changed when configuration structure changes! (causes reset of params while loading them)
struct Configuration
{
	int SchemaVersion;
	int VarioMode;
	int LiftTreshold;
	int SinkTreshold;
	int Volume;
	int LowSoundVolume;
	bool SoundOn;
	int BaseFreq;
	int LowBaseFreq;
	int SpeedCalibrationA;
	int SpeedCalibrationB;
};

//#define VARIO
//#define GPS
#define AIRSPEED


extern MS5611 baro;
extern Sounds snd;
#ifdef AIRSPEED
extern MS4525  airspd;
#endif

const byte normal=0;
const byte compensated=1;
const byte highVolume=0;
const byte lowVolume=0;


class ConfigManager 
{
public:
	ConfigManager();
	Configuration data;
	void SetDefaults();
	void LoadConfigToRuntime();
	
	void SetVarioMode(byte m);

private:
	void ReadFromMemory();
};

#include "HardwareSerial.h"
#endif 



