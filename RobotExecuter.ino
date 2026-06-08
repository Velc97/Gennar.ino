#define DEBUG 1 //For debugging purposses

#include "RobotMovement.h"
#include "RobotBluetooth.h"
#include "UltrasonicSensor.h"
#include "UVSensor.h"
#include "BarometricSensor.h"
#include "LumensSensor.h"
#include "GasSensor.h"
#include "HumiditySensor.h"

#pragma region Parameters

unsigned long previousUltrasonicMillis = 0; //For ultrasonic sensor scheduling
const unsigned long ultrasonicInterval = 3000; //Ultrasonic sensor scheduling interval in ms

#pragma endregion Parameters

#pragma region Arduino

void setup() {
  Serial.begin(9600);
  robotSetup();
  bleSetup();
  setupBarometricSensor(102500);
  setupLumensSensor();
  setupGasSensor(A0, 5000);
  setupDTH11Sensor(3);
}

void loop() {
  //Reading serial value
  if (Serial.available()) {

    //Buffer for received messages
    char inputBuffer[32];

    //Reading the buffer
    int len = Serial.readBytesUntil('\n', inputBuffer, sizeof(inputBuffer) - 1);
    inputBuffer[len] = '\0';

    //removing \r
    if (len > 0 && inputBuffer[len - 1] == '\r') {
      inputBuffer[len - 1] = '\0';
    }

    //Elaborating command
    elaborateCommand(inputBuffer);
  }

  handleUltrasonicTask();
  //getFullScan();

  /*delay(10000);
  Serial.println("Sending location");
  sendLocation(123456, 654321); */

  

  delay(3000);
}

#pragma endregion Arduino

#pragma region Methods

//Gets and sets ultrasonic distance from sensor to connected device
void handleUltrasonicTask() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousUltrasonicMillis >= ultrasonicInterval) {

    previousUltrasonicMillis = currentMillis;

    float distance = get_distance();

    Serial.print("Sending ultrasonic distance: ");
    Serial.println(distance);

    sendUltrasonicDistance(distance);
  }
}

//Obtains a full scan
void getFullScan() {
  Serial.print("Stopping the robot for scanning");
  move(0);
  getUVIndex();
  getTemperature();
  getPressure();
  getAltitude();
  getLumens();
  getGasPercentage();
  DTH11Values dth11Values = getDTH11Values();
}

#pragma endregion Methods


