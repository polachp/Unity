#include "FXS_CompensatedVario.h"
#include "FXS_config.h"

extern ConfigManager config;
extern int32_t Smooth(int32_t data, float filterVal, float smoothedVal);

FXS_CompensatedVario::FXS_CompensatedVario()
{
	smoothingDteMin = SMOOTHING_DTE_MIN;
	compensatedClimbRateAvailable = false;
}

void FXS_CompensatedVario::CalculateDte()
{
	rawCompensation = 3078.25f * abs(airspd.airSpeedData.difPressureAdc_zero) * airspd.airSpeedData.temperature4525 / actualPressure; // 3078.25 = comp = 2 * 287.05 / 2 / 9.81 * 1.0520 * 100 * Temp / Pressure
	rawTotalEnergy = (baro.varioData.rawAltitude * 0.01f) + rawCompensation * config.data.Compesation * 0.0115; // 0.01 means 100% compensation but we add 20% because it seems that it is 20% undercompensated.
	if (totalEnergyLowPass == 0) { totalEnergyLowPass = totalEnergyHighPass = rawTotalEnergy; }

	totalEnergyLowPass += 0.085 * (rawTotalEnergy - totalEnergyLowPass);
	totalEnergyHighPass += 0.1 * (rawTotalEnergy - totalEnergyHighPass);
	rawCompensatedClimbRate = ((totalEnergyHighPass - totalEnergyLowPass)  * 566667.0) / baro.varioData.delaySmooth; // 0.566667 is the parameter to be used for 0.085 and 0.1 filtering if delay is in sec

	abs_deltaCompensatedClimbRate = abs(rawCompensatedClimbRate - smoothCompensatedClimbRate);

	if ((abs_deltaCompensatedClimbRate <= SMOOTHING_DTE_MIN_AT) || (smoothingDteMin >= SMOOTHING_DTE_MAX)) {
		expoSmoothDte_auto = smoothingDteMin;
	}
	else if (abs_deltaCompensatedClimbRate >= SMOOTHING_DTE_MAX_AT)  {
		expoSmoothDte_auto = SMOOTHING_DTE_MAX;
	}
	else {
		expoSmoothDte_auto = smoothingDteMin + (SMOOTHING_DTE_MAX - smoothingDteMin) * (abs_deltaCompensatedClimbRate - SMOOTHING_DTE_MIN_AT) / (SMOOTHING_DTE_MAX_AT - SMOOTHING_DTE_MIN_AT);
	}
	smoothCompensatedClimbRate += expoSmoothDte_auto * (rawCompensatedClimbRate - smoothCompensatedClimbRate) * 0.001;
	compensatedClimbRate = smoothCompensatedClimbRate;
	compensatedClimbRateSM = Smooth(smoothCompensatedClimbRate,0.95,compensatedClimbRateSM); 

	compensatedClimbRateAvailable = true; // allows SPORT protocol to transmit the value every 20 ms
	baro.varioData.altitudeAt20MsecAvailable = false; // avoid to handel it each time.


	//static bool firstCompensatedClimbRate = true;
	//if (firstCompensatedClimbRate) {
	//	Serial.println(F("at, altitude, difPressureADC_zero, rawCompensation , rawTotalEnergy , delaySmooth , rawCompensatedClimbRate , compensatedClimbRate "));
	//	firstCompensatedClimbRate = false;
	//}
	//else {
	//	Serial.print(micros()); Serial.print(F(" , "));
	//	Serial.print(baro.varioData.rawAltitude); Serial.print(F(" , "));
	//	Serial.print(airspd.airSpeedData.difPressureAdc_zero); Serial.print(F(" , "));
	//	Serial.print(rawCompensation); Serial.print(F(" , "));
	//	Serial.print(rawTotalEnergy); Serial.print(F(" , "));
	//	Serial.print(baro.varioData.delaySmooth); Serial.print(F(" , "));
	//	Serial.print(rawCompensatedClimbRate); Serial.print(F(" , "));
	//	Serial.print(compensatedClimbRate);
	//	Serial.println(F(" "));
	//}

}

