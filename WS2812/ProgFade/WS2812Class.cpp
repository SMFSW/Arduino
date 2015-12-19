#include "WS2812Class.h"


void WS2812Strip::updateDimPix(uint16_t n) {
	if (DimPix)		{ free(DimPix); } // Free existing data (if any)
	
	// Allocate new data -- note: ALL DIM VALUES ARE CLEARED
	if ((DimPix = (uint8_t *) malloc(n)))	{ memset(DimPix, 0, n); }
}


inline void WS2812Strip::StripUpdateCol()
{
	if (ControlCol.Status != WAIT)
	{
		switch (ControlCol.Mode)
		{
			case RAINBOW_CYCLE:		{ RainbowCycleUpdate(); } break;
			case THEATER_CHASE:		{ TheaterChaseUpdate(); } break;
			case COLOR_WIPE:		{ ColorWipeUpdate(); } break;
			case SCANNER:			{ ScannerUpdate(); } break;
			case FADE:				{ FadeColorUpdate(); } break;
			case WAVE:				{ WaveUpdate(); } break;
			default:				{ PlainColorUpdate(); } break;
		}
	}
	else { PlainColorUpdate(); }	// To feed with Front color any time when not running
}

inline void WS2812Strip::StripUpdateDim()
{
	if (ControlDim.Status != WAIT)
	{
		switch (ControlDim.Mode)
		{
			case FADE_DIM_LINEAR:		{ ProgressiveFadeUpdate(); } break;
			case FADE_DIM_ALL:			{ FadeUpdate(); } break;
			default:					{} break;
		}
	}
}

// Update the pattern
void WS2812Strip::Update()
{
	uint16_t acttime = millis();
	boolean upd = false;
	
	if (acttime - ControlCol.memotime > ControlCol.interval)
	{
		ControlCol.memotime = acttime;
		upd = true;
		StripUpdateCol();
	}
	
	if (acttime - ControlDim.memotime > ControlDim.interval)
	{
		ControlDim.memotime = acttime;
		upd = true;
		StripUpdateDim();
	}
	
	if (upd)
	{
		setColor(colorFront, ControlDim.Mode == NONE ? false : true);
		show();
	}
}

void WS2812Strip::Increment(Control * id, boolean rst)
{
	if (id->Dir == FORWARD)
	{
		if (++id->idx >= id->steps)
		{
			id->end = true;
			if (rst)				{ id->idx = 0; }
			if (OnComplete != NULL)	{ OnComplete(); } // call the comlpetion callback
		}
	}
	else // DirCol == REVERSE
	{
		if (--id->idx <= 0)
		{
			id->end = true;
			if (rst)				{ id->idx = id->steps/*-1*/; }
			if (OnComplete != NULL)	{ OnComplete(); } // call the comlpetion callback
		}
	}
	//Serial.println(id->idx);
}

void WS2812Strip::Reverse(Control * id, boolean rst)
{
	if (!rst)
	{
		id->end = false;
		id->Dir = (direction) (id->Dir == FORWARD ? BACKWARD : FORWARD);
	}
	else
	{
		id->end = false;
		if (id->Dir == FORWARD)
		{
			Backward(id);
			if (rst)	{ id->idx = id->steps/*-1*/; }
		}
		else
		{
			Forward(id);
			if (rst)	{ id->idx = 0; }
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

void WS2812Strip::setColor(uint32_t col, boolean useDim)	// col may become unused
{
	uint16_t Pixels = numPixels();
	
	for (int i = 0 ; i < Pixels ; i++)
	{
		uint32_t output;
		
		/*** Voir comment recuperer la bonne info (getPixelColor par exemple) pour pouvoir cumuler les couleurs et le dim ***/
		if (useDim)	{ output = DimColor(/*col*/getPixelColor(i), DimPix[fromEnd ? Pixels - 1 - i : i]); }
		else		{ output = /*col*/getPixelColor(i); }
		
		setPixelColor(i, output);
		
		/*rLED[i] = Red(output);
		gLED[i] = Green(output);
		bLED[i] = Blue(output);*/
	}
	show();
}

void WS2812Strip::setDim(uint8_t Dim, boolean disp)
{
	memset(DimPix, Dim, sizeof(DimPix));
	if (disp)	{ setColor(0, true); }
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


/**************************************/
/*** DIMMING Manipulation functions ***/
/**************************************/
void WS2812Strip::FadeInit(uint16_t Interval, uint8_t inc, uint8_t limDwn, uint8_t limUp)
{
	ControlDim.Mode = FADE_DIM_ALL;
	ControlDim.interval = Interval;
	valinc = inc;
	mini = limDwn;
	maxi = limUp;
	ControlDim.steps = (maxi - mini) / valinc;
}

void WS2812Strip::FadeUpdate()
{
	boolean setWait = true;
	
	for (int i = 0 ; i < numPixels() ; i++)
	{
		uint8_t NumLED = ControlDim.Dir ? numPixels() - 1 - i : i;
		int8_t Inc = ControlDim.Dir ? -valinc : valinc;
		int16_t Val = max(0, min(255, DimPix[NumLED] + Inc));
		
		if (ControlDim.Dir == FORWARD)
		{
			DimPix[NumLED] = (Val <= maxi) ? Val : maxi;
			if (DimPix[i] != maxi)	{ setWait = false; }
		}
		else
		{
			DimPix[NumLED] = (Val >= mini) ? Val : mini;
			if (DimPix[i] != mini)		{ setWait = false; }
		}
	}
	
	Increment(&ControlDim, false);
	
	if (setWait)
	{
		Hold(&ControlDim);
		Reverse(&ControlDim, true);
	}
}

void WS2812Strip::ProgressiveFadeInit(uint16_t Interval, uint8_t inc, uint8_t thr, boolean startpos, uint8_t limDwn, uint8_t limUp)
{
	ControlDim.Mode = FADE_DIM_LINEAR;
	ControlDim.interval = Interval;
	fromEnd = startpos;
	valinc = inc;
	threshold = thr;
	mini = limDwn;
	maxi = limUp;
	ControlDim.steps = ((maxi - mini) + (numPixels()*(threshold-1))) / valinc;
}

void WS2812Strip::ProgressiveFadeUpdate()
{
	uint16_t Pixels = numPixels();
	boolean reversed = false;
	
	for (int i = 0 ; i < Pixels ; i++)
	{
		uint8_t NumLED = ControlDim.Dir ? Pixels - 1 - i : i;
		int8_t Inc = ControlDim.Dir ? -valinc : valinc;
		int16_t Val = max(0, min(255, DimPix[NumLED] + Inc));
		
		if (ControlDim.Dir == FORWARD)
		{
			if (DimPix[Pixels-1] == maxi)
			{
				Hold(&ControlDim);
				Reverse(&ControlDim, true);
				reversed = true;
			}
			else if ( (NumLED == 0) || (DimPix[NumLED-1] > threshold) )
			{
				DimPix[NumLED] = (Val <= maxi) ? Val : maxi;
			}
		}
		else
		{
			if (DimPix[0] == mini)
			{
				Hold(&ControlDim);
				Reverse(&ControlDim, true);
				reversed = true;
			}
			else if ( (NumLED == Pixels-1) || (DimPix[NumLED+1] < maxi - threshold) )
			{
				DimPix[NumLED] = (Val >= mini) ? Val : mini;
			}
		}
	}
	
	if (!reversed)	{ Increment(&ControlDim, false); }
}


/************************************/
/*** COLOR Manipulation functions ***/
/************************************/
void WS2812Strip::RainbowCycleInit(uint16_t Interval, uint16_t angle, direction dir)
{
	ControlCol.Mode = RAINBOW_CYCLE;
	ControlCol.interval = Interval;
	ControlCol.steps = 255;
	ControlCol.idx = 0;
	ControlCol.Dir = dir;
	coef = (((uint32_t) angle * 256UL) / 360UL);
}

void WS2812Strip::RainbowCycleUpdate()
{
	uint16_t Pixels = numPixels();
	
	for (uint16_t i = 0 ; i < Pixels ; i++)		{ setPixelColor(i, Wheel( ((uint16_t) ((i * coef / Pixels) + ControlCol.idx) & 255))); }
	Increment(&ControlCol, true);
}

void WS2812Strip::TheaterChaseInit(uint32_t color1, uint32_t color2, uint16_t Interval, direction dir)
{
	ControlCol.Mode = THEATER_CHASE;
	ControlCol.interval = Interval;
	ControlCol.steps = numPixels();
	colorPix = color1;
	colorFront = color2;
	ControlCol.idx = 0;
	ControlCol.Dir = dir;
}

void WS2812Strip::TheaterChaseUpdate()
{
	for(int i = 0 ; i < numPixels() ; i++)
	{
		if ((i + ControlCol.idx) % 3 == 0)	{ setPixelColor(i, colorPix); }
		else								{ setPixelColor(i, colorFront); }
	}
	Increment(&ControlCol, true);
}

void WS2812Strip::ColorWipeInit(uint32_t color, uint16_t Interval, direction dir)
{
	ControlCol.Mode = COLOR_WIPE;
	ControlCol.interval = Interval;
	ControlCol.steps = numPixels();
	colorFront = color;
	ControlCol.idx = 0;
	ControlCol.Dir = dir;
}

void WS2812Strip::ColorWipeUpdate()
{
	setPixelColor(ControlCol.idx, colorFront);
	Increment(&ControlCol, true);
}

void WS2812Strip::ScannerInit(uint32_t color1, uint16_t Interval)
{
	ControlCol.Mode = SCANNER;
	ControlCol.interval = Interval;
	ControlCol.steps = (numPixels() - 1) * 2;
	colorPix = color1;
	ControlCol.idx = 0;
}

void WS2812Strip::ScannerUpdate()
{
	for (int i = 0; i < numPixels(); i++)
	{
		if (i == ControlCol.idx)							{ setPixelColor(i, colorPix); }							// Scan Pixel to the right
		else if (i == ControlCol.steps - ControlCol.idx)	{ setPixelColor(i, colorPix); }							// Scan Pixel to the left
		else												{ setPixelColor(i, DimColor(colorPix, 255 >> i)); }	// Fading tail
	}
	Increment(&ControlCol, true);
}

void WS2812Strip::FadeColorInit(uint32_t color1, uint32_t color2, uint16_t steps, uint16_t Interval, direction dir)
{
	ControlCol.Mode = FADE;
	ControlCol.interval = Interval;
	ControlCol.steps = steps;
	colorFront = color2;
	colorBack = color1;
	ControlCol.idx = 0;
	ControlCol.Dir = dir;
}

void WS2812Strip::FadeColorUpdate()
{
	// Calculate linear interpolation between Color1 and Color2
	// Optimise order of operations to minimize truncation error
	uint8_t red = ((Red(colorBack) * (ControlCol.steps - ControlCol.idx)) + (Red(colorFront) * ControlCol.idx)) / ControlCol.steps;
	uint8_t green = ((Green(colorBack) * (ControlCol.steps - ControlCol.idx)) + (Green(colorFront) * ControlCol.idx)) / ControlCol.steps;
	uint8_t blue = ((Blue(colorBack) * (ControlCol.steps - ControlCol.idx)) + (Blue(colorFront) * ControlCol.idx)) / ControlCol.steps;
	
	setColor(Color(red, green, blue), false);
	Increment(&ControlCol, true);
}

void WS2812Strip::WaveInit(uint32_t colBgd, uint32_t colWave, uint16_t steps, uint16_t Interval, direction dir)
{
	
}

void WS2812Strip::WaveUpdate()
{
	
}

void WS2812Strip::PlainColorUpdate()
{
	for (int i = 0 ; i < numPixels() ; i++)
	{
		setPixelColor(i, colorFront);
	}
	//Increment(&ControlCol, true);
}

