#define DEBUG 1 //For debugging purposses

#include <SoftwareSerial.h>
#include "RobotMovement.h"
#include "UltrasonicSensor.h"
#include "UVSensor.h"
#include "BarometricSensor.h"
#include "LumensSensor.h"
#include "GasSensor.h"
#include "HumiditySensor.h"
#include "GPS.h"

#pragma region Parameters

unsigned long previousUltrasonicMillis = 0; //For ultrasonic sensor scheduling
const unsigned long ultrasonicInterval = 3000; //Ultrasonic sensor scheduling interval in ms

SoftwareSerial bleSerial(-1, 8); //Software serial for sending data via BLE.

//Bluetooth response string
#define msgDisconnected "+DISC:SUCCESS\r"
#define msgConnected "CONNECTED\r"

//Command type enumeration for message parsing
enum CommandType : byte {
  Location = 0x52,
  Move = (byte)'M',
  Ultrasonic = (byte)'U'
};

#pragma endregion Parameters

#pragma region Arduino

void setup() {
  Serial.begin(9600);
  bleSerial.begin(9600);
  robotSetup();
  setupBarometricSensor(102500);
  setupLumensSensor();
  setupGasSensor(A0, 5000);
  setupDTH11Sensor(13);
  setupGPS(3, A3);
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
  getFullScan();

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

    Serial.print(F("Sending ultrasonic distance: "));
    Serial.println(distance);

    sendUltrasonicDistance(distance);
  }
}

//Obtains a full scan
void getFullScan() {
  Serial.print(F("Stopping the robot for scanning"));
  move(0);
  getUVIndex();
  getTemperature();
  getPressure();
  getAltitude();
  getLumens();
  getGasPercentage();

  DTH11Values dth11Values = getDTH11Values();
  Serial.println(F("DTH11 values:"));
  Serial.print(F("hdt: "));
  Serial.print(dth11Values.humidity);
  Serial.print(F(" temp: "));
  Serial.println(dth11Values.temperature);

  GPSValues gpsValues= getGPSValues();
  Serial.print(F("Sat: "));
  Serial.print(gpsValues.satellites);
  Serial.print(F(" Alt: "));
  Serial.print(gpsValues.altitude);
  Serial.print(F(" Lat: "));
  Serial.print(gpsValues.latitude);
  Serial.print(F(" Lon: "));
  Serial.print(gpsValues.longitude);
  Serial.print(F(" Spd: "));
  Serial.println(gpsValues.speed);
}


//Elaborates the received string input as a command
void elaborateCommand(const char* cmd) {

  //Safety check
  if (cmd == nullptr || strlen(cmd) == 0) {
      Serial.println(F("Empty command"));
      return;
  }

  //Printing received command for debugging
  Serial.print(F("Received command: "));
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
      Serial.println(F("stopped the robot for connection/disconnection"));
      return;
    }

  //Getting OPCODE
  char opcode = cmd[0];
  Serial.print(F("Parsing opcode: "));
  Serial.println(opcode);

  //Command parsing
  switch (opcode) {
    case Move:
      move(cmd[1]);
      break;
    default:
      Serial.println(F("Unknown command"));
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
    byte packet[3 + sizeof(int) * 2];

    packet[0] = (byte)CommandType::Location;
    memcpy(&packet[1], &lat, sizeof(int));
    memcpy(&packet[1 + sizeof(int)], &lon, sizeof(int));

    sendPacket(packet, sizeof(packet));
}

//Sends the ultrasonic sensor distance
void sendUltrasonicDistance(float distance) {
    byte packet[3 + sizeof(float)];

    packet[0] = (byte)CommandType::Ultrasonic;
    memcpy(&packet[1], &distance, sizeof(float));

    sendPacket(packet, sizeof(packet));
}

#pragma endregion Methods


