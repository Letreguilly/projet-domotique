#include <EtherCard.h>
#include <EEPROM.h>
#include <TrueRandom.h>
#include <U8glib.h>
#include <OneWire.h>
#include <stdio.h>
#include <string.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
String message;
String big;


OneWire  ds(4);  // on pin 10 (a 4.7K resistor is necessary)
byte addr[8];



const char serveur[] = "www.google.com";
byte mac[6] = { 8, 21, 177, 0, 0, 0 };
byte Ethernet::buffer[750];
static byte myDefaultIp[] = { 10, 0, 0, 1 };


void setup() {
  Serial.begin(9600);
  big = "start";

  InitScreen();
  InitOWBus();
  InitMacAddress();
  InitEthernet();

  if (big != "start") {
    Draw();
    delay(5000);
  }

  MactoScreen(ether.mymac, "  MyMac ");
  Draw();
  delay(0);

  IPtoScreen(ether.myip, "  MyIP  ");
  Draw();
  delay(0);

}


void loop() {	//loop forever

  word size, pos;


  int i ;

  for (;;) {
    String request;
    
    size = ether.packetReceive();
    pos = ether.packetLoop(size);
    if (pos != 0 )  // check if valid tcp data is received
    {

      for (i = pos ; i < size + pos ; i ++) {
        Serial.print((char)Ethernet::buffer[i]);
      }

      i = 0;
      //copy http header line 1 to request
      while ((char)Ethernet::buffer[pos + 4 + i] != ' ' && i < 50) {
        request.concat((char)Ethernet::buffer[pos + 4 + i]);
        ++i;
      }

      //page chooser
      Serial.println(request);
      if (request.startsWith("/temperature.html")) {
        ether.httpServerReply(temperature());
      } else if (request.startsWith("/settings.html")) {
        ether.httpServerReply(settings());
        
      } else {
        ether.httpServerReply(notfound());
      }
      
    }
    ReadtempAsync();
    Draw();
  }
}
