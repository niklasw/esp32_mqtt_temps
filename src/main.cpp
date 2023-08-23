
#include <Arduino.h>
#include <OneWire.h>
#include "mywifi.h"
#include "config.h"
#include "mqtt.h"
#include "TSensors.h"
#include "MyMQTT.h"

int count = 0;

TSensors* tsPtr;
MyMQTT* mqttPtr;
TSensors& ts = *tsPtr;
MyMQTT& mqtt = *mqttPtr;

void setup(void)
{
    Serial.begin(BAUD);
    setup_wifi(ESSID, WEP_KEY);

    IP_t ip({MQTT_SERVER});
    String topic(MQTT_TOPIC);
    mqttPtr = new MyMQTT(ip, MQTT_PORT, topic);

    tsPtr = new TSensors(ONE_WIRE_BUS);

    tsPtr->mkTopics(MQTT_TOPIC, CONTROLLER_ID);
}

void loop(void)
{
    ts.info();

    ts.requestTemperatures();

    for (int i=0; i<ts.nSensors(); i++)
    {
        mqtt.publish(ts.topic(i), ts.mkMessage(i));
        ts.info(i);
    }

    Serial.println("Sleeping");

    delay(LOOP_PERIOD*1000);
}
