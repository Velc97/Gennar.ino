#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

float seaLevelPressure; //Pa sea pressure

void setupBarometricSensor(float value) {
  seaLevelPressure = value;

  if (!bmp.begin()) {
    Serial.println("BMP085 not found!");
  }
}

//Temperature
float getTemperature() {
  float t = bmp.readTemperature();

  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.println(" *C");

  return t;
}

//Pressure
long getPressure() {
  long p = bmp.readPressure();

  Serial.print("Pressure = ");
  Serial.print(p);
  Serial.println(" Pa");

  return p;
}

//Altitude
float getAltitude() {
  long pressure = bmp.readPressure();

  //Usa pressione standard o calibrata
  float altitude = bmp.readAltitude(seaLevelPressure);

  Serial.print("Altitude = ");
  Serial.print(altitude);
  Serial.println(" m");

  return altitude;
}