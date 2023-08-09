#include <PubSubClient.h>
#include <stdio.h>

IPAddress server(192, 168, 10, 200);
WiFiClient interface;
PubSubClient mqtt_client(server, 1883, interface);

const char mqtt_topic[] = "sensors/T/esp32/1";

void mqtt_setup() {
  mqtt_client.setServer(server, 1883);
  delay(1500);
}

void mqtt_reconnect() {
  while (!mqtt_client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (mqtt_client.connect("arduinoClient")) {
      Serial.println("connected");
      mqtt_client.publish(mqtt_topic,"online");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqtt_mk_topic(const char* addr, char* result) {
  const char fmt[] = "%s/%s\0";
  sprintf(result, fmt, mqtt_topic, addr);
}

void mqtt_mk_message(const uint8_t index, const char* address,
                     const float value, char* result) {
  const char fmt[] = "{\"index\":%d, \"address\": \"%s\", \"value\":%0.1f}\0";
  sprintf(result, fmt, index, address, value);
}

void mqtt_pub(const uint8_t index, const char* address, const float value) {
    mqtt_reconnect();
    mqtt_client.loop();

    char topic[100];
    mqtt_mk_topic(address, topic);

    if (value > -100) {
      char message[100];
      mqtt_mk_message(index, address, value message);
      mqtt_client.publish(topic, message);
    } else {
      mqtt_client.publish(topic, "offline");
    }
}
