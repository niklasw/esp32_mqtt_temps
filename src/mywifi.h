#include <WiFi.h>

void setup_wifi(const char* ssid, const char* password) {
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