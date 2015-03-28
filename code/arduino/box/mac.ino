
byte * MacAddr::value(){
	return mac;
}

MacAddr::MacAddr() {
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

String  MacAddr::MactoString() {
	String m1 = String(mac[0], HEX);
	String m2 = String(mac[1], HEX);
	String m3 = String(mac[2], HEX);
	String m4 = String(mac[3], HEX);
	String m5 = String(mac[4], HEX);
	String m6 = String(mac[5], HEX);
	String m = String(":");

	return m1 + m + m2 + m + m3 + m + m4 + m + m5 + m + m6;
}



