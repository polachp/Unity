#ifndef FXS_CONFIG_h
#define FXS_CONFIG_h

#include "FXS_ms5611.h"
#include "FXS_ms4525.h"
#include "FXS_sounds.h"
#include "FXS_EEPROMAnything.h"

 //this version must be changed when configuration structure changes! (causes reset of params while loading them)
struct Configuration
{
	int SchemaVersion;
	byte VarioMode;
	int LiftTreshold;
	int SinkTreshold;
	byte Volume;
	byte LowSoundVolume;
	byte AlarmsVolume;
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
#ifdef PRINTCONFIG
	void Print(Stream &icf);
#endif
private:
	void Load();
    int GetValue(String s);
};

//#include "HardwareSerial.h"
#endif 




