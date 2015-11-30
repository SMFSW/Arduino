/*
This example is for Series 2 XBee
Receives a ZB RX packet and sets a PWM value based on packet data.
Error led is flashed if an unexpected packet is received
*/

#include "SeqTimer.h"
#include <LiquidCrystal.h>
 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel
SeqTimer timerLCD;

#define DEF_TAILLE_SCI_IN    64      //!< Taille réservée pour le buffer d'entrée SCI
static int        SCIInNbChar = 0;    //!< Inutilisé, sert juste à compter le nombre de char recus
static char        breakoutChar = 13;   //!< Declare ASCII value of CR
static String      SCIIn = "";       //!< Declare a new string 
static boolean      SCIbreakout;      //!< Defines if an acquired SCI string breakout char is reached

int statusLed = 13;
int errorLed = 13;
int dataLed = 13;

void flashLed(int pin, int times, int wait) {
	for (int i = 0; i < times; i++)
	{
		digitalWrite(pin, HIGH);
		delay(wait);
		digitalWrite(pin, LOW);
		
		if (i + 1 < times) { delay(wait); }
	}
}



void setup(){
	lcd.begin(16, 2);		// start the library
   lcd.setCursor(0,0);
   lcd.print("Starting");

	pinMode(statusLed, OUTPUT);
	pinMode(errorLed, OUTPUT);
	pinMode(dataLed,  OUTPUT);

	timerLCD.init(250);

	// start serial
	Serial.begin(9600);

	flashLed(statusLed, 3, 50);
}


void loop()
{
	//if (timerLCD.getTimer())	{ displayLCD(); }
	taskSerial();
}

void taskSerial()
{
  if (SCIbreakout == true)
  {
    Serial.print(SCIIn);
    int rem = SCIIn.length();
    
    (void) displayLCD(SCIIn);
    
    // Clean treated part of serial internal buffer
    SCIIn.remove(0, rem);
    SCIbreakout = false;
  }
}

/*!
**  \brief Called whenever a new data has been received on RX
**  \remark This routine is run between each time loop() runs, no delay here set in here
**  \note Multiple bytes of data may be available
**/
void serialEvent(void)
{
  if (Serial.available())
  {
    char inChar = (char) Serial.read();

    SCIIn += inChar;

    if ( (++SCIInNbChar > DEF_TAILLE_SCI_IN-1) || (inChar == breakoutChar) )
    {
      SCIbreakout = true;
    }
  }
}

