struct GPSValues
{
  uint32_t satellites;
  double latitude;
  double longitude;
  double speed;
  double altitude;
};

void setupGPS(uint8_t rxPin, uint8_t txPin);
GPSValues getGPSValues();