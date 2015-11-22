#include <Adafruit_NeoPixel.h>

// Patern directions supported:
enum	direction { FORWARD = 0, BACKWARD };
enum	status { WAIT = 0, RESET, RUN };
enum	mode {	NONE = 0,
				FADE_DIM_LINEAR = 0x01, FADE_DIM_ALL,
				RAINBOW_CHASE = 0x11, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE, WAVE
};

typedef struct Index{
	direction	Dir;
	uint16_t	idx;
	uint16_t	steps;
};

// WS2812 Class - extend Adafruit_NeoPixel class
class WS2812Strip : public Adafruit_NeoPixel
{
	private:
	// private functions
	void updateDimPix(uint16_t n);
	void StripUpdate();
	uint32_t DimColor(uint32_t col, uint8_t Dim);

	uint16_t	memotime;

	public: // here for spy over Serial
	
	//public:
	status		Status;
	
	// Color params
	mode		ModeCol;
	Index		IndexCol;
	
	uint32_t	colorBack;
	uint32_t	colorFront;
	uint32_t	colorLatest;
	
//	uint8_t		rLED[NbLEDsStrip1];
//	uint8_t		gLED[NbLEDsStrip1];
//	uint8_t		bLED[NbLEDsStrip1];
	
	// Dim params
	mode		ModeDim;
	Index		IndexDim;
	
	uint8_t *	DimPix;

	uint8_t		valinc;
	uint8_t		threshold;
	uint16_t	interval;
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
	inline void Hold()		{ Status = WAIT; }
	inline void Reset()		{ Status = RESET; }
	inline void Run()		{ Status = RUN; }
	
	// Direction control
	void Increment(Index * id, boolean rst);
	void Reverse(Index * id, boolean rst);
	inline void Forward(Index * id)  { id->Dir = FORWARD; }
	inline void Backward(Index * id) { id->Dir = BACKWARD; }
 
	void setColor(uint32_t color, boolean useDim);	// Set all pixels to a color (synchronously)

	inline uint8_t Red(uint32_t color)		{ return (color >> 16) & 0xFF; }	// Returns the Red component of a 32-bit color
	inline uint8_t Green(uint32_t color)	{ return (color >> 8) & 0xFF; }		// Returns the Green component of a 32-bit color
	inline uint8_t Blue(uint32_t color)		{ return color & 0xFF; }			// Returns the Blue component of a 32-bit color

	// Input a value 0 to 255 to get a color value.
	// The colours are a transition r - g - b - back to r.
	uint32_t Wheel(uint8_t WheelPos);

	void FadeInit(uint8_t inc);
	void FadeUpdate();

	void ProgressiveFadeInit(uint8_t inc, uint8_t thr, boolean startpos);
	void ProgressiveFadeUpdate();
};
