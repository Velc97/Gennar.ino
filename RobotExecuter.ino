#include "RobotMovement.h"
#include "RobotSensors.h"

char inputBuffer[32];

enum CommandType : byte {
  Location = 0x52,
  Move = (byte)'M',
};

//Bluetooth response string
#define bluetoothTurnOff "+DISC:SUCCESS"

#pragma region Arduino

void setup() {
  Serial.begin(9600);
  robotSetup();
  delay(3000);
}

void loop() {

  //Reading serial value
  if (Serial.available()) {
    
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

#pragma endregion Arduino

#pragma region Methods

//Elaborates the received string input as a command
void elaborateCommand(const char* cmd) {

  //Printing received command for debugging
  Serial.print("Received command: ");
  Serial.println(cmd);

  //Disconnection
  if (strcmp(cmd, bluetoothTurnOff) == 0) {
      move(0);  //Stopping the robot
    }

  //Printing
  char opcode = cmd[0];
  Serial.print("Parsing opcode: ");
  Serial.println(opcode);

  //Command parsing
  switch (opcode) {
    case Move:
      move(cmd[1]);
      break;
    default:
      Serial.println("Unknown command");
  }
}

#pragma endregion Methods