#include <EtherCard.h>
#include <EEPROM.h>
#include <TrueRandom.h>
#include <U8glib.h>
#include <OneWire.h>
#include <string.h>




  













class Screen
{
public:
	Screen();
	String message;
	String big;
	void InitScreen();
	void Draw();
private:
	U8GLIB_SSD1306_128X64 u8g;
	uint8_t ip[4];
};

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


class MacAddr
{
public:
	MacAddr();
	String MactoString();
	byte * value();
private:
	byte mac[6] = { 8, 21, 177, 0, 0, 0 };
};


Screen * screen;
Ethernet_enc28j60 * ethernet;
MacAddr * mac;

void setup() {
}


void loop() {	//loop forever


	/* Serial */
	Serial.begin(9600);

	/* Screen */
	screen = new Screen();
	screen->big = "start";
	screen->InitScreen();

	/* OneWire */
	InitOWBus();

	/* Mac */
	mac = new MacAddr();
	screen->message = mac->MactoString();
	screen->big = "  MyMac";
	screen->Draw();
	delay(3000);

	/* Ethernet */
	//byte macad[6] = mac->value
	ethernet = new Ethernet_enc28j60();
	if (ethernet->InitEthernet(mac->value()) == false) {
		screen->message = "access eth";
		screen->big = "failed";
		screen->Draw();
		delay(5000);
	}
	else if (ethernet->DHCPsetup() == false){
		screen->message = "dhcp fail";
		screen->big = " static ";
		screen->Draw();
		delay(5000);
		if (ethernet->Staticsetup() == false){
			screen->message = "  static eth";
			screen->big = " failed";
			screen->Draw();
			delay(5000);
		}
	}

	screen->message = " " + ethernet->GetIp();
	screen->big = "  MyIp";
	screen->Draw();
	delay(3000);


	for (;;) {

		String request = ethernet->receiveData();
		
		if (request.length() >= 1){
			pageChooser(request);
		}
		ReadtempAsync();
		screen->Draw();
	}
}

/**********************Other function**************************/

void pageChooser(String request){
	//page chooser
	if (request.indexOf("temperature") == 1) {
		ethernet->pageTemperature();
	}
	else if (request.indexOf("settings") == 1) {
		ethernet->pageSettings();
	}
	else {
		ethernet->pageNotfound();
	}
}

String getUrlParameter(String url, String parameter){

	int  pos = url.indexOf(parameter) + parameter.length() + 1;
	Serial.println(pos);
	String value;
	unsigned int i = 0;

	while (url.charAt(pos) != '?' && url.charAt(pos) != '&' && i < 10){
		value.concat(url.charAt(pos));
		pos++;
		i++;
	}


	return value;
}