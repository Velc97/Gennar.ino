#define DEBUG 1 //For debugging purposes

#if DEBUG
  #define DBG_PRINT(x) Serial.print(x)
  #define DBG_PRINTLN(x) Serial.println(x)
#else
  #define DBG_PRINT(x)
  #define DBG_PRINTLN(x)
#endif

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
  //Serials begin
  Serial.begin(9600);
  bleSerial.begin(9600);

  //Robot setup
  DBG_PRINTLN(F("Init DRV8833 driver"));
  robotSetup();
  DBG_PRINTLN(F("Init DRV8833 driver done"));

  //UV sensor setup
  DBG_PRINTLN(F("Initializing UV sensor"));
  setupUV();
  DBG_PRINTLN(F("UV sensor initialized"));

  //Barometric sensor setup
  setupBarometricSensor(102500) ? DBG_PRINTLN(F("BMP085 initialized!")) : DBG_PRINTLN(F("BMP085 not found!"));

  //Luminosity sensor setup
  setupLumensSensor() ? DBG_PRINTLN(F("BH1750  initialized")) : DBG_PRINTLN(F("Error initialising BH1750"));

  DBG_PRINTLN(F("Gas sensor warming up..."));
  setupGasSensor(A0, 5000);
  DBG_PRINTLN(F("Gas sensor ready"));

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
  //getFullScan();

  /*delay(10000);
  DBG_PRINTLN(F("Sending location"));
  sendLocation(123456, 654321); */



  //delay(3000);
}

#pragma endregion Arduino

#pragma region Methods

//Gets and sets ultrasonic distance from sensor to connected device
void handleUltrasonicTask() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousUltrasonicMillis >= ultrasonicInterval) {

    previousUltrasonicMillis = currentMillis;

    float distance = get_distance();

    DBG_PRINT(F("Sending ultrasonic distance: "));
    DBG_PRINTLN(distance);

    sendUltrasonicDistance(distance);
  }
}

//Obtains a full scan
void getFullScan() {

  DBG_PRINT(F("Stopping the robot for scanning"));
  move(0);

  int UVIndex = getUVIndex();
  DBG_PRINT(F("UV Index: "));
  DBG_PRINTLN(UVIndex);
  
  float temperature = getTemperature();
  DBG_PRINT(F("Temperature = "));
  DBG_PRINTLN(F(" *C"));

  long pressure = getPressure();
  DBG_PRINT(F("Pressure = "));
  DBG_PRINTLN(F(" Pa"));

  float altitude = getAltitude();
  DBG_PRINT(F("Altitude = "));
  DBG_PRINTLN(F(" m"));

  float lumens = getLumens();
  if(lumens > 0) {
    DBG_PRINT(F("Light: "));
    DBG_PRINT(lumens);
    DBG_PRINTLN(F(" lx"));
  } else {
      DBG_PRINTLN(F("Error condition detected"));
  }
  
  uint16_t gas = getGasPercentage();
  DBG_PRINT(F("Gas percentage: "));
  DBG_PRINTLN(gas);

  DTH11Values dth11Values = getDTH11Values();
  DBG_PRINTLN(F("DTH11 values:"));
  DBG_PRINT(F("hdt: "));
  DBG_PRINT(dth11Values.humidity);
  DBG_PRINT(F(" temp: "));
  DBG_PRINTLN(dth11Values.temperature);

  GPSValues gpsValues= getGPSValues();
  DBG_PRINT(F("Sat: "));
  DBG_PRINT(gpsValues.satellites);
  DBG_PRINT(F(" Alt: "));
  DBG_PRINT(gpsValues.altitude);
  DBG_PRINT(F(" Lat: "));
  DBG_PRINT(gpsValues.latitude);
  DBG_PRINT(F(" Lon: "));
  DBG_PRINT(gpsValues.longitude);
  DBG_PRINT(F(" Spd: "));
  DBG_PRINTLN(gpsValues.speed);

  DBG_PRINTLN(F("----------- END SCAN  -----------"));
}


//Elaborates the received string input as a command
void elaborateCommand(const char* cmd) {

  //Safety check
  if (cmd == nullptr || strlen(cmd) == 0) {
      DBG_PRINTLN(F("Empty command"));
      return;
  }

  //Printing received command for debugging
  DBG_PRINT(F("Received command: "));
  DBG_PRINTLN(cmd);

  //In-depth command debug
  /*for (int i = 0; i < strlen(cmd); i++) {
      DBG_PRINT((int)cmd[i]);
      DBG_PRINT(F(" "));
  }
  DBG_PRINTLN();
  DBG_PRINT(F("LEN: "));
  DBG_PRINTLN(strlen(cmd));*/

  //Disconnection or connection
  if (strcmp(cmd, msgDisconnected) == 0 || strcmp(cmd, msgConnected) == 0) {
      move(0);  //Stopping the robot
      DBG_PRINTLN(F("stopped the robot for connection/disconnection"));
      return;
    }

  //Getting OPCODE
  char opcode = cmd[0];
  DBG_PRINT(F("Parsing opcode: "));
  DBG_PRINTLN(opcode);

  //Command parsing
  switch (opcode) {

    case Move:
      DBG_PRINT(F("Moving with: "));
      DBG_PRINTLN(cmd[1]);
      move(cmd[1]);
      break;

    default:
      DBG_PRINTLN(F("Unknown command"));
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


