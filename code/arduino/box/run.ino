

void run(){
	/****************************************************************************************************************
	Setup
	****************************************************************************************************************/

	/* Serial */
	Serial.begin(9600);
	/* Screen */
	screen = new Screen();
	//screen->big = "start";
	//screen->InitScreen();

	/* OneWire */
	OWD = new OWTemp();

	/* Mac */
	mac = new MacAddr();
	screen->Draw(mac->MactoString(), "  MyMac");
	delay(3000);

	/* Ethernet */
	//byte macad[6] = mac->value
	ethernet = new Ethernet_enc28j60();
	if (ethernet->InitEthernet(mac->value()) == false) {
		screen->Draw("access eth", "failed");
		delay(5000);
	}
	else if (ethernet->DHCPsetup() == false){
		screen->Draw("dhcp fail", " static");
		delay(5000);
		if (ethernet->Staticsetup() == false){
			screen->Draw("  static eth", " failed");
			delay(5000);
		}
	}
	screen->Draw(" " + ethernet->GetIp(), "  MyIp");
	delay(3000);

	/****************************************************************************************************************
	Loop
	****************************************************************************************************************/
	for (;;){

		String request = ethernet->receiveData();

		if (request.length() >= 1){
			pageChooser(request);
		}
		celsius= OWD->ReadtempAsync();
		screen->Draw("  temperature", " " + String(celsius) + "C");
	}
}