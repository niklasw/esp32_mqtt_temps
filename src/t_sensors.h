#include <DallasTemperature.h>
#include <Array.h>

#define ONE_WIRE_BUS 2
#define MAX_SENSORS 5
#define ADDR_LENGTH 24

typedef char sensorAddressArray[MAX_SENSORS][ADDR_LENGTH+1];

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const uint8_t* n_sensors_ptr = 0;
sensorAddressArray t_sensor_addresses;

void format_sensor_address(uint8_t array[8], char* result)
{
    sprintf(result, "%02X", array[0]);
    for (int i = 1; i < 8; i++) {
        char temp[3];
        sprintf(temp, ":%02X", array[i]);
        strcat(result, temp);
    }
    strcat(result, "\0");
}

void get_sensor_addresses(DallasTemperature& sensors, const uint8_t n_sensors,
                          sensorAddressArray addresses)
{
    for (int i=0; i<n_sensors; i++) {
        uint8_t address[8];
        sensors.getAddress(address, i);
        char address_string[ADDR_LENGTH+1];
        format_sensor_address(address, address_string);

        for (int j = 0; j < 8; j++) {
            strcpy(addresses[i], address_string);
        }

        Serial.print("  sensor address = ");
        Serial.print(address_string);
        Serial.println("");
    }
}

void setup_sensors()
{
    Serial.println("Setting up sensors");
    sensors.begin();
    delay(100);
    static const uint8_t n_sensors = sensors.getDeviceCount();
    Serial.printf("  number of sensors found: %i\n", n_sensors);
    n_sensors_ptr = &n_sensors;
    get_sensor_addresses(sensors, n_sensors, t_sensor_addresses);
    Serial.println("  sensors ready");
}

const int n_sensors()
{
    if (n_sensors_ptr && *n_sensors_ptr > 0) {
        return *n_sensors_ptr;
    }
    return 0;
}

const sensorAddressArray& sensor_addresses()
{
    return t_sensor_addresses;
}
