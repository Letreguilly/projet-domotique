U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

void InitScreen(){
	u8g.setFont(u8g_font_unifontr);
	delay(600);
}

void Draw(String small, String big) {

	// picture loop
	u8g.firstPage();
	do {
		u8g.setPrintPos(0, 14);
		u8g.print(small);
		u8g.drawHLine(0, 16, 128);
		u8g.setScale2x2();
		u8g.setPrintPos(0, 24);
		u8g.print(big);
		u8g.undoScale();
	} while (u8g.nextPage());
	delay(50);
}
