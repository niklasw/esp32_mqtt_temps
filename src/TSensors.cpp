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
}

void TSensors::mkTopics(const String& baseTopic, const String& id)
{
    for(const String& a: _addresses)
    {
        char addr[100];
        sprintf(addr, "%s/%s/%s\0", baseTopic.c_str(), id.c_str(), a.c_str());
        _topics.push_back(String(addr));
    }
}

String TSensors::mkMessage(uint8_t i)
{
    if (this->temperature(i) > -100)
    {
        char message[100];
        const char fmt[] =
            "{\"index\":%d, \"address\": \"%s\", \"value\":%0.1f}\0";
        sprintf(message, fmt, i, this->address(i), this->temperature(i));
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
