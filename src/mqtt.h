#include <PubSubClient.h>

WiFiClient interface;
PubSubClient mqtt_client(interface);

void mqtt_setup(const IPAddress&  server, int port)
{
    mqtt_client.setServer(server, port);
    delay(1500);
}

void mqtt_reconnect(const char* topic)
{
    while (!mqtt_client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        if (mqtt_client.connect("arduinoClient")) {
            Serial.println("connected");
            mqtt_client.publish(topic,"online");
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void mqtt_pub(const char* topic, const char* message)
{
    mqtt_reconnect(topic);
    mqtt_client.loop();
    mqtt_client.publish(topic, message);
}
