#include <EtherCard.h>
#include <EEPROM.h>
#include <TrueRandom.h>
#include <U8glib.h>
#include <OneWire.h>
#include <string.h>


/******************** Screen ************************/
class Screen
{
public:
	Screen();
	void Draw();
	void Draw(String message, String value);
private:
	U8GLIB_SSD1306_128X64 u8g;
	String message;
	String big;
};

/******************** Ethernet ************************/

class Ethernet_enc28j60
{
public:
	Ethernet_enc28j60();
	boolean InitEthernet(byte mymac[6]);
	boolean DHCPsetup();
	boolean Staticsetup();
	String GetIp();
	void pageNotfound();
	void pageTemperature();
	void pageSettings();
	String receiveData();
	void TestEthernet();
private:
	byte myDefaultIp[4] = { 10, 0, 0, 1 };
	
};

/******************** Mac address ************************/
class MacAddr
{
public:
	MacAddr();
	String MactoString();
	byte * value();
private:
	byte mac[6] = { 8, 21, 177, 0, 0, 0 };
};

/******************** ds18b20 ************************/
class OWTemp
{
public:
	OWTemp();
	float ReadtempAsync();
private:

};

/******************** Global variable ************************/
Screen * screen;
Ethernet_enc28j60 * ethernet;
MacAddr * mac;

OneWire ds(4);
byte addr[8];
OWTemp * OWD;
float celsius;

/******************** main ************************/
void setup() {
	pinMode(8, OUTPUT);
	digitalWrite(8, HIGH);
	
}


void loop() {	//loop forever
	run();
}

