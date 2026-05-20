#include <arduino.h>
#include <SoftwareSerial.h>
#include "RobotMovement.h"
#include "UltrasonicSensor.h"

#pragma region Parameters

//Software serial for sending data via BLE.
SoftwareSerial bleSerial(-1, 8);

//Bluetooth response string
#define msgDisconnected "+DISC:SUCCESS\r"
#define msgConnected "CONNECTED\r"

//Command type enumeration for message parsing
enum CommandType : byte {
  Location = 0x52,
  Move = (byte)'M'
};

#pragma endregion Parameters

#pragma region Methods

void bleSetup() {
  bleSerial.begin(9600);
}

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

  //In-depth command debug
  /*for (int i = 0; i < strlen(cmd); i++) {
      Serial.print((int)cmd[i]);
      Serial.print(" ");
  }
  Serial.println();
  Serial.print("LEN: ");
  Serial.println(strlen(cmd));*/

  //Disconnection or connection
  if (strcmp(cmd, msgDisconnected) == 0 || strcmp(cmd, msgConnected) == 0) {
      move(0);  //Stopping the robot
      Serial.println("stopped the robot for connection/disconnection");
      return;
    }

  //Getting OPCODE
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

//Wrapper for sending packet with message terminators "\r\n"
void sendPacket(byte* data, size_t len) {
    data[len-2] = '\r';
    data[len-1] = '\n';

    bleSerial.write(data, len);
}

void sendLocation(int lat, int lon)
{
    byte packet[11];

    packet[0] = (byte)CommandType::Location;

    memcpy(&packet[1], &lat, 4);
    memcpy(&packet[5], &lon, 4);

    sendPacket(packet, sizeof(packet));
}





#pragma endregion Methods