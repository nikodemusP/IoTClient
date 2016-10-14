/*
 IoTClient.h - A base MQTT client for IOT.
*/

#ifndef IoTClient_h
#define IoTClient_h

#define DEBUG

#include <PubSubClient.h>
#include <ArduinoJson.h>

#ifdef ESP8266
#include <functional>
#define IoTClient_CALLBACK std::function<void(JsonObject& iotMsg)> callback
#else
#define IoTClient_CALLBACK void (*callback)(JsonObject& iotMsg)
#endif

class IoTClient {
	
private:
    PubSubClient	_pubSubClient;
	const char		*_domain;
	int					_port;
	const char		*_mqtt = "IoT";
    const char		*_clientId = "Demo"; 
	char				broadCastTopic[64];
	char				clientTopic[64];
	StaticJsonBuffer<200>  jsonBuffer;
	IoTClient_CALLBACK;

	void setOutTopics();
	void subCallback (char* topic, byte* payload, unsigned int length);
	
	void connectWifi();
	void connect();
	void reconnect();
	
public:
	IoTClient(Client& client);
	
	void setServer (const char* domain, uint16_t port, const char *serverTopic);
	void setServerTopic (const char *topic);
	void setClientId (const char *clientId);
	void setCallback(IoTClient_CALLBACK);
	
    void broadcast (const char *msg);
    void publish (const char *msg);
	
	void loop();
};

#endif