#include "ethernet.h"
#include <EtherCard.h>
#include <EEPROM.h>
#include <TrueRandom.h>
#include <U8glib.h>
#include <OneWire.h>

/********************************************** OneWire *******************************************/

OneWire  ds(2);  // on pin 10 (a 4.7K resistor is necessary)
byte addr[8];
byte data[12];
float celsius;

/********************************************** ethernet *******************************************/

byte mac[6];
byte Ethernet::buffer[500];
BufferFiller bfill;
const char website[] PROGMEM = "www.google.com";
static byte myDefaultIp[] = { 10, 0, 0, 1 };



/********************************************** Oled Sreen *******************************************/

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

/********************************************** Setup *******************************************/

void setup() {

	InitScreen();
	Serial.begin(9600);
	InitMacAddress();

	/**	Ethernet	**/
	InitMacAddress();
	PrintMac(mac);
	EthernetSetup();


	
	if (!ether.dnsLookup(website))
		Serial.println("DNS failed");
		ether.printIp("SRV: ", ether.hisip);
		//ether.browseUrl(PSTR("/foo/"), "bar", website, my_callback);
		ether.browseUrl("/foo/", "bar", website, my_callback);

		/**	One Wire	**/
		initOWBus();
	CheckProbeType();
	PrintOWAddress();
}

void loop() {
	word len = ether.packetReceive();
	word pos = ether.packetLoop(len);
	if (pos)  // check if valid tcp data is received
	{
		ether.httpServerReply(homePage()); // send web page data
	}
	//Readtemp();
}
/********************************************** Ethernet function *******************************************/



/********************************************** Oled screen function *******************************************/
void InitScreen(){
	u8g.setFont(u8g_font_unifontr);
	Draw("init success", "OK");
	delay(600);
}

void Draw(String small, String big) {

	// picture loop
	u8g.firstPage();
	do {
		u8g.setPrintPos(0, 14);
		u8g.print(small);
		u8g.drawHLine(0, 16, 128);
		u8g.setScale2x2();
		u8g.setPrintPos(0, 24);
		u8g.print(big);
		u8g.undoScale();
	} while (u8g.nextPage());
	delay(50);
}

/********************************************** One Wire function *******************************************/

void PrintOWAddress() {

	byte i;
	Serial.print("OW Address =");
	for (i = 0; i < 8; i++) {

		Serial.write(' ');
		Serial.print(addr[i], HEX);
	}
}

byte CheckProbeType() {

	if (addr[0] != 0x28) {

		Serial.println("  Chip is not DS18B20");
		return 1;
	}	return 0;

}

void initOWBus() {

	ds.search(addr);
	ds.reset_search();
	ds.reset();
	ds.select(addr);
	if (OneWire::crc8(addr, 7) != addr[7]) {
	  Serial.println("CRC is not valid!");
	}
}

void OWdataPrinter(byte data[12]) {
	byte i;
	for (i = 0; i < 9; i++) {
		// we need 9 bytes
		Serial.print(data[i], HEX);
		Serial.print(" ");
	}
}


void Readtemp() {
	byte present = 0;
	byte i;
	ds.reset();
	ds.select(addr);
	ds.write(0x44, 1);
	// start conversion, with parasite power on at the end
	delay(1000);
	present = ds.reset();
	ds.select(addr);
	ds.write(0xBE);
	// Read Scratchpad
	Serial.print("  Data = ");
	Serial.print(present, HEX);
	Serial.print(" ");
	for (i = 0; i < 9; i++) {
		// we need 9 bytes
		data[i] = ds.read();
		//Serial.print(data[i], HEX);
		//Serial.print(" ");	}
		//Serial.print(" CRC=");
		//Serial.print(OneWire::crc8(data, 8), HEX);
		//Serial.println();
		int16_t raw = (data[1] << 8) | data[0]; 	byte cfg = (data[4] & 0x60);
		if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
		else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
		else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
		celsius = (float)raw / 16.0;
		//Serial.print("  Temperature = ");
		//Serial.print(celsius);
		//Serial.print(" Celsius, ");

	}
}