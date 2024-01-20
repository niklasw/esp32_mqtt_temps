#include <WiFi.h>

void keep_wifi_connected()
{
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("WiFi lost. Reconnecting");
        WiFi.reconnect();
    }
}

void setup_wifi(const char* ssid, const char* password)
{
    delay(10);
    Serial.println("Setting up WiFi");
    Serial.println();
    Serial.print("WiFi connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}