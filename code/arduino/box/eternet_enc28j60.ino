const char serveur[] = "www.google.com";
byte mac[6] = { 8, 21, 177, 0, 0, 0 };
byte Ethernet::buffer[500];
static byte myDefaultIp[] = { 10, 0, 0, 1 };

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

	String m1 = String(mac[0], HEX);
	String m2 = String(mac[1], HEX);
	String m3 = String(mac[2], HEX);
	String m4 = String(mac[3], HEX);
	String m5 = String(mac[4], HEX);
	String m6 = String(mac[5], HEX);
	String m = String(":");

	message = m1 + m + m2 + m + m3 + m + m4 + m + m5 + m + m6;
	big = "mac addr";
	Draw();
	delay(5000);
}

static word homePage() {
	char charVal[10];
	dtostrf(celsius, 4, 1, charVal);

	BufferFiller bfill;
	bfill = ether.tcpOffset();
	bfill.emit_p(PSTR(
		"HTTP/1.0 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"Pragma: no-cache\r\n"
		"\r\n"
		"<meta http-equiv='refresh' content='30'/>"
		"<title>Temp server</title>"
		"temperature : $S"
		), charVal);


	return bfill.position();

}

void EthernetSetup(){
	if (ether.begin(sizeof Ethernet::buffer, mac, 10) == 0){
		message = "access eth";
		big = "failed";
		Draw();
		delay(5000);
	}
		
	DHCPsetup();
}

void DHCPsetup() {

	//Serial.println(F("Setting up DHCP"));
	if (!ether.dhcpSetup()) {
		message = "dhcp fail";
		big = " static ";
		Draw();
		delay(20000);
	}
	String o1 = String(ether.myip[0]);
	String o2 = String(ether.myip[1]);
	String o3 = String(ether.myip[2]);
	String o4 = String(ether.myip[3]);
	String p = ".";
	message = "  " + o1 + p + o2 + p + o3 + p + o4;
	big = "   IP   ";
	Draw();
	delay(5000);
}

void Staticsetup() {

	if (!ether.staticSetup(myDefaultIp)) {
		message = "  static eth";
		big = " failed";
		Draw();
		delay(5000);
	}
}




/******************************* Debug function *******************************/

void PrintMac() {
	Serial.print("MAC: ");
	for (byte i = 0; i < 6; ++i) {

		Serial.print(mac[i], HEX);
		if (i < 5) {

			Serial.print(':');
		}
	}
}

void TestEthernet(){
	ether.printIp("My IP: ", ether.myip);
	ether.printIp("Netmask: ", ether.netmask);
	ether.printIp("GW IP: ", ether.gwip);
	ether.printIp("DNS IP: ", ether.dnsip);
}


// called when the client request is complete?static
void my_callback(byte status, word off, word len) {

	//Serial.println(">>>");
	//Ethernet::buffer[off + 300] = 0;
	//Serial.print((const char*)Ethernet::buffer + off);
	//Serial.println("...");
}

void Get(){
	if (!ether.dnsLookup(serveur)){
		ether.printIp("SRV: ", ether.hisip);
		//ether.browseUrl(PSTR("/foo/"), "bar", website, my_callback);
		ether.browseUrl("/foo/", "bar", serveur, my_callback);
	}
}