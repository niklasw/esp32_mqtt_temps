#include "TSensors.h"
#include <stdio.h>


// Private

String TSensors::formatAddress(const uint8_t rawAddr[])
{
    char tmp[3];
    sprintf(tmp, "%02X", rawAddr[0]);
    String out(tmp);

    for (uint8_t i=1; i<8; i++)
    {
        sprintf(tmp, ":%02X", rawAddr[i]);
        out.concat(tmp);
    }
    return out;
}

void TSensors::collectAddresses()
{
    Serial.println("Collecting sensor addresses");
    for (uint8_t i=0; i < _nSensors; i++)
    {
        uint8_t address[8];
        getAddress(address, i);
        _addresses[i] = formatAddress(address);
    }
}

// Public

TSensors::TSensors(uint8_t pin)
:   _w1(OneWire(pin)),
    DallasTemperature(&_w1),
    _nSensors(0)
{
    Serial.println("Setting up sensors");
    this->begin();
    delay(100);
    _nSensors = this->getDeviceCount();
    collectAddresses();

    for(int i=0; i< _nSensors; i++)
    {
        Serial.printf("  address: %s\n", this->address(i).c_str());
    }
}

void TSensors::mkTopics(const String& baseTopic)
{
    for(int i=0; i< _nSensors; i++)
    {
        char addr[100];
        sprintf(addr, "%s/%s", baseTopic.c_str(), _addresses[i].c_str());
        _topics[i] = String(addr);
    }
}

String TSensors::mkMessage(uint8_t i)
{
    if (this->temperature(i) > -100)
    {
        char message[100];
        const char fmt[] =
            "{\"index\":%d, \"address\": \"%s\", \"value\":%0.1f}";
        sprintf(message, fmt, i, this->address(i).c_str(), this->temperature(i));
        return String(message);
    }
    return String("offline");
}

const uint8_t& TSensors::nSensors() const
{
    return _nSensors;
}

const SensorAddressArray& TSensors::addresses() const
{
    return _addresses;
}

const String& TSensors::address(uint8_t i) const
{
    return _addresses[i];
}

const SensorAddressArray& TSensors::topics() const
{
    return _topics;
}

const String& TSensors::topic(uint8_t i) const
{
    return _topics[i];
}

float TSensors::temperature(uint8_t i)
{
    return this->getTempCByIndex(i);
}

void TSensors::info()
{
    Serial.printf("Number of sensors = %i\n", nSensors());

}

void TSensors::info(uint8_t i)
{
    Serial.printf("Sensor %s ", address(i).c_str());
    Serial.printf("temperature (device %d) = ", i);
    Serial.println(temperature(i));
}
