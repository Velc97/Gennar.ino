#include <Arduino.h>

//Analogic pin for the gas sensor
int gasAnalogPin;

//Setup for the gas sensor
void setupGasSensor(int analogPin, long warmupTime) {
  gasAnalogPin = analogPin;
  Serial.println("The sensor is warming up...");
  delay(warmupTime); //warmup time in ms
}

uint16_t getGasPercentage() {
  uint16_t gasVal = analogRead(gasAnalogPin);
  uint16_t gasPercentage = map(gasVal, 0, 1023, 0, 100);

  Serial.print("Gas value detected from sensor: ");
  Serial.println(gasVal);

  Serial.print("Gas percentage: ");
  Serial.println(gasPercentage);

  return gasPercentage;
}

