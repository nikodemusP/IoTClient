/*
 IoTClient.h - A base MQTT client for IOT.
*/

#ifndef IoTClient_h
#define IoTClient_h

#define DEBUG

#include <PubSubClient.h>

#define MAX_ITEMS 4

#ifdef ESP8266
#include <functional>
#define IoTClient_CALLBACK std::function<void(const char *item, const char* action, const char *parameter, const char *iotMsg)> callback
#else
#define IoTClient_CALLBACK void (*callback)(const char *item, const char* action, const char *parameter, const char *iotMsg)
#endif

class IoTItems {
	public:
		int         _active;
		int         _itemNo;
		const char *_itemName;
		IoTClient_CALLBACK;

		IoTItems();
};
	
class IoTClient {
	
private:
    IoTItems        _iotItems[MAX_ITEMS];
    PubSubClient	_pubSubClient;
	const char		*_domain;
	int				_port;
	const char		*_mqtt     = "IoT";
    const char		*_clientId = "Demo"; 
	
	void subCallback (char* topic, byte* payload, unsigned int length);
	
	void connectWifi();
	void connect();
	void reconnect();
//	String getValue(String data, char separator, int index);
	
public:
	IoTClient(Client& client);
	
	void setServer   (const char* domain, uint16_t port, const char *serverTopic, const char *clientId);
	void setItem     (int itemNo, const char *clientId,IoTClient_CALLBACK);
	
    void publish (const char *itemName, const char *parameter, const char *msg);
	
	void loop();
};

#endif