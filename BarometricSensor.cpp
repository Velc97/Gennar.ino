#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

float seaLevelPressure; //Pa sea pressure

//Sensor setup
bool setupBarometricSensor(float value) {
  seaLevelPressure = value;

  if (!bmp.begin()) {
    return false;
  }
  
  return true;
}

//Temperature in Celsius degree
float getTemperature() {
  return bmp.readTemperature();
}

//Pressure in Pascal
long getPressure() {
  return bmp.readPressure();
}

//Altitude in Meters
float getAltitude() {
  return bmp.readAltitude(seaLevelPressure);
}