#include "Arduino.h"
#include <IoTClient.h>

IoTClient::IoTClient(Client& client) : 
				_pubSubClient(client)
{
	_pubSubClient.setCallback(std::bind(&IoTClient::subCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)); 
	this->callback = NULL;
}

void IoTClient::setServer (const char *domain, uint16_t port, const char *serverTopic) {
	this->_domain    = domain;
	this->_port        = port;
	this->_mqtt       = serverTopic;
	_pubSubClient.setServer(this->_domain, this->_port);
}

void IoTClient::setOutTopics()
{
	sprintf(this->broadCastTopic, "%s/Broadcast",this->_mqtt);	
	sprintf(this->clientTopic, "%s/%s", this->_mqtt,this->_clientId);	
}

void IoTClient::setServerTopic (const char *serverTopic) {
	this->_mqtt = serverTopic;
	setOutTopics();
}

void IoTClient::setClientId (const char *clientId) {
	this->_clientId = clientId;
	setOutTopics();
}

void IoTClient::setCallback(IoTClient_CALLBACK) {
	this->callback = callback;
}

void IoTClient::broadcast (const char *msg)
{
	char message[MQTT_MAX_PACKET_SIZE];
	sprintf(message,"\"%s\" : {%s}", this->_clientId, msg);
	_pubSubClient.publish(this->broadCastTopic,message);
}

void IoTClient::publish (const char *msg)
{
	_pubSubClient.publish(this->clientTopic,msg);
}

void IoTClient::subCallback (char* topic, byte* payload, unsigned int length)
{
	// Add a terminating \0 to the payload
	char message[MQTT_MAX_PACKET_SIZE+1];
	for (int i = 0; i < length; i++) {
		message[i] = payload[i];
	}
	message[length]='\0';

#ifdef DEBUG
	Serial.printf("Message arrived [%s] : %s\n",topic,message);
#endif

	JsonObject& root = jsonBuffer.parseObject(message);
	if (!root.success())
	{
#ifdef DEBUG
	  Serial.println("parseObject() failed");
#endif
	  return;
	}
	if (callback)
		callback(root);
}

void IoTClient::reconnect() {
  // Loop until we're reconnected
  while (!_pubSubClient.connected()) {
#ifdef DEBUG	 
    Serial.print("Attempting MQTT connection. ");
#endif
    // Attempt to connect
    if (_pubSubClient.connect(this->_clientId)) {
#ifdef DEBUG
		Serial.printf("connected\nSubscribe to %s, %s\n",this->broadCastTopic,this->clientTopic);
#endif
		// Once connected, publish an announcement and subscribe...
		broadcast("\"state\" : \"alive\"");		
		_pubSubClient.subscribe(this->broadCastTopic);
		_pubSubClient.subscribe(this->clientTopic);
    } else {
#ifdef DEBUG
      Serial.printf("failed, rc=%i, try again in 5 seconds\n",_pubSubClient.state());
#endif
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void IoTClient::loop() {
	if (!_pubSubClient.connected()) {
		reconnect();
	}
	_pubSubClient.loop();
}
