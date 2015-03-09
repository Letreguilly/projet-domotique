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

	u8g.setFont(u8g_font_unifontr);
	Draw("255.255.255.255", " 13C 23%");
	Serial.begin(9600);
	if (EEPROM.read(1) == '#')
	{
		for (int i = 3; i < 6; i++)
		{
			mac[i] = EEPROM.read(i);
		}
	}
	else
	{
		for (int i = 3; i < 6; i++) {

			mac[i] = TrueRandom.randomByte();
			EEPROM.write(i, mac[i]);
		}
		EEPROM.write(1, '#');
	}

	/**	Ethernet	**/

	PrintMac(mac);
	Serial.println();
	if (ether.begin(sizeof Ethernet::buffer, mac, 10) == 0)  //10 spi slave pin
		Serial.println( "Failed to access Ethernet controller");
		DHCPsetup();
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
	Readtemp();
}
/********************************************** Ethernet function *******************************************/

void InitMacAddress() {
	if (EEPROM.read(1) == '#')
	{
		for (int i = 3; i < 6; i++) {

			mac[i] = EEPROM.read(i);
		}
	}
	else {
		for (int i = 3; i < 6; i++)
		{
			mac[i] = TrueRandom.randomByte();
			EEPROM.write(i, mac[i]);
		}		EEPROM.write(1, '#');
	}
}

static word homePage() {
	bfill = ether.tcpOffset();
	/*bfill.emit_p(PSTR(
		"HTTP/1.0 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"Pragma: no-cache\r\n"
		"\r\n"
		"<meta http-equiv='refresh' content='30'/>"
		"<title>Temp server</title>"
		"page de test"));*/
	bfill.emit_p(
		"HTTP/1.0 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"Pragma: no-cache\r\n"
		"\r\n"
		"<meta http-equiv='refresh' content='30'/>"
		"<title>Temp server</title>"
		"page de test");
	
	return bfill.position();

}

void PrintMac(byte macadd[6]) {
	Serial.print("MAC: ");
	for (byte i = 0; i < 6; ++i) {

		Serial.print(macadd[i], HEX);
		if (i < 5) {

			Serial.print(':');
		}
	}
}

void DHCPsetup() {

	Serial.println(F("Setting up DHCP"));
	if (!ether.dhcpSetup()) {

		//Serial.println(F("DHCP failed"));
	}
	//ether.printIp("My IP: ", ether.myip);
	//ether.printIp("Netmask: ", ether.netmask);
	//ether.printIp("GW IP: ", ether.gwip);
	//ether.printIp("DNS IP: ", ether.dnsip);
}

void Staticsetup() {

	Serial.println(F("Setting up DHCP"));
	if (!ether.staticSetup(myDefaultIp)) {

		Serial.println(F("IP Failed"));
	}
}


// called when the client request is complete?static
void my_callback(byte status, word off, word len) {

	Serial.println(">>>");
	Ethernet::buffer[off + 300] = 0;
	Serial.print((const char*) Ethernet::buffer + off);
	Serial.println("...");
}

/********************************************** Oled screen function *******************************************/
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