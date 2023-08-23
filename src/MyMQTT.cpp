
#include "MyMQTT.h"

MyMQTT::MyMQTT(IP_t ip, int port, String topic)
:
    _server(ip[0], ip[1], ip[2], ip[3]),
    _interface(),
    PubSubClient(_server, port, _interface),
    _topic(topic),
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
        if (this->connect(_identity.c_str())) {
            Serial.println("connected");
            this->publish(_topic.c_str(), "online");
        } else {
            Serial.print("failed, rc=");
            Serial.print(this->state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
};

const String& MyMQTT::topic() const
{
    return _topic;
};

void MyMQTT::publish(const String& topic, const String& message)
{
    this->reconnect();
    this->loop();
    this->publish(topic.c_str(), message.c_str());
};
