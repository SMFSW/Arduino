/*!
**	\file Interrupts.ino
**	\brief code de fonctions gérées en interruptions
**/

#define DEF_LED1_BLINK_PERIOD	2000U		//!< 2s


/*!
**	\brief Interruption générée par MsTimer2
**	\isr Interrupt Service Routine
**
**	\details Actions:
**				- Increments TIME_COUNT
**				- Update LED Test pin
**				- Time check for SCI communication uses TIME_COUNT
**/
void isrLedflash()
{
	static boolean TestLED_State = HIGH, LED1_State = HIGH;  // Static variable of the function
	static int LED1_Blink = 0;
	
	TIME_COUNT++;	// inc LED count
	
	if (++LED1_Blink > (DEF_LED1_BLINK_PERIOD / DEF_MSTIMER2_PERIOD))
	{
		LED1_Blink = 0;
		digitalWrite(LED_Pins[0], LED1_State);
		LED1_State = !LED1_State;
	}
	
	digitalWrite(13, TestLED_State);
	TestLED_State = !TestLED_State; // Invert LED state
}

