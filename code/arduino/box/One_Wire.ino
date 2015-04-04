

OWTemp::OWTemp() {
	
	ds.search(addr);
	ds.reset_search();
	ds.reset();
	ds.select(addr);

}

//you can only wall this function one time per second, even temperature will fail
float OWTemp::ReadtempAsync() {
	byte i;
	int16_t raw;
	byte data[12];

	ds.reset();
	ds.select(addr);
	ds.write(0xBE);		//result

	//read
	for (i = 0; i < 9; i++) {
		data[i] = ds.read();
		raw = (data[1] << 8) | data[0]; 	byte cfg = (data[4] & 0x60);
	}


	ds.reset();
	ds.select(addr);
	ds.write(0x44, 1);	// start conversion, with parasite power on at the end

	return (float)raw / 16.0;
}

/******************************* Debug function ******************************


byte CheckProbeType() {

	if (addr[0] != 0x28) {

		Serial.println("  Chip is not DS18B20");
		return 1;
	}	return 0;

}


void PrintOWAddress() {

	byte i;
	Serial.print("OW Address =");
	for (i = 0; i < 8; i++) {

		Serial.write(' ');
		Serial.print(addr[i], HEX);
	}
}

void OWdataPrinter(byte data[12]) {
	byte i;
	for (i = 0; i < 9; i++) {
		// we need 9 bytes
		Serial.print(data[i], HEX);
		Serial.print(" ");
	}
}

void ReadtempSync() {
	byte present = 0;
	byte i;
	int16_t raw;
	byte data[12];

	ds.reset();
	ds.select(addr);
	ds.write(0x44, 1);	// start conversion, with parasite power on at the end

	delay(1000);		//wait for conversion
	present = ds.reset();
	ds.select(addr);
	ds.write(0xBE);		//result

	//read
	for (i = 0; i < 9; i++) {
		data[i] = ds.read();
		raw = (data[1] << 8) | data[0]; 	byte cfg = (data[4] & 0x60);
	}


	celsius = (float)raw / 16.0;
	screen->message = "  temperature";
	screen->big = " " + String(celsius) + "C";
}
*/