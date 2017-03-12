#include "Arduino.h"
#include <IoTClient.h>

IoTItems::IoTItems() 
{
	this->_active   = 0;
	this->_itemName = NULL;
	this->callback  = NULL;
}

IoTClient::IoTClient(Client& client) : 
				_pubSubClient(client)
{
	_pubSubClient.setCallback(std::bind(&IoTClient::subCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)); 
	for (int ii=0;ii<MAX_ITEMS;ii++)
		this->_iotItems[ii]._active=0;
}

void IoTClient::setServer (const char *domain, uint16_t port, const char *serverTopic, const char *clientId) {
	this->_domain    = domain;
	this->_port      = port;
	this->_mqtt      = serverTopic;
	this->_clientId  = clientId;
	_pubSubClient.setServer(this->_domain, this->_port);

}

void IoTClient::setItem (int itemNo, const char *itemName, IoTClient_CALLBACK) {
	this->_iotItems[itemNo]._itemName = itemName;
	this->_iotItems[itemNo].callback = callback;
 	this->_iotItems[itemNo]._active   = 1;
}

void IoTClient::publish (const char *itemName, const char *parameter, const char *msg)
{
	char  clientTopic[128];
	sprintf(clientTopic, "/%s/%s/%s", this->_mqtt,itemName,parameter);
	_pubSubClient.publish(clientTopic,msg);
}



void IoTClient::subCallback (char* topic, byte* payload, unsigned int length)
{
	// Add a terminating \0 to the payload
	char   message[MQTT_MAX_PACKET_SIZE];
	
	for (int i = 0; i < length; i++) {
		message[i] = payload[i];
	}
	message[length]='\0';
	
#ifdef DEBUG
	Serial.printf("Message arrived [%s] : %s\n",topic,message);
#endif

	// Decode the item and parameter
	char  *topicDecode = &topic[strlen(this->_mqtt)+2];             // filter the MQTT Item
	char  *item        = strtok_r(topicDecode, "/", &topicDecode); // First one is the Item
	char  *action      = strtok_r(topicDecode, "/", &topicDecode); // Skip one

	for (int ii=0; ii<MAX_ITEMS; ii++) {
		if (this->_iotItems[ii]._active==1) {
			if ((strcmp(this->_iotItems[ii]._itemName,item) == 0) && (this->_iotItems[ii].callback))
				this->_iotItems[ii].callback(this->_iotItems[ii]._itemName,action,topicDecode,message);
		}
	}
}

void IoTClient::reconnect() {
  // Loop until we're reconnected
  while (!_pubSubClient.connected()) {
#ifdef DEBUG	 
    Serial.println("Attempting MQTT connection. ");
#endif
    // Attempt to connect
    if (_pubSubClient.connect(this->_clientId)) {
		// Once connected subscribe...
		for (int ii=0;ii<MAX_ITEMS;ii++) {
			if (this->_iotItems[ii]._active==1) {
				char  clientTopic[128];
				sprintf(clientTopic, "/%s/%s/set/#", this->_mqtt,this->_iotItems[ii]._itemName);	
#ifdef DEBUG
				Serial.printf("Subscribe to %s\n",clientTopic);
#endif
				_pubSubClient.subscribe(clientTopic);				
			}
		}
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
