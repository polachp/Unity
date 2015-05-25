//=====================================================================================================
//Fly X Sense project
//=====================================================================================================

#define DEBUG
//#define VARIO_SOUND_TEST
//#define DEBUGCOMPENSATEDCLIMBRATE
//#define DEBUGOUTDATATOSERIAL

#include <Arduino.h>

#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include "FXS_EEPROMAnything.h"
#include "DefaultValues.h"
#include "FXS_ms5611.h"
#include "FXS_ms4525.h"
#include "FXS_config.h"
#include "FXS_sounds.h"
#include "FXS_Button.h"
#include "FXS_CompensatedVario.h"
#include "TinyGPS.h"

float actualPressure;
Button button;
SoftwareSerial softSerial(7,8); // RX, TX
String gpsData="";
String koboData="";
Sounds snd;
ConfigManager config;
MS5611 baro(I2C_MS5611_Add);
bool initialized = false; //delayed initialization of GSPS com port to let the softserial time to load configuration after device start

#ifdef AIRSPEED // differential pressure
#define ALLOWMODESWITCH
MS4525  airspd(I2C_4525_Add);
FXS_CompensatedVario dteVario;
#endif

#ifdef VARIO_SOUND_TEST
int vario = 0 ;
#endif
//loop timers
unsigned long nextVarioDataUpdateMillis = 5000;
unsigned long nextGpsOutputTimeMillis = 5000;
void setup()
{
	button.Configure(BUTTONPIN);
	button.OnClick = OnClick;
	button.OnLongPress= OnLongClick;
	button.OnVLongPress = VLongPress;
	button.OnDblClick = OnDblClick;

	//reset if holding button
	int i = 0;
	while (digitalRead(BUTTONPIN) == LOW)
	{
		i++;
		snd.Play(700,1000);
		delay(50*i);
		noToneAC();
		delay(1000-i*50);
		if(i>=10) 
		{
			snd.SoundUp2();
			snd.SoundUp2();
			snd.SoundUp2(); 
			snd.SoundUp2();
			config.SetDefaults();
			break;
		}
	}

	actualPressure = 101325;
	baro.setup();
#ifdef AIRSPEED
	airspd.setup();
#endif
	config.LoadConfigToRuntime();
	softSerial.begin(SERIAL_SPEED);
}


void loop() {
	if (!initialized && millis() > SERIAL_COMM_DELAY)
	{
		SetupGps();
		Serial.begin(SERIAL_SPEED);
		initialized = true;
		config.Print(Serial);
	}
	readSensors(); //Executive part that reads all sensor values and process them  

#ifdef VARIO_SOUND_TEST
	snd.VarioSound(vario);
#else
#ifdef AIRSPEED
	if (baro.varioData.climbRateAvailable && dteVario.compensatedClimbRateAvailable && config.data.VarioMode == 1)
		snd.VarioSound( dteVario.compensatedClimbRate);
	else
		snd.VarioSound(baro.varioData.climbRate);
#else
	snd.VarioSound(baro.varioData.climbRate);
#endif
#endif
	ProcessKobo();
	if (initialized)
	{
		ProcessGPS();
		if ((millis() - nextVarioDataUpdateMillis) > VARIODATASENDINTERVAL)
		{
			SendVarioData();
			nextVarioDataUpdateMillis = millis();
		}
	}
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
		if (baro.varioData.climbRateAvailable && dteVario.compensatedClimbRateAvailable && config.data.VarioMode == compensated)
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
	//send_cmd(softSerial,cmd);

	//cmd = "PMTK314,0,1,1,1,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0"; with speed sentence

	cmd = "PMTK314,0,1,0,1,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0"; //without speed sentences
	send_cmd(softSerial, cmd);

	cmd = "PMTK220,250";  
	send_cmd(softSerial, cmd);

	cmd = "PMTK225,0"; 
	send_cmd(softSerial, cmd);

	cmd = "PMTK301,2";  //sbas
	send_cmd(softSerial, cmd);

	cmd = "PMTK313,1"; //waas
	send_cmd(softSerial, cmd);

	cmd = "PMTK513,1"; //waas
	send_cmd(softSerial, cmd);

	cmd = "PMTK313,1"; //waas
	send_cmd(softSerial, cmd);

	//cmd = "PMTK251,19200";
	//send_cmd(softSerial, cmd);

	//Serial.begin(GPS_SERIAL_SPEED);
}

void ProcessGPS()
{ 
	while (Serial.available())
	{
		char in = Serial.read();
	    if (in == '$') {
			Serial.println(gpsData);
			gpsData = "$";
		} 
		else if (in != '\n') {
			gpsData.concat(in);
		} 
	}
}

#ifdef DEBUG
Stream &inputStream = Serial;
#else
Stream &inputStream = softSerial;
#endif
void ProcessKobo()
{ 
	while (inputStream.available())
	{
		char in = inputStream.read();
		if (in == '*') {
			koboData.concat(in);
			config.ProcessSetCommand(koboData); 
			koboData = "";
		} 
		else if (in != '\n') {
			koboData.concat(in);
		} 
	}
}

bool Contains(String s, String search) {
	return s.indexOf(search) >=0;
} 

void readSensors() {
#ifdef AIRSPEED
	if(dteVario.compensatedClimbRateAvailable && config.data.VarioMode == 1) // read speed sensor twice only for vario compensated mode purposes
	{
		airspd.readSensor();
	}

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
#ifdef VARIO_SOUND_TEST
	vario = 0;
	Serial.println(0);
	return;
#endif
	snd.SetSound(!snd.SoundOn);
	config.data.SoundOn = snd.SoundOn;
	config.Save();
}

int ii = 0;
void OnClick(int pin)
{ 
#ifdef VARIO_SOUND_TEST
	/*snd.PlayLKSound(ii);
	if (ii>17) {ii=0;return;}
	ii++;
	return;*/

	vario = vario+20;
	Serial.println(vario/100.0);
	return;

#endif

	if (!snd.SoundOn) 
	{
		snd.SetSound(true);
		config.data.SoundOn = true;
		config.Save();
		return;
	}

	if (snd.Volume == 10)	
	{
		snd.Volume = config.data.LowSoundVolume;
		snd.BaseFreq = config.data.LowBaseFreq;
		snd.SoundDn2();
	}else{
		snd.Volume = 10;
		snd.BaseFreq =config.data.BaseFreq;
		snd.SoundUp2();
	}

	config.data.Volume=snd.Volume;
	config.Save();
}

void OnLongClick(int pin)
{
#ifdef ALLOWMODESWITCH
	if (config.data.VarioMode==compensated)
	{
		config.SetVarioMode(normal);
	}
	else{
		config.	SetVarioMode(compensated);
	}
#endif
}

void VLongPress(int pin)
{
#ifdef AIRSPEED
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
#endif
}

#ifdef DEBUG

//used for debuging and testing
void OutputToSerial()
{
	return;
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

