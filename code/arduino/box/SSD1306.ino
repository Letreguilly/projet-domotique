Screen::Screen(){
	u8g = U8GLIB_SSD1306_128X64(U8G_I2C_OPT_NONE);
	delay(100);
}

void Screen::InitScreen(){
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


