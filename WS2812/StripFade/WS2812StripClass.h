#include <Adafruit_NeoPixel.h>

// Pattern types supported:
enum  pattern { NONE, RAINBOW_CHASE, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE, PROGRESSIVE_FADE, WAVE };
// Patern directions supported:
enum  direction { FORWARD, BACKWARD };
enum  status { WAIT, RESET, RUN };

//#define DEBUG_WS2812

// NeoPattern Class - derived from the Adafruit_NeoPixel class
class WS2812Strip : public Adafruit_NeoPixel
{
	public:

	// Member Variables:  
	pattern  ActivePattern;  // which pattern is running
	direction Direction;     // direction to run the pattern
  status    Status;
  
	uint32_t Interval;   // milliseconds between updates
	uint32_t lastUpdate; // last update of position

	uint32_t Color1, Color2;  // What colors are in use
	uint16_t TotalSteps;  // total number of steps in the pattern
	uint16_t Index;  // current step within the pattern

	void (*OnComplete)();  // Callback on completion of pattern

	// Constructor - calls base-class constructor to initialize strip
	WS2812Strip(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
	:Adafruit_NeoPixel(pixels, pin, type)
	{
		OnComplete = callback;
    Status = RESET;
	}

	// Update the pattern
	void Update()
	{
    switch (Status)
    {
      case RESET:
      {
        lastUpdate = millis();
        Status = RUN;
      }
      break;
        
      case RUN:
      {
    		if ((millis() - lastUpdate) > Interval) // time to update
    		{
    			lastUpdate = millis();
    			switch(ActivePattern)
    			{
    				case RAINBOW_CHASE:		{ RainbowChaseUpdate(); } break;
    				case RAINBOW_CYCLE:		{ RainbowCycleUpdate(); } break;
    				case THEATER_CHASE:		{ TheaterChaseUpdate(); } break;
    				case COLOR_WIPE:		{ ColorWipeUpdate(); } break;
    				case SCANNER:			{ ScannerUpdate(); } break;
    				case FADE:				{ FadeUpdate(); } break;
    				case PROGRESSIVE_FADE:	{ ProgressiveFadeUpdate(); } break;
    				case WAVE:				{ WaveUpdate(); } break;
    				default:				{} break;
    			}
    		}
      }
      break;
  
      default:
      {} break;
    }
	}
  
  void Forward() { Direction = FORWARD; }
  void Backward() { Direction = BACKWARD; }
  void Hold() { Status = WAIT; }
  void Reset() { Status = RESET; }
  
  // Reverse pattern direction
  void Reverse(boolean startover = false)
  {
    if (Direction == FORWARD)
    {
      if (startover) { Index = TotalSteps; }
      Direction = BACKWARD;
      Status = RUN;
    }
    else
    {
      if (startover) { Index = 0; }
      Direction = FORWARD;
      Status = RUN;
    }
  }
 
  

	// Increment the Index and reset at the end
	void Increment()
	{
		if (Direction == FORWARD)
		{
		   if (Index++ >= TotalSteps)
			{
        Hold();
        Index = TotalSteps;
        if (OnComplete != NULL)	{ OnComplete(); } // call the comlpetion callback
			}
		}
		else if (Direction == BACKWARD)
		{
			if (Index-- <= 0)
			{
        Hold();
        Index = 0;
        if (OnComplete != NULL)	{ OnComplete(); } // call the comlpetion callback
			}
		}
    else // Hold
    {
    }
	}

	void RainbowChase(uint16_t interval, direction dir = FORWARD, status stat = RUN)
	{
		ActivePattern = RAINBOW_CHASE;
		Interval = interval;
		TotalSteps = 255;
		Index = 0;
		Direction = dir;
    Status = stat;
	}
	
	void RainbowChaseUpdate()
	{
		uint16_t i;
	
		for (i = 0 ; i < numPixels(); i++)
		{
			setPixelColor(i, Wheel((i+Index) & 255));
		}
		
		show();
		Increment();
	}
	
	// Initialize for a RainbowCycle
	void RainbowCycle(uint16_t interval, direction dir = FORWARD, status stat = RUN)
	{
		ActivePattern = RAINBOW_CYCLE;
		Interval = interval;
		TotalSteps = 255;
		Index = 0;
		Direction = dir;
    Status = stat;
	}

	// Update the Rainbow Cycle Pattern
	void RainbowCycleUpdate()
	{
		for (int i = 0 ; i < numPixels() ; i++)
		{
			setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
		}
		show();
		Increment();
	}

	// Initialize for a Theater Chase
	void TheaterChase(uint32_t color1, uint32_t color2, uint16_t interval, direction dir = FORWARD, status stat = RUN)
	{
		ActivePattern = THEATER_CHASE;
		Interval = interval;
		TotalSteps = numPixels();
		Color1 = color1;
		Color2 = color2;
		Index = 0;
		Direction = dir;
    Status = stat;
	}

	// Update the Theater Chase Pattern
	void TheaterChaseUpdate()
	{
		for(int i = 0 ; i < numPixels() ; i++)
		{
			if ((i + Index) % 3 == 0)	{ setPixelColor(i, Color1); }
			else						{ setPixelColor(i, Color2); }
		}
		show();
		Increment();
	}

	// Initialize for a ColorWipe
	void ColorWipe(uint32_t color, uint16_t interval, direction dir = FORWARD, status stat = RUN)
	{
		ActivePattern = COLOR_WIPE;
		Interval = interval;
		TotalSteps = numPixels();
		Color1 = color;
		Index = 0;
		Direction = dir;
    Status = stat;
	}

	// Update the Color Wipe Pattern
	void ColorWipeUpdate()
	{
		setPixelColor(Index, Color1);
		show();
		Increment();
	}

	// Initialize for a SCANNNER
	void Scanner(uint32_t color1, uint16_t interval, status stat = RUN)
	{
		ActivePattern = SCANNER;
		Interval = interval;
		TotalSteps = (numPixels() - 1) * 2;
		Color1 = color1;
		Index = 0;
    Status = stat;
	}

	// Update the Scanner Pattern
	void ScannerUpdate()
	{ 
		for (int i = 0; i < numPixels(); i++)
		{
			if (i == Index)						{ setPixelColor(i, Color1); }						// Scan Pixel to the right
			else if (i == TotalSteps - Index)	{ setPixelColor(i, Color1); }						// Scan Pixel to the left
			else								{ setPixelColor(i, DimColor(getPixelColor(i))); }	// Fading tail
		}
		show();
		Increment();
	}

	// Initialize for a Fade
	void Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint16_t interval, direction dir = FORWARD, status stat = RUN)
	{
		ActivePattern = FADE;
		Interval = interval;
		TotalSteps = steps;
		Color1 = color1;
		Color2 = color2;
		Index = 0;
		Direction = dir;
    Status = stat;
	}

	// Update the Fade Pattern
	void FadeUpdate()
	{
		// Calculate linear interpolation between Color1 and Color2
		// Optimise order of operations to minimize truncation error
		uint8_t red = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
		uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
		uint8_t blue = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;
		
		ColorSet(Color(red, green, blue));
		show();
		Increment();
	}
	
	void ProgressiveFade(uint32_t color1, uint32_t color2, uint16_t steps, uint16_t interval, direction dir = FORWARD, status stat = RUN)
	{
		ActivePattern = PROGRESSIVE_FADE;
		Interval = interval;
		TotalSteps = min(255, steps);
		Color1 = color1;
		Color2 = color2;
		Index = 0;
		Direction = dir;
    Status = stat;
	}

	void ProgressiveFadeUpdate()
	{
		float ratio;
		uint16_t r,g,b;
		
		// Calculate linear interpolation between Color1 and Color2
		// Optimise order of operations to minimize truncation error
		uint8_t red = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
		uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
		uint8_t blue = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;

		for(int i = 0 ; i < numPixels() ; i++)
		{
			ratio = ((float) (i+1) / (float) numPixels()) + ((float) Index / (float) TotalSteps);
			r = min(255, red * ratio);
			g = min(255, green * ratio);
			b = min(255, blue * ratio);
			setPixelColor(numPixels()-1-i, Color(r, g, b));

#ifdef DEBUG_WS2812
			Serial.print(ratio);
			Serial.print("\t");
			Serial.print(Color(r, g, b), HEX);
			Serial.print("\t");
#endif
}
#ifdef DEBUG_WS2812
		Serial.print("\n");
#endif
		show();
		Increment();
	}
	
	void Wave(uint32_t colBgd, uint32_t colWave, uint16_t steps, uint16_t interval, direction dir = FORWARD, status stat = RUN)
	{
		ActivePattern = WAVE;
		Interval = interval;
		TotalSteps = steps;
		Color1 = colBgd;
		Color2 = colWave;
		Index = 0;
		Direction = dir;
    Status = stat;
	}
	
	// Update the Wave Pattern
	void WaveUpdate()
	{
		uint32_t Val;
		
		for (int i = 0 ; i < numPixels() ; i++)
		{
			//Val = Wheel(((i * 32 / numPixels()) + Index) & 255);
			//Val = Wheel((Color1 + (Color2 * ((Index*85)/i)) & 255));	// Strobo
			Val = Wheel((Color1 + (Color2 * ((i*170)/Index)) & 255));
			setPixelColor(i, Val);
		}
		show();
		Increment();
	}
	
	// Calculate 50% dimmed version of a color (used by ScannerUpdate)
	uint32_t DimColor(uint32_t color)
	{
		// Shift R, G and B components one bit to the right
		uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
		return dimColor;
	}

	// Set all pixels to a color (synchronously)
	void ColorSet(uint32_t color)
	{
		for (int i = 0 ; i < numPixels() ; i++)
		{
			setPixelColor(i, color);
		}
		show();
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

	// Input a value 0 to 255 to get a color value.
	// The colours are a transition r - g - b - back to r.
	uint32_t Wheel(byte WheelPos)
	{
		WheelPos = 255 - WheelPos;
		if(WheelPos < 85)			{	return Color(255 - WheelPos * 3, 0, WheelPos * 3); }
		else if(WheelPos < 170)		{	WheelPos -= 85;
										return Color(0, WheelPos * 3, 255 - WheelPos * 3); }
		else						{	WheelPos -= 170;
										return Color(WheelPos * 3, 255 - WheelPos * 3, 0); }
	}
};
