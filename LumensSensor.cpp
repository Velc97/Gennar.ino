#include <BH1750.h>

//Light sensor
BH1750 lightSensor;

//Get lumens (lux) value of the light
float getLumens() {
  //Getting lux value
  float lux = lightSensor.readLightLevel();
  Serial.print(F("Light: "));
  Serial.print(lux);
  Serial.println(F(" lx"));

  /* 
    Auto-adjusting the value, after the measurement the MTreg value is changed according to the result:
    lux > 40000 ==> MTreg =  32
    lux < 40000 ==> MTreg =  69  (default)
    lux <    10 ==> MTreg = 138

    MTreg is an internal register of the BH1750 sensor, called the Measurement Time register.
    It is used to adjust the integration time of the light measurement, meaning how long the sensor "observes" the brightness in order to calculate the lux value.
  */
  if (lux < 0) {
    Serial.println(F("Error condition detected"));
  } else {
    if (lux > 40000.0) {
      if (lightSensor.setMTreg(32)) {  //Reduce measurement time - needed in direct sun light
        Serial.println("Setting MTReg to low value for high light environment");
      } else {
        Serial.println("Error setting MTReg to low value for high light environment");
      }
    } else {
      if (lux > 10.0) {  //Typical light environment
        if (lightSensor.setMTreg(69)) {
          Serial.println("Setting MTReg to default value for normal light environment");
        } else {
          Serial.println("Error setting MTReg to default value for normal light environment");
        }
      } else {
        if (lux <= 10.0) {
          //Very low light environment
          if (lightSensor.setMTreg(138)) {
            Serial.println("Setting MTReg to high value for low light environment");
          } else {
            Serial.println("Error setting MTReg to high value for low light environment");
          }
        }
      }
    }
  }

  return lux;
}

//Sensor initialization
void setupLumensSensor() {
  //If voltage > 0.7VCC then address will be 0x5C, otherwise 0x23
  if (lightSensor.begin()) {
    Serial.println("BH1750  begin");
    getLumens();
  } else {
    Serial.println("Error initialising BH1750");
  }
}