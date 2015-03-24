#include <EtherCard.h>
#include <EEPROM.h>
#include <TrueRandom.h>
#include <U8glib.h>
#include <OneWire.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
String message;
String big;


OneWire  ds(4);  // on pin 10 (a 4.7K resistor is necessary)
byte addr[8];



const char serveur[] = "www.google.com";
byte mac[6] = { 8, 21, 177, 0, 0, 0 };
byte Ethernet::buffer[500];
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
  delay(5000);

  IPtoScreen(ether.myip, "  MyIP  ");
  Draw();
  delay(5000);

}


void loop() {	//loop forever

  word size = ether.packetReceive();
  word pos = ether.packetLoop(size);
  delay(200);

  if (pos != 0 )  // check if valid tcp data is received
  {
    for (int i = pos ; i < size ; i ++) {
      Serial.print((char)Ethernet::buffer[i]);
    }


    ether.httpServerReply(homePage()); // send web page data
    delay(200);
  }
  ReadtempAsync();
  Draw();
}
