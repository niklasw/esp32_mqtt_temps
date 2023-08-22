
#include "TSensors.h"


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
