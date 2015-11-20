#include <Adafruit_NeoPixel.h>

//#define DBG_SERIAL

#define	pinPB1	8
#define	pinPB2	9

#define	NbLEDsStrip1	9

#define DELTA_LED	30

enum	direction { FORWARD, BACKWARD };
enum	status { WAIT, RESET, RUN };

Adafruit_NeoPixel Strip = Adafruit_NeoPixel(NbLEDsStrip1, 6, NEO_GRB + NEO_KHZ800);


typedef struct StructStrip{
	uint32_t color;
	uint8_t DimLED[NbLEDsStrip1];
	uint16_t idx;
	uint16_t interval;
	uint16_t memotime;
	direction Dir;
	status Status;
};

static StructStrip StripState;


static boolean memoBP1 = false, memoBP2 = false;
static uint16_t TimeBP1, TimeBP2;

// Macros de comparaison de temps écoulé depuis la valeur de timer sauvegardée
#define TIME_COMP_SUP(v, t)		((uint16_t) ((uint16_t) millis() - (uint16_t) v) > (uint16_t) t)
//!< Tests if \b v (a Timer save variable) has reached time lapse stated in \b t (ms) 
#define TIME_COMP_INF(v, t)		((uint16_t) ((uint16_t) millis() - (uint16_t) v) < (uint16_t) t)
//!< Tests if \b v (a Timer save variable) has not reached time lapse stated in \b t (ms) 

// Initialize everything and prepare to start
void setup()
{
	
	pinMode(pinPB1, INPUT_PULLUP);
	pinMode(pinPB2, INPUT_PULLUP);
	
	#if DBG_SERIAL
	Serial.begin(9600);
	#endif
	
	// Initialize all the pixelStrips
	Strip.begin();

	for (int i = 0 ; i < NbLEDsStrip1 ; i++) { StripState.DimLED[i] = 0; }
	
	StripState.color = Strip.Color(255, 255, 255);
	StripState.interval = 10;
}

// Main loop
void loop()
{
	StripHandle();
	
	// Switch patterns on a button press:
	if (digitalRead(pinPB1) == LOW) // Button #1 pressed
	{
		if (	(memoBP1 == false)
			&&	(TIME_COMP_SUP(TimeBP1, 100)))
		{
			memoBP1 = true;

			StripState.Status = RUN;
		}
	}
	else
	{
		memoBP1 = false;
		TimeBP1 = millis();
	}

	if (digitalRead(pinPB2) == LOW) // Button #2 pressed
	{
		if (	(memoBP2 == false)
			&&	(TIME_COMP_SUP(TimeBP2, 100)))
		{
			memoBP2 = true;
		}
	}
	else
	{
		memoBP2 = false;
		TimeBP2 = millis();
	}
}

void StripHandle()
{
	uint16_t acttime = millis();
	boolean show = false;
	
	if (StripState.Status != WAIT)
	{
		if (acttime - StripState.memotime > StripState.interval)
		{
			StripState.memotime = acttime;
			show = true;
		}
	}
	
	if (show)
	{
		for (int i = 0 ; i < Strip.numPixels() ; i++)
		{
			Strip.setPixelColor(i, DimColor(StripState.color, StripState.DimLED[i]));
			
			#if DBG_SERIAL
			Serial.print(StripState.DimLED[i]);
			Serial.print("\t");
			#endif
		}
		#if DBG_SERIAL
		Serial.print("\n");
		#endif
		Strip.show();
		UpdateDimTab();
	}
}

void UpdateDimTab()
{
	if (StripState.Dir == FORWARD)
	{
		StripState.idx++;
		
		for (int i = 0 ; i < sizeof(StripState.DimLED) ; i++)
		{
			uint8_t NumLED = sizeof(StripState.DimLED) - 1 - i;
			
			if (StripState.DimLED[NbLEDsStrip1-1] == 255)
			{
				StripState.Status = WAIT;
				StripState.Dir = BACKWARD;
			}
			else if (NumLED == 0)
			{
				if (StripState.DimLED[NumLED] < 255)	StripState.DimLED[NumLED]++;
			}
			else if (StripState.DimLED[NumLED-1] > DELTA_LED)
			{
				if (StripState.DimLED[NumLED] < 255)	StripState.DimLED[NumLED]++;
			}
		}
	}
	else
	{
		StripState.idx--;
		
		for (int i = 0 ; i < sizeof(StripState.DimLED) ; i++)
		{
			if (StripState.DimLED[0] == 0)
			{
				StripState.Status = WAIT;
				StripState.Dir = FORWARD;
			}
			else if (i == sizeof(StripState.DimLED)-1)
			{
				if (StripState.DimLED[i] > 0)	StripState.DimLED[i]--;
			}
			else if (StripState.DimLED[i+1] < 255 - DELTA_LED)
			{
				if (StripState.DimLED[i] > 0)	StripState.DimLED[i]--;
			}
		}
	}
}

uint32_t DimColor(uint32_t color, uint8_t Dim)
{
	uint8_t red, green, blue;
	float DimVal = (float) Dim / 255.0f;
	
	red = (uint8_t) ((float) Red(color) * DimVal);
	green = (uint8_t) ((float) Green(color) * DimVal);
	blue = (uint8_t) ((float) Blue(color) * DimVal);
	
	uint32_t dimColor = Strip.Color(red, green, blue);
	return dimColor;
}

// Returns the Red component of a 32-bit color
uint8_t Red(uint32_t color)
{
	return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t Green(uint32_t color)
{
	return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t Blue(uint32_t color)
{
	return color & 0xFF;
}

