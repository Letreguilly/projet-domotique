
void pageChooser(String request){
	//page chooser
	if (request.indexOf("temperature") == 1) {
		ethernet->pageTemperature();
	}
	else if (request.indexOf("settings") == 1) {
		ethernet->pageSettings();
	}
	else {
		ethernet->pageNotfound();
	}
}

String getUrlParameter(String url, String parameter){

	int  pos = url.indexOf(parameter) + parameter.length() + 1;
	Serial.println(pos);
	String value;
	unsigned int i = 0;

	while (url.charAt(pos) != '?' && url.charAt(pos) != '&' && i < 10){
		value.concat(url.charAt(pos));
		pos++;
		i++;
	}


	return value;
}

int freeRam() {
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}