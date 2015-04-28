#include "FXS_ms5611.h"

extern unsigned long micros( void ) ;
extern unsigned long millis( void ) ;
extern void delay(unsigned long ms) ;

MS5611::MS5611(uint8_t addr)
{
  // constructor
  _addr = addr;
  varioData.SensorState = 0 ;
}

// **************** Setup the MS5611 sensor *********************
void MS5611::setup() {
  kalmanVario = KalmanFilter(0.0002f,0.05f);
  varioData.sensitivity = SENSITIVITY_MIN;
  varioData.absoluteAltAvailable=false ;
  varioData.climbRateAvailable = false ;
  varioData.delaySmooth = 20000 ; // delay between 2 altitude calculation = 20msec = 20000 usec
  
  I2c.begin() ;
  I2c.timeOut( 80); //initialise the time out in order to avoid infinite loop
  errorI2C = I2c.write( _addr, 0x1e) ;
  errorCalibration = false ;
  if (errorI2C > 0 ) {
    errorCalibration = true ;
  } else {
    delay(100);
    for (byte i = 1; i <= 6; i++) {
      errorI2C =  I2c.read( _addr, 0xa0 + i * 2, 2 ) ; //read 2 bytes from the device after sending the command A0 + xx depending on the register to be read
      if ( errorI2C > 0 ) {
        errorCalibration = true ;
      } else {
        high = I2c.receive() ;
        low = I2c.receive() ;
        _calibrationData[i] = high << 8 | low;
      }
    } // End for
  } // End if else

}  //end of setup


//********************************************************************************************
//***                            read the sensor                                           ***
//********************************************************************************************

void MS5611::readSensor() {
  if (varioData.SensorState == 1) { // ========================= Read the pressure
    extended2Micros = micros() >> 1 ;
    if (extended2Micros < varioData.lastCommand2Micros) extended2Micros = extended2Micros | 0x80000000 ;
    if ( extended2Micros  > varioData.lastCommand2Micros + 4500) { // wait 9 msec at least before asking for reading the pressure
      result = 0;
      if (  ! I2c.read( _addr, 0, 3 )) {
        //read 3 bytes from the device after sending a command "00"; keep previous value in case of error
        result = I2c.receive() ;
        result <<= 8 ;
        result |= I2c.receive() ;
        result <<= 8 ;
        result |= I2c.receive() ;
        D1 = result;
      } else {
        D1 = 0 ; // D1 value are not processed to calculate Alt.
      }
      I2c.write( _addr, 0x58) ; // ask a conversion of Temperature
      varioData.lastCommand2Micros = (micros() >> 1 );
      varioData.SensorState = 2;
    } // end of delay of 9 ms
  } // end of SensorState == 1
  else if (varioData.SensorState == 2) { // READ TEMPERATURE
    extended2Micros = micros() >> 1 ;
    if (extended2Micros < varioData.lastCommand2Micros) extended2Micros = extended2Micros | 0x80000000 ;
    if ( extended2Micros > varioData.lastCommand2Micros + 4500) { // wait 9000 usec to get Temp with high precision
      if ( ! I2c.read( _addr, 0, 3 )) {
        result = I2c.receive();
        result <<= 8 ;
        result |= I2c.receive() ;
        result <<= 8 ;
        result |= I2c.receive() ;
        D2 = result;
      }
      varioData.SensorState = 0;
    }  // End of process if temperature can be read

  } // End of process if SensorState was 1 or 2
 
  if (varioData.SensorState == 0) {  // ========================== new Pressure and (new or old) Temp are known so Request Pressure immediately and calculate altitude
    I2c.write( _addr, 0x48) ; // ask a conversion of Pressure
    pressureMicrosPrev1 = pressureMicros ;
    pressureMicros = micros(); // pressureMicros is the timestamp to calculate climbrate between 2 pressures
    varioData.lastCommand2Micros = pressureMicros >> 1 ;
    varioData.SensorState = 1;
    if ((D1 > 0) & (millis() > 1000) ) { // If D1 has been read in a previous loop and if sensor is started since more than 1 sec, then calculate pressure etc...
      if (D2Prev == 0) D2Prev = D2 ;
      D2Apply = (D2 + D2Prev ) / 2 ;
      D2Prev = D2 ;
      dT = D2Apply - ((long)_calibrationData[5] << 8);
      TEMP = (2000 + (((int64_t)dT * (int64_t)_calibrationData[6]) >> 23)) / (float) 1.0 ;
      varioData.temperature = TEMP;
      OFF  = (((int64_t)_calibrationData[2]) << 16) + ((_calibrationData[4] * dT) >> 7);
      SENS = (((int64_t)_calibrationData[1]) << 15) + ((_calibrationData[3] * dT) >> 8);
      varioData.rawPressure = (((((((int64_t) D1) * (int64_t) SENS) >> 21) - OFF) * 10000 ) >> 15) ; // 1013.25 mb gives 1013250000 is a factor to keep higher precision (=1/100 cm).
    
      if ( varioData.rawPressure > 954610000) {
        varioData.rawAltitude = ( 1013250000 - varioData.rawPressure ) * 0.08526603 ; // = 500 / (101325 - 95461)  // returned value 1234567 means 123,4567 m (temp is fixed to 15 degree celcius)
      } else if ( varioData.rawPressure > 898760000) {
        varioData.rawAltitude = 5000000 + ( 954610000 - varioData.rawPressure ) * 0.089525515  ;
      } else if ( varioData.rawPressure > 845980000) {
        varioData.rawAltitude = 10000000 + ( 898760000 - varioData.rawPressure ) * 0.094732853  ;
      } else if ( varioData.rawPressure > 794980000) {
        varioData.rawAltitude = 15000000 + ( 845980000 - varioData.rawPressure ) *  0.098039216 ;
      } else if ( varioData.rawPressure > 746860000) {
        varioData.rawAltitude = 20000000 + ( 794980000 - varioData.rawPressure ) *  0.103906899 ;
      } else if ( varioData.rawPressure > 701120000) {
        varioData.rawAltitude = 25000000 + ( 746860000 - varioData.rawPressure ) *  0.109313511 ;
      } else if ( varioData.rawPressure > 657680000) {
        varioData.rawAltitude = 30000000 + ( 701120000 - varioData.rawPressure ) *  0.115101289 ;
      } else if ( varioData.rawPressure > 616450000) {
        varioData.rawAltitude = 35000000 + ( 657680000 - varioData.rawPressure ) *  0.121270919 ;
      } else if ( varioData.rawPressure > 577330000) {
        varioData.rawAltitude = 40000000 + ( 616450000 - varioData.rawPressure ) *  0.127811861 ;
      } else {
        varioData.rawAltitude = 45000000 + ( 577330000 - varioData.rawPressure ) *  0.134843581 ;
      }

      if (altitude == 0) {
        altitudeLowPass = altitudeHighPass = altitude = varioData.rawAltitude ;
        pressureMicrosPrev2 = pressureMicrosPrev1 - 20000 ;
      }

      altitude += 0.04 * (varioData.rawAltitude - altitude) ; // 0.04 default
      
      varioData.altitudeAt20MsecAvailable = true ; // inform that dTE can be performed

      altitudeLowPass += 0.085 * ( varioData.rawAltitude - altitudeLowPass) ;
      altitudeHighPass += 0.1 * ( varioData.rawAltitude - altitudeHighPass) ;
      if (pressureMicrosPrev1 > pressureMicrosPrev2 ) varioData.delaySmooth += 0.1 * ( pressureMicrosPrev1 -  pressureMicrosPrev2  - varioData.delaySmooth ) ; //delay between 2 measures  only if there is no overflow of pressureMicos
      climbRate2AltFloat = ((altitudeHighPass - altitudeLowPass )  * 5666.685 ) / varioData.delaySmooth;

      climbRateFloat += varioData.sensitivity * (climbRate2AltFloat - climbRateFloat)  * 0.001 ; // sensitivity is an integer and must be divided by 1000
      varioData.climbRate = (int32_t)  climbRateFloat;
      varioData.climbRateSM=kalmanVario.update(climbRateFloat);
      varioData.climbRateAvailable =  true; // allows SPORT protocol to transmit the value

      varioData.absoluteAlt = altitude / 100; // altitude is in m *10000 and AbsoluteAlt must be in m * 100
      varioData.absoluteAltAvailable = true;
      pressureMicrosPrev2 = pressureMicrosPrev1 ;
    }  // end of D1 > 0 & time > 3 sec
  } // End of process if SensorState was 0
} // End of readSensor







