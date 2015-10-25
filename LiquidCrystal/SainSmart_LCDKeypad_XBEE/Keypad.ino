
//#define		V11		// Switch to V1.1 Thresholds

// define some values used by the panel and buttons
//static int lcd_key = 0;
static int adc_key_in = 0;
 
#define btnRIGHT	0
#define btnUP		1
#define btnDOWN		2
#define btnLEFT		3
#define btnSELECT	4
#define btnNONE		5


int read_LCD_buttons(){				// read the buttons
	adc_key_in = analogRead(0);		// read the value from the sensor 

	// my buttons when read are centered at these valies: 0, 144, 329, 504, 741
	// we add approx 50 to those values and check to see if we are close
	// We make this the 1st option for speed reasons since it will be the most likely result

	if (adc_key_in > 1000) return btnNONE; 

#ifdef V11
	// For V1.1 us this threshold
	if (adc_key_in < 50)   return btnRIGHT;  
	if (adc_key_in < 250)  return btnUP; 
	if (adc_key_in < 450)  return btnDOWN; 
	if (adc_key_in < 650)  return btnLEFT; 
	if (adc_key_in < 850)  return btnSELECT;
#else
	// For V1.0 comment the other threshold and use the one below:
	if (adc_key_in < 50)   return btnRIGHT;  
	if (adc_key_in < 195)  return btnUP; 
	if (adc_key_in < 380)  return btnDOWN; 
	if (adc_key_in < 555)  return btnLEFT; 
	if (adc_key_in < 790)  return btnSELECT;   
#endif

	return btnNONE;		// when all others fail, return this.
}
 

