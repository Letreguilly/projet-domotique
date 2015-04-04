Screen::Screen(){
	u8g = U8GLIB_SSD1306_128X64(U8G_I2C_OPT_NONE);
	u8g.setFont(u8g_font_unifontr);
	delay(100);
}

void Screen::Draw(){
	// picture loop
	u8g.firstPage();
	do {
		u8g.setPrintPos(0, 14);
		u8g.print(message);
		u8g.drawHLine(0, 16, 128);
		u8g.setScale2x2();
		u8g.setPrintPos(0, 24);
		u8g.print(big);
		u8g.undoScale();
	} while (u8g.nextPage());
	delay(10);
}

void Screen::Draw(String Newessage, String Newvalue){
	byte state = 0;
	if (Newessage != message){ 
		message = Newessage;
		state = 1;
	}
	if (Newvalue != big){ 
		big = Newvalue; 
		state = 1;
	}

	// picture loop
	if (state == 1){
		u8g.firstPage();
		do {
			u8g.setPrintPos(0, 14);
			u8g.print(message);
			u8g.drawHLine(0, 16, 128);
			u8g.setScale2x2();
			u8g.setPrintPos(0, 24);
			u8g.print(big);
			u8g.undoScale();
		} while (u8g.nextPage());
		delay(10);
	}
}


