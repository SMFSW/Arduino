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
		/*case RAINBOW_CHASE:		{ RainbowChaseUpdate(); } break;
		case RAINBOW_CYCLE:		{ RainbowCycleUpdate(); } break;
		case THEATER_CHASE:		{ TheaterChaseUpdate(); } break;
		case COLOR_WIPE:		{ ColorWipeUpdate(); } break;
		case SCANNER:			{ ScannerUpdate(); } break;
		case FADE:				{ FadeUpdate(); } break;
		case WAVE:				{ WaveUpdate(); } break;*/
		default:				{} break;
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
		
		setColor(colorFront, ModeDim == NONE ? false : true);
		
		show();
	}
}

void WS2812Strip::Increment(Index * id, boolean rst = false)
{
	if (id->Dir == FORWARD)
	{
		if (++id->idx >= id->steps)
		{
			if (rst)  { id->idx = 0; }
			if (OnComplete != NULL) { OnComplete(); } // call the comlpetion callback
		}
	}
	else // DirCol == REVERSE
	{
		if (--id->idx <= 0)
		{
			if (rst)  { id->idx = id->steps-1; }
			if (OnComplete != NULL) { OnComplete(); } // call the comlpetion callback
		}
	}
}

void WS2812Strip::Reverse(Index * id, boolean rst = false)
{
	if (!rst)
	{
		id->Dir = (direction) (id->Dir == FORWARD ? BACKWARD : FORWARD);
	}
	else
	{
		if (id->Dir == FORWARD)
		{
			Backward(id);
			if (rst)  { id->idx = id->steps-1; }
		}
		else
		{
			Forward(id);
			if (rst)  { id->idx = 0; }
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

void WS2812Strip::setColor(uint32_t col, boolean useDim)
{
	for (int i = 0 ; i < WS2812Strip::numPixels() ; i++)
	{
		uint32_t output;
		
		if (useDim)	{	output = DimColor(col, DimPix[fromEnd ? WS2812Strip::numPixels() - 1 - i : i]); }
		else		{	output = col; }
		
		setPixelColor(i, output);

		/*rLED[i] = Red(output);
		gLED[i] = Green(output);
		bLED[i] = Blue(output);*/
	}
	show();
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
	IndexDim.steps = 255 / valinc;
}

void WS2812Strip::FadeUpdate()
{
	boolean setWait = true;
	
	for (int i = 0 ; i < WS2812Strip::numPixels() ; i++)
	{
		uint8_t NumLED = IndexDim.Dir ? WS2812Strip::numPixels() - 1 - i : i;
		int8_t Inc = IndexDim.Dir ? -valinc : valinc;
		int16_t Val = max(0, min(255, DimPix[NumLED] + Inc));

		if (IndexDim.Dir == FORWARD)
		{
			if (Val <= 255)			{ DimPix[NumLED] = Val; }
			if (DimPix[i] != 255)	{ setWait = false; }
		}
		else
		{
			if (Val >= 0)			{ DimPix[NumLED] = Val; }
			if (DimPix[i] != 0)		{ setWait = false; }
		}
	}
	
	Increment(&IndexDim, false);
 
	if (setWait)
	{
		Hold();
		Reverse(&IndexDim, true);
	}
}

void WS2812Strip::ProgressiveFadeInit(uint8_t inc, uint8_t thr, boolean startpos = false)
{
	ModeDim = FADE_DIM_LINEAR;
	fromEnd = startpos;
	valinc = inc;
	threshold = thr;
	IndexDim.steps = (255 + WS2812Strip::numPixels()) / valinc;
}

void WS2812Strip::ProgressiveFadeUpdate()
{
	for (int i = 0 ; i < WS2812Strip::numPixels() ; i++)
	{
		uint8_t NumLED = IndexDim.Dir ? WS2812Strip::numPixels() - 1 - i : i;
		int8_t Inc = IndexDim.Dir ? -valinc : valinc;
		int16_t Val = max(0, min(255, DimPix[NumLED] + Inc));
		
		if (IndexDim.Dir == FORWARD)
		{
			if (DimPix[numPixels()-1] == 255)
			{
				Hold();
				Reverse(&IndexDim, true);
			}
			else if ( (NumLED == 0) || (DimPix[NumLED-1] > threshold) )
			{
				if (Val <= 255)	DimPix[NumLED] = Val;
			}
		}
		else
		{
			if (DimPix[0] == 0)
			{
				Hold();
				Reverse(&IndexDim, true);
			}
			else if ( (NumLED == numPixels()-1) || (DimPix[NumLED+1] < 255 - threshold) )
			{
				if (Val >= 0)	DimPix[NumLED] = Val;
			}
		}
	}
	Increment(&IndexDim, false);
}
