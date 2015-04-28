#include "FXS_ms4525.h"

#define FILTERING4525_ADC_MIN        0.001   // 
#define FILTERING4525_ADC_MAX        0.01 // 
#define FILTERING4525_ADC_MIN_AT       10 // when abs(delta between ADC and current value) is less than MIN_AT , apply MIN  
#define FILTERING4525_ADC_MAX_AT       100 // when abs(delta between ADC and current value) is more than MAX_AT , apply MAX (interpolation in between)

extern unsigned long micros( void ) ;
extern unsigned long millis( void ) ;
extern void delay(unsigned long ms) ;

MS4525::MS4525(uint8_t addr)
{ // constructor
  _addr = addr;
}

// **************** Setup the 4525DO sensor *********************
void MS4525::setup() {
  //airSpeedData.available=false;
  kalmanAirspeedSM = KalmanFilter(0.0008f,0.004f);
  calibrated4525 = false ;
  //  calibrateCount4525 = 0 ;
  airSpeedData.airSpeedAvailable = false ;
  airSpeedData.compensationAvailable = false ;

  airSpeedData.airspeedReset = true ; // set on true to force a reset the first time the 100 ms part is entered
  nextAirSpeedMillis  =  3200 ;  // save when AirSpeed has to be calculated; Airspeed is available only after 3200 in order to get a stable value (less temperature drift)

  I2c.begin() ;
  I2c.timeOut( 80); //initialise the time out in order to avoid infinite loop
  // read the sensor to get the initial temperature
  I2CErrorCode4525 = I2c.read( _addr,  4 ) ; //read 4 bytes from the device;
  data[0] = I2c.receive() ;
  data[1] = I2c.receive() ;
  data[2] = I2c.receive() ;
  data[3] = I2c.receive() ;
  if ( ( data[0] & 0xC0 ) == 0) {
    temperature4525Adc =    (data[2] << 8) + data[3] ;
    temperature4525Adc = (0xFFE0 & temperature4525Adc) >> 5;
    airSpeedData.temperature4525 = (0.097703957f * temperature4525Adc)  + 223 ; // in kelvin
  } else {
    airSpeedData.temperature4525 = 300 ;
  }
}  //end of setup


/****************************************************************************/
/* readSensor - Read differential pressure + temperature from the 4525DO    */
/****************************************************************************/
void MS4525::readSensor() {
  static int32_t difPressureAdc4Values[4] = { 0, 0, 0, 0 };

  static int32_t difPressureAdcSum4Values;
  static int8_t countAverage ;
  I2CErrorCode4525 = I2c.read( _addr,  2 ) ; //read 2 bytes from the device;
  if ( I2CErrorCode4525 == 0) {
    data[0] = I2c.receive() ;
    data[1] = I2c.receive() ;
    if ( ( data[0] & 0xC0 ) == 0) {
      difPressureAdc =  ( ( (data[0] << 8) + data[1] ) & 0x3FFF) - 0x2000   ; // substract in order to have a zero value
      if ( calibrated4525 == false) {
        calibrateCount4525++ ;
        if (calibrateCount4525 == 256 ) { // after 256 reading , we can calculate the offset
          offset4525 =  (  (float) difPressureSum / 128.0 ) ; //there has been 128 reading (256-128)
          calibrated4525 = true ;
        } else if  (calibrateCount4525 >= 128  ) { // after 128 reading, we can start cummulate the ADC values in order to calculate the offset
          difPressureSum += difPressureAdc ;
        } // end calibration
      }  else { // sensor is calibrated
        difPressureAdc_0 = difPressureAdc - offset4525 ;
        // calculate a moving average on 4 values
        difPressureAdcSum4Values += difPressureAdc - difPressureAdc4Values[countAverage] ;
        difPressureAdc4Values[countAverage] = difPressureAdc ;
        if ( (++countAverage) >= 4 ) countAverage = 0 ;
        airSpeedData.difPressureAdc_zero = (float) difPressureAdcSum4Values * 0.25 - offset4525 ;


        abs_deltaDifPressureAdc =  abs(difPressureAdc_0 - airSpeedData.smoothDifPressureAdc) ;
        if (abs_deltaDifPressureAdc <= FILTERING4525_ADC_MIN_AT) {
          expoSmooth4525_adc_auto = FILTERING4525_ADC_MIN ;
        } else if (abs_deltaDifPressureAdc >= FILTERING4525_ADC_MAX_AT)  {
          expoSmooth4525_adc_auto = FILTERING4525_ADC_MAX ;
        } else {
          expoSmooth4525_adc_auto = FILTERING4525_ADC_MIN + ( FILTERING4525_ADC_MAX - FILTERING4525_ADC_MIN) * (abs_deltaDifPressureAdc - FILTERING4525_ADC_MIN_AT) / (FILTERING4525_ADC_MAX_AT - FILTERING4525_ADC_MIN_AT) ;
        }
        airSpeedData.smoothDifPressureAdc += expoSmooth4525_adc_auto * ( difPressureAdc_0 - airSpeedData.smoothDifPressureAdc ) ; //
        //difPressure =  ((smoothDifPressureAdc  ) * 10520.56427) ; //-  86184462.5   ; //conversion in 1/10000 of Pa
        //           difPressure = -((difPressureAdc - 1638) * 0.0001525972 - 1) * 6894.757f ; // convert ADC in pascal : first calculate in PSI and then * conversion factor PSI to Pa
      }
      // calculate airspeed based on pressure, altitude and temperature
      // airspeed = sqr(2 * differential_pressure / air_mass_per_kg) ; air mass per kg = pressure  pa / (287.05 * (Temp celcius + 273.15))
      // so airspeed m/sec =sqr( 574.1 * differential_pressure pa * (temperature Celsius + 273.15) / pressure pa )
      //               rawAirSpeed =  sqrt( (float) (574 * 10520.56427 * abs(smoothDifPressureAdc) * temperature4525  /  actualPressure) ); // in cm/sec ; actual pressure must be in pa (so 101325 about at sea level)
      airSpeedData.rawAirSpeed =  2457.4 * sqrt( (float) ( abs(airSpeedData.smoothDifPressureAdc) * airSpeedData.temperature4525  /  actualPressure) ); // in cm/sec ; actual pressure must be in pa (so 101325 about at sea level)
     
      //if ( airSpeedData.smoothDifPressureAdc < 0 ) airSpeedData.rawAirSpeed = - airSpeedData.rawAirSpeed ; // apply the sign
      smoothAirSpeed = airSpeedData.rawAirSpeed;
      
    } // en if data[0] is valid
  } // end no error on I2C
  airSpeedMillis = millis();

  if (airSpeedMillis > nextAirSpeedMillis) { // publish airspeed only once every xx ms
    nextAirSpeedMillis = airSpeedMillis + 100 ;
    if ( smoothAirSpeed >  300) {  // normally send only if positive and greater than 300 cm/sec , otherwise send 0 but for test we keep all values to check for drift
      airSpeedData.airSpeedSM = (kalmanAirspeedSM.update(smoothAirSpeed)*SPEEDCALIBRATION_A) + SPEEDCALIBRATION_B; /// cm/sec
      airSpeedData.airSpeed = smoothAirSpeed;
    } else {
      airSpeedData.airSpeedSM = 0;
      airSpeedData.airSpeed = 0 ;
    }

    airSpeedData.airSpeedAvailable = true ;

    // check if offset must be reset
    if (airSpeedData.airspeedReset) { // adjust the offset if a reset command is received from Tx
      offset4525 =  offset4525  + airSpeedData.smoothDifPressureAdc ;
      airSpeedData.airspeedReset = false ; // avoid that offset is changed again and again if PPM do not send a command
    }


  }  // end of process every xx millis



} // End of readSensor



