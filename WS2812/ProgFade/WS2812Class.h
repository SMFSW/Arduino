#include <Adafruit_NeoPixel.h>

// Patern directions supported:
enum	direction { FORWARD = 0, BACKWARD };
enum	status { WAIT = 0, RESET, RUN, RUN_FADE, RUN_COLOR };
enum	mode {	NONE = 0,
				FADE_DIM_LINEAR = 0x01, FADE_DIM_ALL,
				RAINBOW_CHASE = 0x11, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE, WAVE
};

typedef struct Control{
	status		Status;
	mode		Mode;
	direction	Dir;
	uint16_t	idx;
	uint16_t	steps;
	uint16_t	interval;
	uint16_t	memotime;
};

// WS2812 Class - extend Adafruit_NeoPixel class
class WS2812Strip : public Adafruit_NeoPixel
{
	private:
	// private functions
	void updateDimPix(uint16_t n);
	inline void StripUpdateCol();
	inline void StripUpdateDim();
	uint32_t DimColor(uint32_t col, uint8_t Dim);
	
	void RainbowChaseUpdate();
	void RainbowCycleUpdate();
	void TheaterChaseUpdate();
	void ColorWipeUpdate();
	void ScannerUpdate();
	void FadeColorUpdate();
	void WaveUpdate();
	void SimpleColorUpdate();

	public: // here for spy over Serial
	
	//public:
	// Color params
	Control		ControlCol;
	
	uint32_t	colorBack;
	uint32_t	colorFront;
	uint32_t	colorPix;
	uint32_t	colorLatest;
	
//	uint8_t		rLED[NbLEDsStrip1];
//	uint8_t		gLED[NbLEDsStrip1];
//	uint8_t		bLED[NbLEDsStrip1];
	
	// Dim params
	Control		ControlDim;
	
	uint8_t *	DimPix;

	uint8_t		valinc;
	uint8_t		threshold;
	boolean		fromEnd;
	void		(*OnComplete)();  // Callback on completion

	// Constructor - calls base-class constructor to initialize strip
	WS2812Strip(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
	:Adafruit_NeoPixel(pixels, pin, type)
	{
		OnComplete = callback;
		updateDimPix(pixels);	// Create DimPix tab
	}
	
	void Update();	// Update Pixels (action to perform manually)

	// Status control
	inline void Hold(Control * id)	{ id->Status = WAIT; }
	inline void Reset(Control * id)	{ id->Status = RESET; }
	inline void Run(Control * id)		{ id->Status = RUN; }
	
	// Direction control
	void Increment(Control * id, boolean rst = false);
	void Reverse(Control * id, boolean rst = false);
	inline void Forward(Control * id)  { id->Dir = FORWARD; }
	inline void Backward(Control * id) { id->Dir = BACKWARD; }
 
	void setColor(uint32_t color, boolean useDim);	// Set all pixels to a color (synchronously)

	inline uint8_t Red(uint32_t color)		{ return (color >> 16) & 0xFF; }	// Returns the Red component of a 32-bit color
	inline uint8_t Green(uint32_t color)	{ return (color >> 8) & 0xFF; }		// Returns the Green component of a 32-bit color
	inline uint8_t Blue(uint32_t color)		{ return color & 0xFF; }			// Returns the Blue component of a 32-bit color

	// Input a value 0 to 255 to get a color value.
	// The colours are a transition r - g - b - back to r.
	uint32_t Wheel(uint8_t WheelPos);

	// Dimming functions
	void FadeInit(uint8_t inc, uint16_t Interval);
	void FadeUpdate();

	void ProgressiveFadeInit(uint8_t inc, uint8_t thr, uint16_t Interval, boolean startpos = false);
	void ProgressiveFadeUpdate();

	// Color functions
	void RainbowChaseInit(uint16_t Interval, direction dir = FORWARD);
	void RainbowCycleInit(uint16_t Interval, direction dir = FORWARD);
	void TheaterChaseInit(uint32_t color1, uint32_t color2, uint16_t Interval, direction dir = FORWARD);
	void ColorWipeInit(uint32_t color, uint16_t Interval, direction dir = FORWARD);
	void ScannerInit(uint32_t color1, uint16_t interval);
	void FadeColorInit(uint32_t color1, uint32_t color2, uint16_t steps, uint16_t Interval, direction dir = FORWARD);
	void WaveInit(uint32_t colBgd, uint32_t colWave, uint16_t steps, uint16_t Interval, direction dir = FORWARD);
};
