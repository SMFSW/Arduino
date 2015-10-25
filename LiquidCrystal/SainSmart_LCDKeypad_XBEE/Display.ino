enum EnumKey {
	RIGHT = 0,
	UP,
	DOWN,
	LEFT,
	SELECT,
	NONE
} Key;


void displayLCD(){
	lcd.setCursor(0,0);			// set the LCD cursor   position 
	lcd.print("Feed me l1");	// print a simple message on the LCD

	lcd.setCursor(9,1);			// move cursor to second line "1" and 9 spaces over
	lcd.print(millis()/1000);	// display seconds elapsed since power-up

	lcd.setCursor(0,1);			// move to the begining of the second line

	displayButton();
}

void displayButton(){
	switch (read_LCD_buttons()){		// depending on which button was pushed, we perform an action

		case RIGHT:{					//  push button "RIGHT" and show the word on the screen
			lcd.print("RIGHT ");
			break;
		}
		case LEFT:{
			 lcd.print("LEFT   ");		//  push button "LEFT" and show the word on the screen
			 break;
		}
		case UP:{
			 lcd.print("UP    ");		//  push button "UP" and show the word on the screen
			 break;
		}
		case DOWN:{
			 lcd.print("DOWN  ");		//  push button "DOWN" and show the word on the screen
			 break;
		}
		case SELECT:{
			 lcd.print("SELECT");		//  push button "SELECT" and show the word on the screen
			 break;
		}
		case NONE:{
			 lcd.print("NONE  ");		//  No action  will show "None" on the screen
			 break;
		}
	}
}

