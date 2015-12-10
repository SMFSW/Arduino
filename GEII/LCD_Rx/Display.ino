
void displayLCD(String SCIIn){
	lcd.setCursor(0,0);			// set the LCD cursor   position 
	lcd.print(SCIIn.c_str());

	lcd.setCursor(9,1);			// move cursor to second line "1" and 9 spaces over

	lcd.setCursor(0,1);			// move to the begining of the second line
}


