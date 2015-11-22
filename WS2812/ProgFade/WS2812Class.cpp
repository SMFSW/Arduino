#include "WS2812Class.h"


void WS2812Strip::updateDimPix(uint16_t n) {
	if (DimPix)		{ free(DimPix); } // Free existing data (if any)

	// Allocate new data -- note: ALL DIM VALUES ARE CLEARED
	if ((DimPix = (uint8_t *) malloc(n)))	{ memset(DimPix, 0, n); }
}

inline void WS2812Strip::StripUpdate()
{
	switch (ModeDim)
	{
		case FADE_DIM_LINEAR:		{ ProgressiveFadeUpdate(); } break;
		case FADE_DIM_ALL:			{ FadeUpdate(); } break;
		default:					{} break;
	}

	switch (ModeCol)
	{
		//case :		{ (); } break;
		//case :			{ (); } break;
		default:					{} break;
	}
}

// Update the pattern
void WS2812Strip::Update()
{
	uint16_t acttime = millis();
	boolean update = false;
	
	if (Status != WAIT)
	{
		if (acttime - memotime > interval)
		{
			memotime = acttime;
			update = true;
		}
	}
	
	if (update)
	{
		StripUpdate();
		
		// Test si setDim ou set Color !!!!
		setDimColor();	
		
		show();
	}
}

// Increment the Index and reset at the end
void WS2812Strip::Increment()
{
	if (DirDim == FORWARD)
	{
		if (++idx >= steps)
		{
			//idx = 0;
			if (OnComplete != NULL)	{ OnComplete(); }
		}
	}
	else // DirDim == REVERSE
	{
		if (--idx <= 0)
		{
			//idx = steps-1;
			if (OnComplete != NULL)	{ OnComplete(); }
		}
	}
}

// Reverse pattern direction
void WS2812Strip::Reverse(boolean rst = false)
{
	if (!rst)
	{
		DirDim = (direction) (DirDim == FORWARD ? BACKWARD : FORWARD);
	}
	else
	{
		if (DirDim == FORWARD)
		{
			Forward();
			idx = steps-1;
		}
		else
		{
			Backward();
			idx = 0;
		}
	}
}


uint32_t WS2812Strip::DimColor(uint32_t col, uint8_t Dim)
{
	uint8_t red, green, blue;
	float DimVal = (float) Dim / 255.0f;
	
	red = (uint8_t) ((float) Red(col) * DimVal);
	green = (uint8_t) ((float) Green(col) * DimVal);
	blue = (uint8_t) ((float) Blue(col) * DimVal);
	
	uint32_t dimColor = Color(red, green, blue);
	return dimColor;
}

void WS2812Strip::setDimColor()
{
	for (int i = 0 ; i < WS2812Strip::numPixels() ; i++)
	{
		uint32_t output = DimColor(colorFront, DimPix[fromEnd ? WS2812Strip::numPixels() - 1 - i : i]);
		
		setPixelColor(i, output);
		
		/*rLED[i] = Red(output);
		gLED[i] = Green(output);
		bLED[i] = Blue(output);*/
	}
	show();
}

// Set all pixels to a color (synchronously)
void WS2812Strip::setColor(uint32_t color)
{
	for (int i = 0 ; i < WS2812Strip::numPixels() ; i++)
	{
		setPixelColor(i, color);
	}
	show();
}

// Returns the Red component of a 32-bit color
uint8_t WS2812Strip::Red(uint32_t color)
{
	return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t WS2812Strip::Green(uint32_t color)
{
	return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t WS2812Strip::Blue(uint32_t color)
{
	return color & 0xFF;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t WS2812Strip::Wheel(uint8_t WheelPos)
{
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85)
	{
		return Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	else if (WheelPos < 170)
	{
		WheelPos -= 85;
		return Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	else
	{
		WheelPos -= 170;
		return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	}
}

void WS2812Strip::FadeInit(uint8_t inc)
{
	ModeDim = FADE_DIM_ALL;
	valinc = inc;
	steps = 255 / valinc;
}

void WS2812Strip::FadeUpdate()
{
	boolean setWait = true;
	
	for (int i = 0 ; i < WS2812Strip::numPixels() ; i++)
	{
		uint8_t NumLED = DirDim ? WS2812Strip::numPixels() - 1 - i : i;
		int8_t Inc = DirDim ? -valinc : valinc;
		int16_t Val = max(0, min(255, DimPix[NumLED] + Inc));

		if (DirDim == FORWARD)
		{
			//idx++;
			if (Val <= 255)			{ DimPix[NumLED] = Val; }
			if (DimPix[i] != 255)	{ setWait = false; }
		}
		else
		{
			//idx--;
			if (Val >= 0)			{ DimPix[NumLED] = Val; }
			if (DimPix[i] != 0)		{ setWait = false; }
		}
	}
	
	Increment();
	if (setWait)
	{
		Hold();
		Reverse();
	}
}

void WS2812Strip::ProgressiveFadeInit(uint8_t inc, uint8_t thr, boolean startpos = false)
{
	ModeDim = FADE_DIM_LINEAR;
	fromEnd = startpos;
	valinc = inc;
	threshold = thr;
	steps = (255 + WS2812Strip::numPixels()) / valinc;
}

void WS2812Strip::ProgressiveFadeUpdate()
{
	for (int i = 0 ; i < WS2812Strip::numPixels() ; i++)
	{
		uint8_t NumLED = DirDim ? WS2812Strip::numPixels() - 1 - i : i;
		int8_t Inc = DirDim ? -valinc : valinc;
		int16_t Val = max(0, min(255, DimPix[NumLED] + Inc));
		
		if (DirDim == FORWARD)
		{
			//idx++;
			
			if (DimPix[numPixels()-1] == 255)
			{
				Hold();
				Reverse();
			}
			else if ( (NumLED == 0) || (DimPix[NumLED-1] > threshold) )
			{
				if (Val <= 255)	DimPix[NumLED] = Val;
			}
		}
		else
		{
			//idx--;
			
			if (DimPix[0] == 0)
			{
				Hold();
				Reverse();
			}
			else if ( (NumLED == numPixels()-1) || (DimPix[NumLED+1] < 255 - threshold) )
			{
				if (Val >= 0)	DimPix[NumLED] = Val;
			}
		}
	}
	Increment();
}
