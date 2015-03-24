


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

void InitEthernet() {
  if (ether.begin(sizeof Ethernet::buffer, mac, 10) == 0) {
    message = "access eth";
    big = "failed";
    return;
  }
  DHCPsetup();
}

void DHCPsetup() {
  if (!ether.dhcpSetup()) {
    message = "dhcp fail";
    big = " static ";
    Staticsetup();
  }
}

void Staticsetup() {

  if (!ether.staticSetup(myDefaultIp)) {
    message = "  static eth";
    big = " failed";
  }
}

void IPtoScreen(uint8_t ip[4], String info) {
  String o1 = String(ip[0]);
  String o2 = String(ip[1]);
  String o3 = String(ip[2]);
  String o4 = String(ip[3]);
  String p = ".";
  message = "  " + o1 + p + o2 + p + o3 + p + o4;
  big = info;
}






  



/******************************* Debug function *******************************/



void TestEthernet() {
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

void Get() {
  if (!ether.dnsLookup(serveur)) {
    ether.printIp("SRV: ", ether.hisip);
    ether.browseUrl(PSTR("/foo/"), "bar", serveur, my_callback);
  }
}
