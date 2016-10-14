#include <IoTLgAC.h>

IoTLgAC::IoTLgAC(int acType) : 
					irsend(4) 
{
	this->acType = acType;
}

void IoTLgAC::setup() {
		irsend.begin();
}

void IoTLgAC::loop() {
}

void IoTLgAC::sendCode(unsigned long code)
{
	Serial.print("send code "); Serial.println(code,HEX);
	irsend.sendLG(code, 28);
}	
	
void  IoTLgAC::activate()
{
	 this->acPower=1;
	
	 int acMsBits1 = 8;
	 int acMsBits2 = 8;
	 int acMsBits3 = 0;
	 int acMsBits4 = 0;
	 if ( this->acMode == LG_AC_MODE_HEADING ) 
		acMsBits4 = 4;
	 int acMsBits5 = this->acTemperature - 15;
	 int acMsBits6;

	 if ( this->acType == LG_AC_TYPE_TOWER) {
		acMsBits6 = AC_FLOW_TOWER[this->acFlow];
	 } else {
		acMsBits6 = AC_FLOW_WALL[this->acFlow];
	 }

	 int acMsBits7 = (acMsBits3 + acMsBits4 + acMsBits5 + acMsBits6) & B00001111;

	 int code2Send  =  acMsBits1 << 4 ;
	 code2Send =  (code2Send + acMsBits2) << 4;
	 code2Send =  (code2Send + acMsBits3) << 4;
	 code2Send =  (code2Send + acMsBits4) << 4;
	 code2Send =  (code2Send + acMsBits5) << 4;
	 code2Send =  (code2Send + acMsBits6) << 4;
	 code2Send =  (code2Send + acMsBits7);

	 sendCode(code2Send);
}

void IoTLgAC::powerOn()
{
	activate();
}

void IoTLgAC::powerOff()
{
	int code2send = 0x88C0051;
	sendCode(code2send);
	this->acPower = 0;
}

void IoTLgAC::setAirSwing(int airSwing)
{
	int code2send = 0;
	if ( this->acType == LG_AC_TYPE_TOWER) {
		if ( airSwing == LG_AC_FAN_SWING_ON) {
			code2send = 0x881316B;
		} else {
			code2send = 0x881317C;
		}
	} else {
		if ( airSwing == LG_AC_FAN_SWING_ON) {
			code2send = 0x8813149;
		} else {
			code2send = 0x881315A;
		}
	}
	sendCode(code2send);
}


void IoTLgAC::setAirClean(int airClean)
{
	int code2send = 0;
	if ( airClean == LG_AC_AIR_CLEAN_ON) {
		code2send = 0x88C000C;
	} else {
		code2send = 0x88C0084;
	}
	sendCode(code2send);
}

void IoTLgAC::setTemperature(int temperature)
{
	this->acTemperature = temperature;
	activate();
}

void IoTLgAC::setAirFlow(int airFlow)
{
	this->acFlow = airFlow;
	activate();
}

void IoTLgAC::setParameter(JsonObject& iotMsg)
{
	if (iotMsg.containsKey("temp"))
	{
		int value = iotMsg["temp"];
		setTemperature(value);
	}
	if (iotMsg.containsKey("airClean"))
	{
		int value = iotMsg["airClean"];
		setAirClean(value);
	}
	if (iotMsg.containsKey("airSwing"))
	{
		int value = iotMsg["airSwing"];
		setAirSwing(value);
	}
	if (iotMsg.containsKey("airFlow"))
	{
		int value = iotMsg["airFlow"];
		setAirFlow(value);
	}
	if (iotMsg.containsKey("power"))
	{
		String value = iotMsg["power"];
		if (value.equals("ON"))
			powerOn();
		if (value.equals("OFF"))
			powerOff();
	}	
}
