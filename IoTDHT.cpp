#include <IoTDHT.h>
#include "DHT.h"
#define DEBUG 1

IoTDHT::IoTDHT(IoTClient *iotClient, int DHTPin, int DHTType) : 
	dht(DHTPin, DHTType)
{
	_iotClient = iotClient;
	return;
}

void IoTDHT::setup() {
	dht.begin();
}

void IoTDHT::loop() {
	float h = dht.readHumidity();
	float t = dht.readTemperature();
	char  buffer[32];
#ifdef DEBUG
	Serial.print("Temperatur :");Serial.println(t);
	Serial.print("Humidity   :");Serial.println(h);
#endif
	dtostrf(t, 4, 2, buffer);
	_iotClient->publish("DHT","temp",buffer);
	dtostrf(h, 4, 2, buffer);
	_iotClient->publish("DHT","humity",buffer);
}

void IoTDHT::setParameter(const char* parameter, const char* value)
{
}

void IoTDHT::getParameter(const char* parameter)
{
}
