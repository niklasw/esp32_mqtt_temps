
#include <Arduino.h>
#include <OneWire.h>
#include "mywifi.h"
#include "config.h"
#include "mqtt.h"
#include "TSensors.h"

int count = 0;

IPAddress server(MQTT_SERVER);
TSensors* tsPtr;

void setup(void)
{
    Serial.begin(BAUD);
    setup_wifi(ESSID, WEP_KEY);
    mqtt_setup(server, MQTT_PORT);

    tsPtr = new TSensors(ONE_WIRE_BUS);
    tsPtr->mkTopics(MQTT_TOPIC);
}

void loop(void)
{
    tsPtr->info();

    tsPtr->requestTemperatures();

    if (tsPtr->nSensors() > 0)
    {
        keep_wifi_connected();
        for (int i=0; i < tsPtr->nSensors(); i++)
        {
            mqtt_pub(tsPtr->topic(i).c_str(), tsPtr->mkMessage(i).c_str());
            tsPtr->info(i);
        }
    }

    Serial.println("Sleeping");

    delay(LOOP_PERIOD*1000);
}
