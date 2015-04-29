//=====================================================================================================
//Fly X Sense project
//=====================================================================================================

//#define DEBUG
//#define VARIO_SOUND_TEST
//#define DEBUGCOMPENSATEDCLIMBRATE
//#define DEBUGOUTDATATOSERIAL

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include "FXS_config.h"
#include "FXS_ms5611.h"
#include "FXS_ms4525.h"
#include "FXS_sounds.h"
#include "FXS_Button.h"
#include "FXS_CompensatedVario.h"
#include "FXS_EEPROMAnything.h"
#include "TinyGPS.h"

float actualPressure;
Button button;
SoftwareSerial softSerial(7,8); // RX, TX
String gpsData="";
String koboData="";
Sounds snd;
Configuration config;
//TinyGPS gps;
MS5611 baro(I2C_MS5611_Add);
#ifdef AIRSPEED // differential pressure
MS4525  airspd(I2C_4525_Add);
FXS_CompensatedVario dteVario;
#endif

//loop timers
unsigned long nextVarioDataUpdateMillis = 5000;
unsigned long nextGpsOutputTimeMillis = 5000;

void setup()
{
	LoadConfig();
	button.Configure(BUTTONPIN);
	button.OnClick = OnClick;
	button.OnLongPress= OnLongClick;
	button.OnVLongPress = VLongPress;
	button.OnDblClick = OnDblClick;

	baro.setup();
#ifdef AIRSPEED
	airspd.setup();
#endif

	actualPressure = 101325;

	softSerial.begin(SERIAL_SPEED);
	SetupGps();
	Serial.begin(SERIAL_SPEED);
	/*snd.Volume = LOWSOUNDVOLUME;
	snd.BaseFreq = config.BaseFreq/4;*/
}

#ifdef DEBUG && VARIO_SOUND_TEST
int vario = 0 ;
#endif

void loop() {
	readSensors(); //Executive part that reads all sensor values and process them  

#ifdef DEBUG && VARIO_SOUND_TEST
	snd.VarioSound(vario);
	OutputToSerial();
#else
#ifdef AIRSPEED
	if (baro.varioData.climbRateAvailable && dteVario.compensatedClimbRateAvailable && config.Mode == 1)
		snd.VarioSound( dteVario.compensatedClimbRate);
	else
		snd.VarioSound(baro.varioData.climbRate);
#else
	snd.VarioSound(baro.varioData.climbRate);
#endif
#endif

#ifndef DEBUG
	ProcessGPS();

	if ((millis() - nextVarioDataUpdateMillis) > VARIODATASENDINTERVAL)
	{
		SendVarioData();
		ProcessKobo();
		nextVarioDataUpdateMillis = millis();
	}
#endif
	button.CheckBP();

}// LOOP


// $LXWP0,logger_stored, airspeed, airaltitude,
//   v1[0],v1[1],v1[2],v1[3],v1[4],v1[5], hdg, windspeed*CS<CR><LF>
//
// 0 loger_stored : [Y|N] (not used in LX1600)
// 1 IAS [km/h] ----> Condor uses TAS!
// 2 baroaltitude [m]
// 3-8 vario values [m/s] (last 6 measurements in last second)
// 9 heading of plane (not used in LX1600)
// 10 windcourse [deg] (not used in LX1600)
// 11 windspeed [km/h] (not used in LX1600)
//
// e.g.:
// $LXWP0,Y,222.3,1665.5,1.71,,,,,,239,174,10.1

String varioData = "";
void SendVarioData()
{
#ifndef DEBUGOUTDATATOSERIAL
	varioData = "LXWP0,N,";
#ifdef AIRSPEED
	varioData.concat(airspd.airSpeedData.airSpeedSM * 3.6 / 100);
#endif
	varioData.concat(",");
	varioData.concat(baro.varioData.absoluteAlt / 100);
	varioData.concat(",");
	for (int i = 0; i < 6; i++)
	{
#ifdef AIRSPEED
		if (baro.varioData.climbRateAvailable && dteVario.compensatedClimbRateAvailable && config.Mode == 1)
		{
			varioData.concat((float)dteVario.compensatedClimbRate / 100);
		}
		else{
			varioData.concat((float)baro.varioData.climbRateSM / 100);
		}
#else
		varioData.concat((float)baro.varioData.climbRateSM / 100);
#endif
		varioData.concat(",");
	}

	varioData.concat(",,,");
	send_cmd(Serial, varioData);
#endif //debug
}

void send_cmd(Stream &icf, String &cmd) {
	unsigned int checksum_end, ai, bi;                                               // Calculating checksum for data string
	for (checksum_end = 0, ai = 0; ai < cmd.length(); ai++)
	{
		bi = (unsigned char)cmd[ai];
		checksum_end ^= bi;
	}
	icf.print("$");                 //print first sign of NMEA protocol
	icf.print(cmd);                 // print data string
	icf.print("*");                 //end of protocol string
	icf.println(checksum_end, HEX); //print calculated checksum on the end of the string in HEX
}

void SetupGps(){
	String cmd = "";

	//cmd = "PMTK869,1,1"; 
	send_cmd(softSerial,cmd);
	//cmd = "PMTK314,0,1,1,1,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0"; with speed sentence
	cmd = "PMTK314,0,1,0,1,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0"; //without speed sentences
	send_cmd(softSerial, cmd);
	cmd = "PMTK220,500";  
	send_cmd(softSerial, cmd); //set default 500ms per fix
	//cmd = "PMTK251,57600"; send_cmd(softSerial, cmd);
	//Serial.begin(GPS_SERIAL_SPEED);
}

void ProcessGPS()
{ 
	while (Serial.available())
	{
		char in = Serial.read();
		//gps.encode(in);
		if (in == '$') {
			Serial.println(gpsData);
			gpsData = "$";
		} 
		else if (in != '\n') {
			gpsData.concat(in);
		} 
	}
}

void ProcessKobo()
{ 
	while (softSerial.available())
	{
		char in = softSerial.read();
		Serial.println(in);
		if (in == '$') {
			if (Contains(koboData,"UNITY_SOUND"))
			{
				snd.PlayLKSound(GetValueFromSoundNMEA(koboData));
			}
			koboData = "$";
		} 
		else if (in != '\n') {
			koboData.concat(in);
		} 
	}
}

int GetValueFromSoundNMEA(String s)
{
	int i = s.indexOf(",");
	int ii = s.indexOf("*");
	return s.substring(i+1,ii).toInt();
}

bool Contains(String s, String search) {
	return s.indexOf(search) >=0;
} 

void readSensors() {
#ifdef AIRSPEED
	airspd.readSensor();
#endif
	baro.readSensor();
	if ( baro.varioData.absoluteAltAvailable == true && baro.varioData.rawPressure > 100000.0f ) actualPressure = baro.varioData.rawPressure / 10000.0 ;

#ifdef AIRSPEED
	airspd.readSensor(); //reread senson to provide more data

	if ( baro.varioData.altitudeAt20MsecAvailable == true )
	{
		dteVario.CalculateDte();
	}
#endif
}

//=====================================================================================================
//button handlers section
//=====================================================================================================
void OnDblClick(int pin) {
#ifdef DEBUG && VARIO_SOUND_TEST VARIO_SOUND_TEST
	vario = vario-20;
	return;
#endif

	snd.SetSound(!snd.SoundOn);
}

int ii = 0;
void OnClick(int pin)
{ 
	/*snd.PlayLKSound(ii);
	if (ii>17) {ii=0;return;}
	ii++;
	return;*/

#ifdef DEBUG && VARIO_SOUND_TEST VARIO_SOUND_TEST
	vario = vario+20;
	return;
#endif

	if (!snd.SoundOn) 
	{
		snd.SetSound(true);
		return;
		EEPROM_writeAnything(0, config);
	}

	if (snd.Volume == 10)	
	{
		snd.Volume = LOWSOUNDVOLUME;
		snd.BaseFreq = config.BaseFreq/4;
		snd.SoundUp2();
	}else{
		snd.Volume = 10;
		snd.BaseFreq = config.BaseFreq;
		snd.SoundUp2();
	}
	EEPROM_writeAnything(0, config);
}

void VLongPress(int pin)
{
	//airspeeed sensor for proper null airspeed
	snd.Play(600,200);
	delay(500);
	snd.Play(600,200);
	delay(500);
	snd.Play(700,1000);
	delay(800);
	airspd.airSpeedData.airspeedReset=true;
	delay(200);
	noToneAC();
}

void OnLongClick(int pin)
{
	SetMode(config.Mode++);
}

void SetMode(byte m)
{
	if (config.Mode > 2 || config.Mode < 0) 
	{
		config.Mode = 1;
		snd.SoundUp2();
	}else{
	    snd.SoundUp2();
		snd.SoundUp2();
	}

#ifndef AIRSPEED  // step over dte vario mode if no airspeed sensor
	if (config.Mode == 1) config.Mode++;
#endif

	EEPROM_writeAnything(0, config);
}

void LoadConfig()
{
	EEPROM_readAnything(0, config);
	if (config.SchemaVersion != SCHEMAVERSION )
	{
		SetDefaults();
	}
	snd.SoundOn = config.SoundOn;
	snd.Volume = config.Volume;
	if (snd.Volume<10) snd.BaseFreq = config.BaseFreq/4;
	SetMode(config.Mode);
}

void SetDefaults()
{
	config.SchemaVersion = SCHEMAVERSION;
	config.Mode = 1;
	config.SoundOn = true;
	config.BaseFreq = DEFAULTSOUNDBASEFREQ;
	config.SpeedCalibrationA= SPEEDCALIBRATION_A;
	config.SpeedCalibrationB= SPEEDCALIBRATION_B;
	config.VarioClimbRateStart = CLIMBSOUNDTRESHOLD;
	config.VarioSinkRateStart = SINKSOUNDTRESHOLD;
	config.Volume = 5;
	EEPROM_writeAnything(0, config);
}

#ifdef DEBUG
//used for debuging and testing
void OutputToSerial()
{return;
	//Serial.print( airspd.airSpeedData.airSpeed * 3.6/100);
	//Serial.print(F(","));
	Serial.print( airspd.airSpeedData.airSpeed * 3.6 / 100);
	Serial.print(F(", "));
	Serial.print( ( (float)baro.varioData.climbRate) / 100);
	Serial.print(F(", "));
	Serial.print( ( (float)baro.varioData.climbRateSM) / 100);
	Serial.print(F(", "));
	Serial.print( ( (float)baro.varioData.absoluteAlt) / 100);
	Serial.print(F(","));
	//Serial.print( (float)compensatedClimbRate / 100);
	Serial.print(F(","));
	//   Serial.print( (float)rawTotalEnergy);
	Serial.println("");
	//Serial.print(F(","));
	//Serial.print( (airspd.airSpeedData.temperature4525) -273.15);
	//Serial.print(F(","));
	//Serial.print( baro.varioData.temperature/100.0);
}
#endif // OutputToSerial

