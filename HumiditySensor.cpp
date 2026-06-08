#include <dht_nonblocking.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11

DHT_nonblocking* dht_sensor = nullptr;

void setupDTH11Sensor(int pin)
{
    if (dht_sensor != nullptr)
    {
        delete dht_sensor;
    }

    dht_sensor = new DHT_nonblocking(pin, DHT_SENSOR_TYPE);
}

struct DTH11Values
{
    float temperature;
    float humidity;
};

DTH11Values getDTH11Values()
{
    DTH11Values data;

    if (dht_sensor == nullptr)
    {
        data.temperature = NAN;
        data.humidity = NAN;
        return data;
    }

    while (!dht_sensor->measure(&data.temperature, &data.humidity))
    {
        delay(1000);
    }

    return data;
}