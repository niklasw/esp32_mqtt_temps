#ifndef TSensors_h
#define TSensors_h

#include <Arduino.h>
#include <Array.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
#define MAX_SENSORS 5
#define ADDR_LENGTH 24

typedef Array<String, MAX_SENSORS> SensorAddressArray;

class TSensors
: public DallasTemperature
{
private:
    OneWire _w1;
    uint8_t _nSensors;
    SensorAddressArray _addresses;

    String formatAddress(const uint8_t rawAddr[]);

    void collectAddresses();

public:
    TSensors(uint8_t pin);

    const uint8_t& nSensors() const;

    const SensorAddressArray& addresses() const;

    const String& address(uint8_t i) const;

    float temperature(uint8_t i);

    void info();

    void info(uint8_t i);
};

#endif
