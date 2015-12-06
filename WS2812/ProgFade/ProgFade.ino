#include "WS2812Class.h"

String ProjectName = "WS2812 library test bench - SMFSW";  //!< Project Name
String SWVersion = "v0.3";  //!< Version en cours du soft

#define NbLEDsStrip1	58
#define NbLEDsStrip2    12
#define NbLEDsStrip3    12

#define DELTA_LED		30//250

#define pinStrip1		6
#define pinStrip2		4
#define pinStrip3		2

#define	pinPB1			8
#define	pinPB2			9

void doneStrip();
void doneStrip2();
void doneStrip3();

WS2812Strip Strip(NbLEDsStrip1, pinStrip1, NEO_GRB + NEO_KHZ800, doneStrip);
WS2812Strip Strip2(NbLEDsStrip2, pinStrip2, NEO_GRB + NEO_KHZ800, doneStrip);
WS2812Strip Strip3(NbLEDsStrip3, pinStrip3, NEO_GRB + NEO_KHZ800, doneStrip);

Key CustomScenar[4] = {
                        { Strip.Color(0,200,0), 100 },
                        { Strip.Color(0,255,255), 100 },
                        { Strip.Color(0,0,200), 100 },
                        { Strip.Color(150,150,150), 100 },
};
uint8_t memoStep = 0;


static boolean memoBP1 = false, memoBP2 = false;
static uint16_t timeBP1, timeBP2;

// Macros de comparaison de temps écoulé depuis la valeur de timer sauvegardée
#define TIME_COMP_SUP(v, t)		((uint16_t) ((uint16_t) millis() - (uint16_t) v) > (uint16_t) t)	//!< Tests if \b v (a Timer save variable) has reached time lapse stated in \b t (ms)
#define TIME_COMP_INF(v, t)		((uint16_t) ((uint16_t) millis() - (uint16_t) v) < (uint16_t) t)	//!< Tests if \b v (a Timer save variable) has not reached time lapse stated in \b t (ms)

inline void Gere_Custom_Scenar()
{
  Strip.FadeInit(3, 200);
  Strip.RainbowCycleInit(200, 80);
  //Strip.ColorWipeInit(CustomScenar[memoStep].color, CustomScenar[memoStep].interval);
  //if (++memoStep >= sizeof(CustomScenar)/sizeof(Key)) { memoStep = 0; }
}

inline void actBP1()
{
    Strip.RunColor();
    
	if (Strip.ControlDim.Status != RUN)			{ Strip.RunDim(); }
	else
	{
		if (Strip.ControlDim.Dir == FORWARD)	{ Strip.ControlDim.Dir = BACKWARD; }
		else									{ Strip.ControlDim.Dir = FORWARD; }
	}
}

inline void actBP2()
{
	//Strip.FadeColorInit(Strip.colorFront, Strip.Wheel(random(255)), 255, 5);
	Strip.colorFront = Strip.Wheel(random(255));
}

void doneStrip()
{
  //if (Strip.ControlCol.end == true) { Gere_Custom_Scenar(); }
}

void doneStrip2()
{
  
}

void doneStrip3()
{
  
}

// Initialize everything and prepare to start
void setup()
{
	/** Initialize 1st strip **/
	Strip.begin();
	Strip.setColor(0, false);

	// Strip LED parameters
	Strip.colorFront = Strip.Color(255, 255, 102);

	// step inc by 3 every 5ms, start from bottom
	// threshold between pix of DELTA_LED
	Strip.ProgressiveFadeInit(3, DELTA_LED, 5);
  Gere_Custom_Scenar();

  /** Initialize 2nd strip **/
  Strip2.begin();
  Strip2.setColor(0, false);

  // Strip LED parameters
  // step inc every 5ms
  Strip2.setDim(255);
  //Strip2.FadeInit(3, 5);
  //Strip2.RainbowChaseInit(5);
  //Strip2.RunColor();
  
  /** Initialize 3rd strip **/
  Strip3.begin();
  Strip3.setColor(0, false);

  // Strip LED parameters
  // step inc every 5ms
  Strip3.setDim(255);
  Strip3.ScannerInit(Strip3.Color(0, random(255), random(255)), 500);
  Strip3.RunColor();
  
	// Initialize push buttons
	pinMode(pinPB1, INPUT_PULLUP);
	pinMode(pinPB2, INPUT_PULLUP);

	setupSerial();
}

// Main loop
void loop()
{
	Strip.Update();
    Strip2.Update();
    Strip3.Update();
	
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

