#include <arduino.h>
#include "RobotMovement.h"

#pragma region Parameters

//Bluetooth response string
#define bluetoothTurnOff "+DISC:SUCCESS"

//Command type enumeration for message parsing
enum CommandType : byte {
  Location = 0x52,
  Move = (byte)'M',
};

#pragma endregion Parameters

#pragma region Methods

//Elaborates the received string input as a command
void elaborateCommand(const char* cmd) {

  //Safety check
  if (cmd == nullptr || strlen(cmd) == 0) {
      Serial.println("Empty command");
      return;
  }

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