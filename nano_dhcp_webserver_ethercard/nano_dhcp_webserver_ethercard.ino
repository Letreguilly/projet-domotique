#include <EtherCard.h>
#include <EEPROM.h>
#include <TrueRandom.h>




/********************************************** ethernet *******************************************/

//static byte mac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte mac[6];
static byte myIp[] = { 10,0,0,143 };
static byte gwIp[] = { 10,0,0,1 };
byte Ethernet::buffer[500];
BufferFiller bfill;
char macstr[18];


void setup () {
  
  
  
  
  
  Serial.begin(9600);
  if (EEPROM.read(1) == '#') {
    for (int i = 3; i < 6; i++) {
      mac[i] = EEPROM.read(i);
    }
  } else {
    for (int i = 3; i < 6; i++) {
      mac[i] = TrueRandom.randomByte();
      EEPROM.write(i, mac[i]);
    }
    EEPROM.write(1, '#');
  }
  //snprintf(macstr, 18, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);


  
  
  
  
  
  
   /**
  Ethernet
  **/
   Serial.print("MAC: ");
  for (byte i = 0; i < 6; ++i) {
    Serial.print(mac[i], HEX);
    if (i < 5)
      Serial.print(':');
  }
  Serial.println();
  
  if (ether.begin(sizeof Ethernet::buffer, mac, 10) == 0)  //10 spi slave pin
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


