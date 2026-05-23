#include <Arduino.h>

#pragma region Constants

const int uvPin = A1; //Analog pin for value reading
const int numSamples = 100; //Number of scans for average estimation

#pragma endregion Constants

#pragma region Methods

//Setup of the sensor pin
void setupUV() {
  Serial.println("Initializing UV sensor");
  pinMode(uvPin, INPUT);
  Serial.println("UV sensor initialized");
}

//Gets the UV index value from sensor
int getUVIndex() {

  //Average calculation for a more precise result
  long sum = 0;
  for (int i = 0; i < numSamples; i++) {
    sum += analogRead(uvPin);
    delay(5);
  }

  Serial.print("Sum: ");
  Serial.print(sum);
  Serial.print(" | ");



  //ADC and voltage calculation
  float adc = sum / (float)numSamples;
  float voltage = adc * (5.0 / 1023.0);

  //Calculating UV-index
  int uvIndex = 0;
  if (voltage < 0.05) uvIndex = 0;
  else if (voltage < 0.227) uvIndex = 1;
  else if (voltage < 0.318) uvIndex = 2;
  else if (voltage < 0.408) uvIndex = 3;
  else if (voltage < 0.503) uvIndex = 4;
  else if (voltage < 0.606) uvIndex = 5;
  else if (voltage < 0.696) uvIndex = 6;
  else if (voltage < 0.795) uvIndex = 7;
  else if (voltage < 0.881) uvIndex = 8;
  else if (voltage < 0.976) uvIndex = 9;
  else if (voltage < 1.079) uvIndex = 10;
  else uvIndex = 11;

  //ADC value
  Serial.print("ADC: ");
  Serial.print(adc);

  //Voltage value
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V");

  //Index value
  Serial.print(" | UV Index: ");
  Serial.println(uvIndex);

  return uvIndex;
}