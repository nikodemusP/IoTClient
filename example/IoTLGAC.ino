#include <ESP8266WiFi.h>
#include "IoTClient.h"
#include "IoTLgAC.h"

const char* ssid = "nikolas";
const char* password = "B1I5A0N9C0A8";
const char* mqtt_server = "192.168.0.50";
const int   port = 1883;
const char* clientId = "TestClient";

WiFiClient espClient;
IoTClient iotClient(espClient);
IoTLgAC iotAC(LG_AC_TYPE_TOWER);

void setup_wifi() {
  Serial.begin(115200);
  delay(1000);
  Serial.printf("Connecting to %s\n",ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);
  Serial.print("WiFi connected IP address: ");Serial.println(WiFi.localIP());
}

void callback (JsonObject& iotMsg)
{
  iotAC.setParameter(iotMsg);
}

void setup() {
  setup_wifi();
  iotClient.setServer(mqtt_server,port,"ownhome");
  iotClient.setClientId(clientId);
  iotClient.setCallback(callback);
  iotAC.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  iotClient.loop();
  iotAC.loop();
  delay(1000);
}
