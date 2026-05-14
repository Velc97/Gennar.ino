#include "RobotMovement.h"
#include "RobotSensors.h"

#pragma region Arduino

void setup() {
  Serial.begin(9600);
  robotSetup();
  delay(3000);
}

void loop() {
  //Reading serial value
  if (Serial.available()) {
    elaborateCommand(Serial.readString());
  }

  //Serial.print("Distance: ");
  //Serial.println(get_distance());
}

#pragma endregion Arduino