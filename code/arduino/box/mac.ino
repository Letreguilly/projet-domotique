

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
    }
    EEPROM.write(1, '#');
  }
}

void MactoScreen(uint8_t macaddr[6], String info) {
  String m1 = String(macaddr[0], HEX);
  String m2 = String(macaddr[1], HEX);
  String m3 = String(macaddr[2], HEX);
  String m4 = String(macaddr[3], HEX);
  String m5 = String(macaddr[4], HEX);
  String m6 = String(macaddr[5], HEX);
  String m = String(":");

  message = m1 + m + m2 + m + m3 + m + m4 + m + m5 + m + m6;
  big = info;  
}

void MactoSerial() {
  Serial.print("MAC: ");
  for (byte i = 0; i < 6; ++i) {

    Serial.print(mac[i], HEX);
    if (i < 5) {

      Serial.print(':');
    }
  }
}
