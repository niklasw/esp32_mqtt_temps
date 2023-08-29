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

    String _topic;

    const String _identity;

public:
    MyMQTT(IP_t ip, int port, const char topic[]);

    void reconnect();

    const String& topic() const;

    void publish(const String& topic, const String& message);
};

#endif
