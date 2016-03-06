#ifndef FXS_COMPENSATEDVARIO_h
#define FXS_COMPENSATEDVARIO_h

#include "DefaultValues.h"
#include "FXS_ms5611.h"
#include "FXS_ms4525.h"

extern MS4525  airspd;
extern MS5611 baro;

class FXS_CompensatedVario
{
private:
	float smoothCompensatedClimbRate;
	float rawCompensatedClimbRate;
	float abs_deltaCompensatedClimbRate;
	float expoSmoothDte_auto;
	float rawCompensation;
	float totalEnergyLowPass;
	float totalEnergyHighPass;

public:
	float rawTotalEnergy;
	float smoothingDteMin;
	bool compensatedClimbRateAvailable;
	int32_t compensatedClimbRate;
	FXS_CompensatedVario();
	void CalculateDte();
};


#endif //fXv_COMPENSATEDVARIO_h

