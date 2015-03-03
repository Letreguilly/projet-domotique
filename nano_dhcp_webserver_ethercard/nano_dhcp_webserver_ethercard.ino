#include <EtherCard.h>




/********************************************** ethernet *******************************************/

static byte myMac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte myIp[] = { 10,0,0,143 };
static byte gwIp[] = { 10,0,0,1 };
byte Ethernet::buffer[500];
BufferFiller bfill;


void setup () {
  Serial.begin(9600);
  
   /**
  Ethernet
  **/
   Serial.print("MAC: ");
  for (byte i = 0; i < 6; ++i) {
    Serial.print(myMac[i], HEX);
    if (i < 5)
      Serial.print(':');
  }
  Serial.println();
  
  if (ether.begin(sizeof Ethernet::buffer, myMac, 10) == 0)  //10 spi slave pin
    Serial.println( "Failed to access Ethernet controller");
    
    
  Serial.println(F("Setting up DHCP"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));
  
  ether.printIp("My IP: ", ether.myip);
  ether.printIp("Netmask: ", ether.netmask);
  ether.printIp("GW IP: ", ether.gwip);
  ether.printIp("DNS IP: ", ether.dnsip);

}
void loop () {

  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);

  if (pos)  // check if valid tcp data is received
  {
    ether.httpServerReply(homePage()); // send web page data
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


