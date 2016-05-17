//=====================================================================================================
//UNITY project
//=====================================================================================================

#define VARIO
#define GPS
//#define HELMET
#define AIRSPEED
//#define TESTCOMPENSTAION

//#define DEBUG
//#define DEBUG_SOUND
//#define DEBUG_COMMANDS
//#define DEBUGOUTDATATOSERIAL
//#define DEBUGCOMPENSATEDCLIMBRATE


#include <SoftwareSerial.h>
//#include <avr/interrupt.h>
//#include <avr/power.h>
//#include <avr/sleep.h>

#include <EEPROM.h>
#include "FXS_EEPROMAnything.h"
#include "DefaultValues.h"
#include "FXS_ms5611.h"
#include "LowPower.h"

#ifdef AIRSPEED
#include "FXS_ms4525.h"
#include "FXS_CompensatedVario.h"
#endif

#include "FXS_config.h"
#include "FXS_sounds.h"
#include "FXS_Button.h"


float actualPressure;
static Button button;
static SoftwareSerial softSerial(7, 8); // RX, TX
static String gpsData = "";
static String koboData = "";
Sounds snd;
ConfigManager config;
MS5611 baro(I2C_MS5611_Add);
static bool initialized = false; //delayed initialization of GSPS com port to let the softserial time to load configuration after device start
static String cmd = "";

#ifdef AIRSPEED // differential pressure
MS4525  airspd(I2C_4525_Add);
static FXS_CompensatedVario dteVario;
#endif

#ifdef DEBUG_SOUND
int vario = 0;
#endif
//loop timers
static unsigned long lastVarioDataTime = 5000;
#ifdef DEBUGOUTDATATOSERIAL
unsigned long lastOutupDataTime = 2000;
#endif

void setup()
{
	softSerial.begin(19200);
	button.Configure(BUTTONPIN);
	button.OnClick = OnClick;
	button.OnLongPress = OnLongClick;
	button.OnVLongPress = VLongPress;
	button.OnDblClick = OnDblClick;
	actualPressure = 101325;

	baro.setup();
#ifdef AIRSPEED
	airspd.setup();
#endif

	config.LoadConfigToRuntime();
#ifdef HELMET
	config.SetVarioMode(0);
	config.data.BaseFreq = 200;
	config.data.Volume=1;
	config.data.LiftTreshold = 20;
	config.data.SinkTreshold  = -1000;
	baro.varioData.sensitivity = 23;

#endif

#ifdef DEBUG_SOUND
	config.data.RateMultiplier = 100;
	snd.SetSound(true);
	snd.BaseFreq = DEFAULTLOWSOUNDBASEFREQ;
	snd.Volume = 9;
#endif
}

void loop() {

#ifdef GPS
	if (!initialized && millis() > SERIAL_COMM_DELAY)
	{
		/*softSerial.begin(9600);*/
		/*String cmd = "";
		cmd = "PMTK251,19200";
		send_cmd(softSerial, cmd);*/
		softSerial.end();
		softSerial.begin(SERIAL_SPEED);
		SetupGps();
		Serial.begin(SERIAL_SPEED);
		initialized = true;
		airspd.airSpeedData.airspeedReset = true;
		//PrintCFG();
	}
#else
	initialized=true;
#endif

	ProcessKobo();

	if (initialized)
	{
#ifdef DEBUG_SOUND
		//snd.ContinousVarioSound(vario);
		snd.VarioSound(vario);
#else

#ifdef AIRSPEED
		if ( dteVario.compensatedClimbRateAvailable && config.data.VarioMode == 1)
		{
			snd.VarioSound(dteVario.compensatedClimbRate);
		}
		else{
			snd.VarioSound(baro.varioData.climbRate);
		}
#else
		snd.VarioSound(baro.varioData.climbRate);
#endif // airspeed

#endif // debug sound

		readSensors(); //Executive part that reads all sensor values and process them  

#ifndef DEBUG_COMMANDS
#ifdef GPS
		ProcessGPS();
#endif
#endif

#ifndef HELMET
#ifdef DEBUGOUTDATATOSERIAL
		if ((millis() - lastOutupDataTime) > 100)
		{
			DebugOutputToSerial();
			lastOutupDataTime = millis();
		}
#else
		if ((millis() - lastVarioDataTime) > VARIODATASENDINTERVAL)
		{
			SendVarioData();
			lastVarioDataTime = millis();
		}
#endif
#endif
	}
	button.CheckBP();
}// LOOP

//
//// $LXWP0,logger_stored, airspeed, airaltitude,
////   v1[0],v1[1],v1[2],v1[3],v1[4],v1[5], hdg, windspeed*CS<CR><LF>
////
//// 0 loger_stored : [Y|N] (not used in LX1600)
//// 1 IAS [km/h] ----> Condor uses TAS!
//// 2 baroaltitude [m]
//// 3-8 vario values [m/s] (last 6 measurements in last second)
//// 9 heading of plane (not used in LX1600)
//// 10 windcourse [deg] (not used in LX1600)
//// 11 windspeed [km/h] (not used in LX1600)
////
//// e.g.:
//// $LXWP0,Y,222.3,1665.5,1.71,,,,,,239,174,10.1
void SendVarioData()
{
#ifndef DEBUGOUTDATATOSERIAL
	static String varioData = "";
	varioData = "LXWP0,N,";
#ifdef AIRSPEED
	varioData.concat(airspd.airSpeedData.airSpeedSM * 3.6 / 100);
#endif
	varioData.concat(",");
	varioData.concat(baro.varioData.absoluteAlt / 100);
	varioData.concat(",,,,,,");
#ifdef AIRSPEED
	if (dteVario.compensatedClimbRateAvailable && config.data.VarioMode == 1)
	{
		varioData.concat(dteVario.compensatedClimbRateSM / 100.0f);
	}
	else{
		varioData.concat(baro.varioData.climbRateSM / 100.0f);
	}
#else
	//varioData.concat(vario/100.0f);
	varioData.concat(baro.varioData.climbRateSM / 100.0f);
#endif
	varioData.concat(",,,,");
	send_cmd(Serial, varioData);
#endif //DEBUGOUTDATATOSERIAL
}

void send_cmd(Stream &icf, String &cmd) {
	static unsigned int checksum_end, ai, bi;                                               // Calculating checksum for data string
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

	cmd = "PMTK314,0,1,0,1,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0"; //without speed sentences
	send_cmd(softSerial, cmd);

	cmd = "PMTK225,0"; // power mode
	send_cmd(softSerial, cmd);

	//cmd = "PMTK313,1"; // SBAS ON
	//send_cmd(softSerial, cmd);

	//cmd = "PMTK301,2"; //WAAS ENABLED
	//send_cmd(softSerial, cmd);

	cmd = "PMTK397,0";  // Turn off Navthreshold
	send_cmd(softSerial, cmd);

	cmd = "PMTK220,500";
	send_cmd(softSerial, cmd);
}

void wakeUp()
{//nothing its just for func pointer in sleep	
}

void SleepMode(){
	cmd = "PMTK161,0";
	send_cmd(softSerial, cmd);
	snd.PlayBeeps(80,700,1,0);
	delay(1000);
	attachInterrupt(0,wakeUp, LOW);
	LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
	detachInterrupt(0); 
	cmd = "wake";
	send_cmd(softSerial, cmd);
	config.SetVarioMode(config.data.VarioMode);
}

void ProcessGPS()
{
	while (Serial.available() > 0)
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

#ifdef DEBUG_COMMANDS
Stream &inputStream = Serial;
#else
Stream &inputStream = softSerial;
#endif
void ProcessKobo()
{
	while (inputStream.available() > 0)
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
	return s.indexOf(search) >= 0;
}

void readSensors() {
#ifdef AIRSPEED
	if (dteVario.compensatedClimbRateAvailable && config.data.VarioMode == 1) // read speed sensor twice only for vario compensated mode purposes
	{
		airspd.readSensor();
	}

#endif
	baro.readSensor();
	if (baro.varioData.absoluteAltAvailable == true && baro.varioData.rawPressure > 100000) actualPressure = baro.varioData.rawPressure / 10000.0;

#ifdef AIRSPEED

	airspd.readSensor();//reread senson to provide more data
	if (baro.varioData.altitudeAt20MsecAvailable == true && config.data.VarioMode == 1)
	{
		dteVario.CalculateDte();
	}
#endif
}

//=====================================================================================================
//button handlers section
//=====================================================================================================
void OnDblClick(int pin) {
#ifdef DEBUG_SOUND
	vario = vario - 30;
	return;
#endif
	snd.SetSound(!snd.SoundOn);
	config.data.SoundOn = snd.SoundOn;
	config.Save();
}

void OnClick(int pin)
{
#ifdef DEBUG_SOUND
	/*snd.PlayLKSound(ii);
	if (ii>17) {ii=0;return;}
	ii++;
	return;*/

	vario = vario + 30;
	Serial.println(vario / 100.0);
	return;

#endif
	if (!snd.SoundOn)
	{
		snd.SetSound(true);
		return;
	}

	if (snd.Volume == 10)
	{
		snd.Volume = config.data.LowSoundVolume;
		snd.BaseFreq = config.data.LowBaseFreq;
		snd.PlayBeeps( config.data.LowBaseFreq,160,2,20);
	}
	else{
		snd.Volume = 10;
		snd.BaseFreq = config.data.BaseFreq;
		snd.PlayBeeps( config.data.BaseFreq,160,2,20);
	}

	config.data.Volume = snd.Volume;
	config.Save();
}

void OnLongClick(int pin)
{
	if (config.data.VarioMode == 1)
	{
		config.SetVarioMode(0);

	}
	else{
		config.SetVarioMode(1);
	}
}

void VLongPress(int pin)
{
#ifdef TESTCOMPENSTAION
	if (config.data.Compesation >= 140) config.data.Compesation = 70;
	config.data.Compesation += 10;
	snd.PlayBeeps(1000,80, config.data.Compesation/10-7,160);
	config.Save();
	return;
#endif

#ifdef AIRSPEED
	//airspeeed sensor for proper null airspeed
	snd.SoundUp();
	snd.SoundDn();
	airspd.airSpeedData.airspeedReset = true;
	return;
#else
	SleepMode(); 
	return;
#endif
}

int32_t Smooth(int32_t data, float filterVal, float smoothedVal){
	smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);
	return (int32_t)smoothedVal;
}

#ifdef DEBUGOUTDATATOSERIAL

//used for debuging and testing
void DebugOutputToSerial()
{

	Serial.print(airspd.airSpeedData.airSpeed * 3.6 / 100);
	Serial.print(F(", "));

	Serial.print(airspd.airSpeedData.airSpeedSM * 3.6 / 100);
	Serial.print(F(", "));

	Serial.print(((float)baro.varioData.climbRate) / 100);
	Serial.print(F(", "));
	Serial.print(((float)baro.varioData.climbRateSM) / 100);
	Serial.print(F(", "));

	Serial.print((float)dteVario.compensatedClimbRate / 100);
	Serial.print(F(","));

	Serial.print((float)dteVario.compensatedClimbRateSM / 100);
	Serial.print(F(","));

	Serial.print((float)dteVario.rawTotalEnergy);
	Serial.print("");

	Serial.print(((float)baro.varioData.absoluteAlt) / 100);
	Serial.println(F(","));

	//Serial.print(F(","));
	//Serial.print( (airspd.airSpeedData.temperature4525) -273.15);
	//Serial.print(F(","));
	//Serial.print( baro.varioData.temperature/100.0);
}
#endif // OutputToSerial
//
//void PrintCFG()
//{
//	Serial.print("schema:");
//	Serial.println(config.data.SchemaVersion);
//
//	Serial.print("mode:");
//	Serial.println(config.data.VarioMode);
//
//	Serial.print("soundon:");
//	Serial.println(config.data.SoundOn);
//
//	Serial.print("basefreq:");
//	Serial.println(config.data.BaseFreq);
//
//	Serial.print("lowbasefreq:");
//	Serial.println(config.data.LowBaseFreq);
//
//	Serial.print("LowSoundVolume:");
//	Serial.println(config.data.LowSoundVolume);
//
//	Serial.print("Volume:");
//	Serial.println(config.data.Volume);
//
//	Serial.print("LiftTreshold:");
//	Serial.println(config.data.LiftTreshold);
//
//	Serial.print("SinkTreshold:");
//	Serial.println(config.data.SinkTreshold);
//
//	Serial.print("Beeprate:");
//	Serial.println(config.data.RateMultiplier);
//
//	Serial.print("Sensitivity:");
//	Serial.println(config.data.Sensitivity);
//
//	Serial.print("Compensation:");
//	Serial.println(config.data.Compesation);
//	Serial.print("SpeedCalibrationA:");
//	Serial.println(config.data.SpeedCalibrationA);
//	Serial.print("SpeedCalibrationB:");
//	Serial.println(config.data.SpeedCalibrationB);
//}
