
#include <Arduino.h>
#include <OneWire.h>
#include "mywifi.h"
#include "config.h"
#include "TSensors.h"
#include "MyMQTT.h"

int count = 0;

TSensors* tsPtr;
MyMQTT* mqttPtr;

void setup(void)
{
    Serial.begin(BAUD);
    setup_wifi(ESSID, WEP_KEY);

    IP_t ip({MQTT_SERVER});
    String topic(MQTT_TOPIC);
    mqttPtr = new MyMQTT(ip, MQTT_PORT, topic);

    tsPtr = new TSensors(ONE_WIRE_BUS);

    tsPtr->mkTopics(MQTT_TOPIC);

    for (const String& t: tsPtr->topics())
    {
        Serial.printf("Topic: ");
        Serial.println(t);
    }
}

void loop(void)
{
    tsPtr->info();

    tsPtr->requestTemperatures();

    for (int i=0; i<tsPtr->nSensors(); i++)
    {
        // mqttPtr->publish(tsPtr->topic(i), tsPtr->mkMessage(i));
        mqttPtr->publish(String("sensors/T/esp32/2/T"), String("apa"));

        tsPtr->info(i);
    }

    Serial.println("Sleeping");

    delay(LOOP_PERIOD*1000);
}
