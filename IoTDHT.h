#ifndef IoTDHT_h
#define IoTDHT_h

#include "Arduino.h"
#include "DHT.h"
#include "IoTClient.h"

#define DHTTYPE_DHT11       1
#define DHTTYPE_DHT21       2
#define DHTTYPE_DHT22       3

class IoTDHT {

private:
	DHT       dht;
	IoTClient *_iotClient;
	
public:
    IoTDHT(IoTClient *iotClient, int DHTPin, int DHTType);
	void setup();
	void loop();
	void setParameter(const char* parameter, const char* value);
	void getParameter(const char* parameter);
};
#endif

