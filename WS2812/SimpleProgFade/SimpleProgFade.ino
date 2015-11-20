#include <Adafruit_NeoPixel.h>

//#define DBG_SERIAL

String ProjectName = "Simple Progressive Fade demo - SMFSW";  //!< Project Name
String SWVersion = "v0.2";  //!< Version en cours du soft

#define	pinPB1	8
#define	pinPB2	9

#define	NbLEDsStrip1	30//9

#define DELTA_LED	30

enum	direction { FORWARD = 0, BACKWARD };
enum	status { WAIT = 0, RESET, RUN };
enum	mode { NONE = 0, FADE_DIM_LINEAR, FADE_DIM_ALL };

Adafruit_NeoPixel Strip = Adafruit_NeoPixel(NbLEDsStrip1, 6, NEO_GRB + NEO_KHZ800);


typedef struct StructStrip{
	uint32_t	color;
	uint8_t		DimLED[NbLEDsStrip1];
	uint8_t		valinc;
	uint16_t	idx;
	uint16_t	interval;
	uint16_t	memotime;
	boolean		fromtop;
	direction	Dir;
	status		Status;
	mode		Mode;
};

static StructStrip StripState;

static boolean memoBP1 = false, memoBP2 = false;
static uint16_t timeBP1, timeBP2;

// Macros de comparaison de temps écoulé depuis la valeur de timer sauvegardée
#define TIME_COMP_SUP(v, t)		((uint16_t) ((uint16_t) millis() - (uint16_t) v) > (uint16_t) t)	//!< Tests if \b v (a Timer save variable) has reached time lapse stated in \b t (ms)
#define TIME_COMP_INF(v, t)		((uint16_t) ((uint16_t) millis() - (uint16_t) v) < (uint16_t) t)	//!< Tests if \b v (a Timer save variable) has not reached time lapse stated in \b t (ms)


inline void actBP1()
{
	if (StripState.Status != RUN)		{ StripState.Status = RUN; }
	else
	{
		if (StripState.Dir == FORWARD)	{ StripState.Dir = BACKWARD; }
		else							{ StripState.Dir = FORWARD; }
	}
}

inline void actBP2()
{
	StripState.color = Wheel(random(255));
}


// Initialize everything and prepare to start
void setup()
{
	// Strip LED parameters
	StripState.color = color(255, 255, 75);
	StripState.interval = 5;
	StripState.fromtop = false;
	StripState.valinc = 3;

	StripState.Mode = FADE_DIM_LINEAR;
	for (int i = 0 ; i < sizeof(StripState.DimLED) ; i++) { StripState.DimLED[i] = 0; }

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
	StripHandle();
	
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

boolean StripHandle()
{
	uint16_t acttime = millis();
	boolean show = false;
	
	if (StripState.Status != WAIT)
	{
		//if TIME_COMP_SUP(StripState.memotime, StripState.interval)
		if (acttime - StripState.memotime > StripState.interval)
		{
			StripState.memotime = acttime;
			show = true;
		}
	}
	
	if (show)
	{
		UpdateDimTab();
		for (int i = 0 ; i < Strip.numPixels() ; i++)
		{
			Strip.setPixelColor(i, DimColor(StripState.color, StripState.DimLED[StripState.fromtop ? sizeof(StripState.DimLED) - 1 - i : i]));
		}
		Strip.show();
	}
	
	return show;
}

void UpdateDimTab()
{
	switch (StripState.Mode)
	{
		case FADE_DIM_LINEAR:
		{
			for (int i = 0 ; i < sizeof(StripState.DimLED) ; i++)
			{
				uint8_t NumLED = StripState.Dir ? sizeof(StripState.DimLED) - 1 - i : i;
				int8_t Inc = StripState.Dir ? -StripState.valinc : StripState.valinc;
				int16_t Val = max(0, min(255, StripState.DimLED[NumLED] + Inc));
				
				if (StripState.Dir == FORWARD)
				{
					StripState.idx++;
					
					if (StripState.DimLED[NbLEDsStrip1-1] == 255)
					{
						StripState.Status = WAIT;
						StripState.Dir = BACKWARD;
					}
					else if ( (NumLED == 0) || (StripState.DimLED[NumLED-1] > DELTA_LED) )
					{
						if (Val <= 255)	StripState.DimLED[NumLED] = Val;
					}
				}
				else
				{
					StripState.idx--;
					
					if (StripState.DimLED[0] == 0)
					{
						StripState.Status = WAIT;
						StripState.Dir = FORWARD;
					}
					else if ( (NumLED == sizeof(StripState.DimLED)-1) || (StripState.DimLED[NumLED+1] < 255 - DELTA_LED) )
					{
						if (Val >= 0)	StripState.DimLED[NumLED] = Val;
					}
				}
			}
		}
		break;

		case FADE_DIM_ALL:
		{
			boolean setWait = true;
			
			for (int i = 0 ; i < sizeof(StripState.DimLED) ; i++)
			{
				uint8_t NumLED = StripState.Dir ? sizeof(StripState.DimLED) - 1 - i : i;
				int8_t Inc = StripState.Dir ? -StripState.valinc : StripState.valinc;
				int16_t Val = max(0, min(255, StripState.DimLED[NumLED] + Inc));

				if (StripState.Dir == FORWARD)
				{
					StripState.idx++;
					if (Val <= 255)						{ StripState.DimLED[NumLED] = Val; }
					if (StripState.DimLED[i] != 255)	{ setWait = false; }
				}
				else
				{
					StripState.idx--;
					if (Val >= 0)					{ StripState.DimLED[NumLED] = Val; }
					if (StripState.DimLED[i] != 0)	{ setWait = false; }
				}
			}

			if (setWait)
			{
				StripState.Status = WAIT;
				StripState.Dir = BACKWARD;
			}
		}
		break;

		default: {} break;
	}
}

uint32_t DimColor(uint32_t col, uint8_t Dim)
{
	uint8_t red, green, blue;
	float DimVal = (float) Dim / 255.0f;
	
	red = (uint8_t) ((float) Red(col) * DimVal);
	green = (uint8_t) ((float) Green(col) * DimVal);
	blue = (uint8_t) ((float) Blue(col) * DimVal);
	
	uint32_t dimColor = color(red, green, blue);
	return dimColor;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(uint8_t WheelPos)
{
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85)
	{
		return color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	else if (WheelPos < 170)
	{
		WheelPos -= 85;
		return color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	else
	{
		WheelPos -= 170;
		return color(WheelPos * 3, 255 - WheelPos * 3, 0);
	}
}

inline uint32_t color(uint8_t r, uint8_t g, uint8_t b)
{
	return ((((uint32_t) r & 255) << 16) | (((uint32_t) g & 255) << 8) | ((uint32_t) b & 255));
}

// Returns the Red component of a 32-bit color
inline uint8_t Red(uint32_t color)
{
	return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
inline uint8_t Green(uint32_t color)
{
	return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
inline uint8_t Blue(uint32_t color)
{
	return color & 0xFF;
}
