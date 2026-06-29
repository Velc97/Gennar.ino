#include <BH1750.h>

BH1750 lightSensor;  //Light sensor

//Get lumens (lux) value of the light
float getLumens() {
  //Getting lux value
  float lux = lightSensor.readLightLevel();

  /* 
    Auto-adjusting the value, after the measurement the MTreg value is changed according to the result:
    lux > 40000 ==> MTreg =  32
    lux < 40000 ==> MTreg =  69  (default)
    lux <    10 ==> MTreg = 138

    MTreg is an internal register of the BH1750 sensor, called the Measurement Time register.
    It is used to adjust the integration time of the light measurement, meaning how long the sensor "observes" the brightness in order to calculate the lux value.
  */
  if (lux >= 0) {
    if (lux > 40000.0) {  //Reduce measurement time - needed in direct sun light
      lightSensor.setMTreg(32);
    } else {
      if (lux > 10.0) {  //Typical light environment
        lightSensor.setMTreg(69);
      } else {
        if (lux <= 10.0) {  //Very low light environment
          lightSensor.setMTreg(138);
        }
      }
    }
  }

  return lux;
}

//Sensor initialization
bool setupLumensSensor() {
  //If voltage > 0.7VCC then address will be 0x5C, otherwise 0x23
  if (lightSensor.begin()) {
    getLumens();
    return true;
  } else {
    return false;
  }
}