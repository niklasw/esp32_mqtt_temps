#ifndef MyMQTT_H
#define MyMQTT_H

#include <Arduino.h>
#include <Array.h>
#include <WiFi.h>
#include <PubSubClient.h>

typedef Array<uint8_t, 4> IP_t;

class MyMQTT
: public PubSubClient
{
private:
    const IPAddress _server;

    WiFiClient _interface;

    String _baseTopic;

    const char* _identity;

public:
    MyMQTT(IP_t ip, int port, String topic);

    void reconnect();

    String mkTopic(const Array<String, 4> subTopics);
};

#endif
