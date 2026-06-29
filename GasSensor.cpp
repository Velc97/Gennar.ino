#include <Arduino.h>

//Analogic pin for the gas sensor
int gasAnalogPin;

//Setup for the gas sensor
void setupGasSensor(int analogPin, long warmupTime) {
  gasAnalogPin = analogPin;
  delay(warmupTime); //warmup time in ms
}

//Gets gast percentage
uint16_t getGasPercentage() {
  return map(analogRead(gasAnalogPin), 0, 1023, 0, 100);
}

