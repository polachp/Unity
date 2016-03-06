//=====================================================================================================
//UNITY project
//=====================================================================================================

#define VARIO
#define GPS
#define AIRSPEED

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
#include "FXS_ms4525.h"
#include "FXS_config.h"
#include "FXS_sounds.h"
#include "FXS_Button.h"
#include "FXS_CompensatedVario.h"

float actualPressure;
Button button;
SoftwareSerial softSerial(7, 8); // RX, TX
String gpsData = "";
String koboData = "";
Sounds snd;
ConfigManager config;
MS5611 baro(I2C_MS5611_Add);
bool initialized = false; //delayed initialization of GSPS com port to let the softserial time to load configuration after device start

#ifdef AIRSPEED // differential pressure
MS4525  airspd(I2C_4525_Add);
FXS_CompensatedVario dteVario;
#endif

#ifdef DEBUG_SOUND
int vario = 0;
#endif
//loop timers
unsigned long lastVarioDataTime = 5000;
#ifdef DEBUGOUTDATATOSERIAL
unsigned long lastOutupDataTime = 2000;
#endif
unsigned long nextGpsOutputTimeMillis = 5000;
void setup()
{
	softSerial.begin(19200);
	button.Configure(BUTTONPIN);
	button.OnClick = OnClick;
	button.OnLongPress = OnLongClick;
	button.OnVLongPress = VLongPress;
	button.OnDblClick = OnDblClick;

	//reset if holding button
	int i = 0;
	while (digitalRead(BUTTONPIN) == LOW)
	{
		i++;
		snd.Play(700, 1000);
		delay(50 * i);
		noToneAC();
		delay(1000 - i * 50);
		if (i >= 10)
		{
			snd.Play(1500, 1000);
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

#ifdef DEBUG_SOUND
	config.data.RateMultiplier = 130;
	snd.SetSound(true);
	snd.BaseFreq = DEFAULTSOUNDBASEFREQ;
	snd.Volume = 10;
#endif
}

void loop() {
	if (!initialized && millis() > SERIAL_COMM_DELAY)
	{
		softSerial.end();
		softSerial.begin(9600);

		String cmd = "";
		cmd = "PMTK251,19200";
		send_cmd(softSerial, cmd);

		softSerial.begin(SERIAL_SPEED);
		SetupGps();
		Serial.begin(SERIAL_SPEED);
		initialized = true;
		config.Print(Serial);
	}
	ProcessKobo();

	if (initialized)
	{
#ifdef DEBUG_SOUND
		//snd.ContinousVarioSound(vario);
		snd.VarioSound(vario);
#else

#ifdef AIRSPEED
		if (baro.varioData.climbRateAvailable && dteVario.compensatedClimbRateAvailable && config.data.VarioMode == compensated)
		{
			snd.VarioSound(dteVario.compensatedClimbRate);
		}
		else{
			snd.VarioSound(baro.varioData.climbRate);
		}
#else
		snd.VarioSound(baro.varioData.climbRate);
#endif


#endif

		readSensors(); //Executive part that reads all sensor values and process them  

#ifndef DEBUG_COMMANDS
		ProcessGPS();
#endif
		if ((millis() - lastVarioDataTime) > VARIODATASENDINTERVAL)
		{
			SendVarioData();
			lastVarioDataTime = millis();
		}
#ifdef DEBUGOUTDATATOSERIAL
		if ((millis() - lastOutupDataTime) > 100)
		{
			OutputToSerial();
			lastOutupDataTime = millis();
		}
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
;
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
	if (baro.varioData.climbRateAvailable && dteVario.compensatedClimbRateAvailable && config.data.VarioMode == compensated)
	{
		varioData.concat(dteVario.compensatedClimbRate / 100.0f);
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

	cmd = "PMTK314,0,1,0,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0"; //without speed sentences
	send_cmd(softSerial, cmd);

	cmd = "PMTK220,500";
	send_cmd(softSerial, cmd);
}

void SleepMode(){
	//String cmd = "";
	//cmd = "PMTK161,0";
	//send_cmd(softSerial, cmd);

	//for (int x = 1; x < 20; x++){
	//	pinMode(x, INPUT);
	//	digitalWrite(x, LOW);
	//}
	//digitalWrite(13, LOW);
	//byte adcsra, mcucr1, mcucr2;
	//sleep_enable();
	//set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	//adcsra = ADCSRA;               //save the ADC Control and Status Register A
	//ADCSRA = 0;                    //disable ADC
	//cli();
	//EIMSK |= _BV(INT0);            //enable INT0
	//mcucr1 = MCUCR | _BV(BODS) | _BV(BODSE);  //turn off the brown-out detector
	//mcucr2 = mcucr1 & ~_BV(BODSE);
	//MCUCR = mcucr1;
	//MCUCR = mcucr2;
	//sei();                         //ensure interrupts enabled so we can wake up again
	//sleep_cpu();                   //go to sleep
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
	airspd.readSensor(); //reread senson to provide more data
	if (baro.varioData.altitudeAt20MsecAvailable == true)
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
		config.data.SoundOn = true;
		config.Save();
		return;
	}

	if (snd.Volume == 10)
	{
		snd.Volume = config.data.LowSoundVolume;
		snd.BaseFreq = config.data.LowBaseFreq;
		snd.SoundDn2();
	}
	else{
		snd.Volume = 10;
		snd.BaseFreq = config.data.BaseFreq;
		snd.SoundUp2();
	}

	config.data.Volume = snd.Volume;
	config.Save();
}

void OnLongClick(int pin)
{
#ifdef AIRSPEED
	if (config.data.VarioMode == compensated)
	{
		config.SetVarioMode(normal);
	}
	else{
		config.SetVarioMode(compensated);
	}
#else
	//snd.continous = !snd.continous;
	//snd.SoundUp2();
	//snd.SoundDn2();
#endif
}

void VLongPress(int pin)
{
#ifdef AIRSPEED
	//airspeeed sensor for proper null airspeed
	snd.Play(300, 200);
	delay(400);
	snd.Play(300, 200);
	delay(400);
	snd.Play(500, 1000);
	delay(400);
	airspd.airSpeedData.airspeedReset = true;
	delay(200);
	noToneAC();
#endif
}

#ifdef DEBUGOUTDATATOSERIAL

//used for debuging and testing
void OutputToSerial()
{
	//Serial.print( airspd.airSpeedData.airSpeed * 3.6/100);
	//Serial.print(F(","));
	Serial.print(airspd.airSpeedData.airSpeedSM * 3.6 / 100);
	Serial.print(F(", "));
	Serial.print(((float)baro.varioData.climbRate) / 100);
	Serial.print(F(", "));
	Serial.print(((float)baro.varioData.climbRateSM) / 100);
	Serial.print(F(", "));

	Serial.print((float)dteVario.compensatedClimbRate / 100);
	Serial.print(F(","));

	Serial.print(((float)baro.varioData.absoluteAlt) / 100);
	Serial.print(F(","));

	Serial.print((float)dteVario.rawTotalEnergy);
	Serial.println("");
	//Serial.print(F(","));
	//Serial.print( (airspd.airSpeedData.temperature4525) -273.15);
	//Serial.print(F(","));
	//Serial.print( baro.varioData.temperature/100.0);
}
#endif // OutputToSerial


