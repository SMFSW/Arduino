#include <Adafruit_NeoPixel.h>
#include "WS2812Class.h"

String ProjectName = "Simple Progressive Fade demo - SMFSW";  //!< Project Name
String SWVersion = "v0.2";  //!< Version en cours du soft

#define NbLEDsStrip1	30//9
#define DELTA_LED		30

#define pinStrip1		6
#define pinStrip2		4
#define pinStrip3		2

#define	pinPB1			8
#define	pinPB2			9


WS2812Strip Strip(NbLEDsStrip1, pinStrip1, NEO_GRB + NEO_KHZ800, NULL);

static boolean memoBP1 = false, memoBP2 = false;
static uint16_t timeBP1, timeBP2;

// Macros de comparaison de temps écoulé depuis la valeur de timer sauvegardée
#define TIME_COMP_SUP(v, t)		((uint16_t) ((uint16_t) millis() - (uint16_t) v) > (uint16_t) t)	//!< Tests if \b v (a Timer save variable) has reached time lapse stated in \b t (ms)
#define TIME_COMP_INF(v, t)		((uint16_t) ((uint16_t) millis() - (uint16_t) v) < (uint16_t) t)	//!< Tests if \b v (a Timer save variable) has not reached time lapse stated in \b t (ms)


inline void actBP1()
{
	if (Strip.Status != RUN)		{ Strip.Status = RUN; }
	else
	{
		if (Strip.DirDim == FORWARD)	{ Strip.Backward(); }
		else							{ Strip.Forward(); }
	}
}

inline void actBP2()
{
	//FadeInit();
	Strip.colorLatest = Strip.colorFront;
	Strip.colorFront = Strip.Wheel(random(255));
}


// Initialize everything and prepare to start
void setup()
{
	// Strip LED parameters
	Strip.colorFront = Strip.Color(255, 255, 102);
	Strip.interval = 5;
	Strip.threshold = DELTA_LED;
	
	  // step inc by 3, start from bottom
   // threshold between pix of DELTA_LED
	Strip.ProgressiveFadeInit(3, DELTA_LED, false);

	// Initialize all the pixelStrips
	Strip.begin();
	
	// Initialize push buttons
	pinMode(pinPB1, INPUT_PULLUP);
	pinMode(pinPB2, INPUT_PULLUP);

	setupSerial();
}

// Main loop
void loop()
{
	Strip.Update();
	
	// Switch patterns on a button press:
	if (digitalRead(pinPB1) == LOW) // Button #1 pressed
	{
		if ( (memoBP1 == false) && (TIME_COMP_SUP(timeBP1, 100)) )
		{
			memoBP1 = true;
			actBP1();
		}
	}
	else
	{
		memoBP1 = false;
		timeBP1 = millis();
	}

	if (digitalRead(pinPB2) == LOW) // Button #2 pressed
	{
		if ( (memoBP2 == false) && (TIME_COMP_SUP(timeBP2, 100)) )
		{
			memoBP2 = true;
			actBP2();
		}
	}
	else
	{
		memoBP2 = false;
		timeBP2 = millis();
	}

	taskSerial();
}

