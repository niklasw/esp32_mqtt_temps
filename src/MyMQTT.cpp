
#include "MyMQTT.h"

MyMQTT::MyMQTT(IP_t ip, int port, String topic)
:
    _server(ip[0], ip[1], ip[2], ip[3]),
    _interface(),
    PubSubClient(_server, port, _interface),
    _baseTopic(topic),
    _identity("arduinoClient")
{
    this->setServer(_server, port);
    delay(1500);
};

void MyMQTT::reconnect()
{
    while (!this->connected())
    {
        Serial.print("Attempting MyMQTT connection...");
        if (this->connect(_identity)) {
            Serial.println("connected");
            this->publish(_baseTopic.c_str(), "online");
        } else {
            Serial.print("failed, rc=");
            Serial.print(this->state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
};

String MyMQTT::mkTopic(const Array<String, 4> subTopics)
{
    String topic(_baseTopic);

    for (const String& s: subTopics)
    {
        topic.concat(s);
    }
    return topic;
};
