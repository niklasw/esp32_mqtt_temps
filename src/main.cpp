
#include <Arduino.h>
#include <OneWire.h>
#include "mywifi.h"
#include "config.h"
#include "mqtt.h"
#include "TSensors.h"
#include "MyMQTT.h"

int count = 0;

TSensors* tsPtr;

void setup(void)
{
    Serial.begin(BAUD);
    setup_wifi(ESSID, WEP_KEY);
    mqtt_setup();

    tsPtr = new TSensors(ONE_WIRE_BUS);
}

void loop(void)
{
    tsPtr->info();

    tsPtr->requestTemperatures();

    if (tsPtr->nSensors() > 0) {
        for (int i=0; i<tsPtr->nSensors(); i++) {
            mqtt_pub(i, tsPtr->address(i).c_str(), tsPtr->temperature(i));
            tsPtr->info(i);
        }
    }

    Serial.println("Sleeping");

    delay(LOOP_PERIOD*1000);
}
