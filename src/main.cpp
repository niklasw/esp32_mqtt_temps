
#include <Arduino.h>
#include <OneWire.h>
#include "mywifi.h"
#include "config.h"
#include "t_sensors.h"
#include "mqtt.h"

int count = 0;

void setup(void)
{
  Serial.begin(BAUD);
  setup_wifi(ESSID, WEP_KEY);
  mqtt_setup();
  setup_sensors();
}

void loop(void)
{
  sensors.requestTemperatures();
  
  for (int i=0; i<n_sensors(); i++) {
    float temperature = sensors.getTempCByIndex(i);
    mqtt_pub(i, sensor_addresses()[i], temperature);

    Serial.printf("Sensor %s ", sensor_addresses()[i]);
    Serial.printf("temperature (device %d) = ", i);
    Serial.println(temperature);
  }

  delay(30*1000);
}
