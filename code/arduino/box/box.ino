#include <EtherCard.h>
#include <EEPROM.h>
#include <TrueRandom.h>
#include <U8glib.h>
#include <OneWire.h>

String message;
String big;

void setup() {

	InitScreen();
	Serial.begin(9600);


	/**	Ethernet	**/
	InitMacAddress();
	
	EthernetSetup();
	//TestEthernet();
	

	
	
	/**	One Wire	**/
	initOWBus();
}

void loop() {
	word len = ether.packetReceive();
	word pos = ether.packetLoop(len);
	if (pos)  // check if valid tcp data is received
	{
		ether.httpServerReply(homePage()); // send web page data
		delay(500);
	}
	Readtemp();
}
