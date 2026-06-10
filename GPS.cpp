#include "TinyGPS++.h"
#include "SoftwareSerial.h"

SoftwareSerial* serial_connection = nullptr;
TinyGPSPlus gps;

//GPS Values
struct GPSValues
{
  uint32_t satellites;
  double latitude;
  double longitude;
  double speed;
  double altitude;
};

//Setups the GPS
void setupGPS(uint8_t rxPin, uint8_t txPin) {

  if (serial_connection != nullptr)
  {
    delete serial_connection;
  }

  serial_connection = new SoftwareSerial(rxPin, txPin);
  serial_connection->begin(9600);
}

//Gets the GPS scanned values
GPSValues getGPSValues() {

  GPSValues data = {
    0,
    NAN,
    NAN,
    NAN,
    NAN
  };

  if (serial_connection == nullptr)
  {
    Serial.print(F("Serial not available"));
    return data;
  }

  while (serial_connection->available() > 0)
  {
    if (gps.encode(serial_connection->read()))
    {
      data.satellites = gps.satellites.value();
      data.latitude = gps.location.lat();
      data.longitude = gps.location.lng();
      data.speed = gps.speed.kmph();
      data.altitude = gps.altitude.meters();
    }
  }

  return data;
}