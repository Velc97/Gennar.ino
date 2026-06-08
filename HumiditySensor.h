
struct DTH11Values
{
    float temperature;
    float humidity;
};

void setupDTH11Sensor(int pin);
DTH11Values getDTH11Values();
