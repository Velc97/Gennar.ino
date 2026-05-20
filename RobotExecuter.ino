#include "RobotMovement.h"
#include "RobotSensors.h"
#include "RobotBluetooth.h"

//#pragma region Parameters

//#pragma endregion Parameters

//#pragma region Arduino

void setup() {
  Serial.begin(9600);
  robotSetup();
  delay(3000);
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
}

//#pragma endregion Arduino

