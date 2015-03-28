byte Ethernet::buffer[700];

Ethernet_enc28j60::Ethernet_enc28j60(){

}



boolean Ethernet_enc28j60::InitEthernet(byte mymac[6]) {
	if (ether.begin(700, mymac, 10) == 0) {
		return false;
	}
	return true;
}

boolean Ethernet_enc28j60::DHCPsetup() {
	if (!ether.dhcpSetup()) {		
		return false;
	}
	ether.myip;
	return true;
}

boolean Ethernet_enc28j60::Staticsetup() {

	if (!ether.staticSetup(myDefaultIp)) {
		return false;
	}
	return true;
}

String Ethernet_enc28j60::GetIp() {

	String o1 = String(ether.myip[0]);
	String o2 = String(ether.myip[1]);
	String o3 = String(ether.myip[2]);
	String o4 = String(ether.myip[3]);
	String p = ".";
	return o1 + p + o2 + p + o3 + p + o4;
}

void Ethernet_enc28j60::pageNotfound() {
	BufferFiller bfill;
	bfill = ether.tcpOffset();
	bfill.emit_p(PSTR(
		"HTTP/1.0 404 Not Found\r\n"
		"Content-Type: text/html\r\n"
		"Pragma: no-cache\r\n"
		"\r\n"
		"<title>404</title>"
		"<h1>Not Found <br>Error 404</H1>"
		));
	ether.httpServerReply(bfill.position());
}

void Ethernet_enc28j60::pageTemperature() {
	char charVal[10];
	dtostrf(celsius, 4, 1, charVal);

	BufferFiller bfill;
	bfill = ether.tcpOffset();
	bfill.emit_p(PSTR(
		"HTTP/1.0 20 OK\r\n"
		"Content-Type: text/html\r\n"
		"Pragma: no-cache\r\n"
		"\r\n"
		"<title>Temp server</title>"
		"temperature : $S"
		), charVal);
	ether.httpServerReply(bfill.position());
}

void Ethernet_enc28j60::pageSettings() {
	BufferFiller bfill;
	bfill = ether.tcpOffset();
	bfill.emit_p(PSTR(
		"HTTP/1.0 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"Pragma: no-cache\r\n"
		"\r\n"
		"<title>Temp Server settings</title>"
		"<form action=\"settings.html\">"
		"First name : <br>"
		"<input type=\"text\" name=\"firstname\">"
		"<br>"
		"Last name : <br>"
		"<input type=\"text\" name=\"Lastname\">"
		"<br><br>"
		"<input type=\"submit\" value=\"submit\">"
		"</form>"

		));
	ether.httpServerReply(bfill.position());
}


String Ethernet_enc28j60::receiveData(){

	String request;

	word size = ether.packetReceive();
	word pos = ether.packetLoop(size);
	int i;

	if (pos != 0)  // check if valid tcp data is received
	{
		i = 0;
		//copy http header line 1 to request
		while ((char)Ethernet::buffer[pos + 4 + i] != ' ' && i < 50) {
			request.concat((char)Ethernet::buffer[pos + 4 + i]);
			++i;
		}
		return request;
	}
	else {
		return "";
	}
}








/******************************* Debug function *******************************/



void Ethernet_enc28j60::TestEthernet() {
	ether.printIp("My IP: ", ether.myip);
	ether.printIp("Netmask: ", ether.netmask);
	ether.printIp("GW IP: ", ether.gwip);
	ether.printIp("DNS IP: ", ether.dnsip);
}

