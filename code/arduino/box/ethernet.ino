
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
	bfill.emit_p(PSTR(
		"HTTP/1.0 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"Pragma: no-cache\r\n"
		"\r\n"
		"<meta http-equiv='refresh' content='30'/>"
		"<title>Temp server</title>"
		"page de test"));


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

void EthernetSetup(){
	if (ether.begin(sizeof Ethernet::buffer, mac, 10) == 0)
		Draw("Ethernet failed", "eth fail");
	DHCPsetup();
}

void DHCPsetup() {

	Serial.println(F("Setting up DHCP"));
	if (!ether.dhcpSetup()) {
		Draw("dhcp failed", "warning");
	}
}

void Staticsetup() {

	if (!ether.staticSetup(myDefaultIp)) {
		Draw("Ethernet failed", "eth fail");
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

	Serial.println(">>>");
	Ethernet::buffer[off + 300] = 0;
	Serial.print((const char*)Ethernet::buffer + off);
	Serial.println("...");
}