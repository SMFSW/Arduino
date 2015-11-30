
void displayLCD(String SCIIn){
	lcd.setCursor(0,0);			// set the LCD cursor   position 
	lcd.print(SCIIn);	// print a simple message on the LCD

	lcd.setCursor(9,1);			// move cursor to second line "1" and 9 spaces over

	lcd.setCursor(0,1);			// move to the begining of the second line
}


