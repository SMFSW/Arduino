/*!
**	\file Interrupts.ino
**	\brief code de fonctions gérées en interruptions
**/

#define DEF_LED1_BLINK_PERIOD	2000U		//!< Période de changement d'état de LED1 (2s)


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
	static boolean TestLED_State = HIGH;	//!< Variable statique de mémorisation d'état de TestLED (HIGH ou LOW)
	static boolean LED1_State = HIGH;		//!< Variable statique de mémorisation d'état de LED1 (HIGH ou LOW)
	static unsigned int LED1_Blink = 0;		//!< Variable statique de compteur pour mise à jour de LED1

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


