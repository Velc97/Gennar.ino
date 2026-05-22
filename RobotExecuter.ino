#include "RobotMovement.h"
#include "RobotBluetooth.h"
#include "UltrasonicSensor.h"

#pragma region Parameters

unsigned long previousUltrasonicMillis = 0; //For ultrasonic sensor scheduling
const unsigned long ultrasonicInterval = 3000; //Ultrasonic sensor scheduling interval in ms

#pragma endregion Parameters

#pragma region Arduino

void setup() {
  Serial.begin(9600);
  robotSetup();
  bleSetup();
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

  handleUltrasonicTask();

  /*delay(10000);
  Serial.println("Sending location");
  sendLocation(123456, 654321); */
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

#pragma endregion Methods

