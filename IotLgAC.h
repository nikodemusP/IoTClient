#ifndef IoTLgAC_h
#define IoTLgAC_h

#include "Arduino.h"
#include <IRremoteESP8266.h>
#include <ArduinoJson.h>

#define LG_AC_TYPE_TOWER  0
#define LG_AC_TYPE_WALL    1

#define LG_AC_MODE_COOLING        0
#define LG_AC_MODE_HEADING        1

#define LG_AC_POWER_OFF             0
#define LG_AC_POWER_ON              1

#define LG_AC_AIR_CLEAN_OFF        0
#define LG_AC_AIR_CLEAN_ON         1

#define LG_AC_FAN_SWING_ON        0
#define LG_AC_FAN_SWING_OFF       1

class IoTLgAC {

private:

	IRsend irsend;

    int acType = LG_AC_TYPE_TOWER;;
	int acMode = LG_AC_MODE_COOLING;
	int acPower = LG_AC_POWER_OFF;
	int acAirClean = LG_AC_AIR_CLEAN_OFF;
	int acTemperature = 27;
	int acFlow = 1;
    const int AC_FLOW_TOWER[3] = {0, 4, 6};
    const int AC_FLOW_WALL[4]  = {0, 2, 4, 5};

	void sendCode(unsigned long code);	
	void activate();

public:
    IoTLgAC(int acType);
	void setup();
	void loop();
	void powerOn();
	void powerOff();
	void setAirSwing(int airSwing);
	void setAirClean(int airClean);
	void setTemperature(int temperature);
	void setAirFlow(int airFlow);
	void setParameter(JsonObject& iotMsg);
};
#endif

