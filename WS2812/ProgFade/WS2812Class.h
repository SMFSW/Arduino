#include <Adafruit_NeoPixel.h>

// Patern directions supported:
enum	direction { FORWARD = 0, BACKWARD };
enum	status { WAIT = 0, RESET, RUN };
enum	mode { NONE = 0, FADE_DIM_LINEAR, FADE_DIM_ALL };

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
	uint16_t	idx;
	uint16_t	steps;

	//public:
	status		Status;
	
	// Color params
	mode		ModeCol;
	uint32_t	colorBack;
	uint32_t	colorFront;
	uint32_t	colorLatest;
	direction	DirCol;
	
//	uint8_t		DimPix[NbLEDsStrip1];
//	uint8_t		rLED[NbLEDsStrip1];
//	uint8_t		gLED[NbLEDsStrip1];
//	uint8_t		bLED[NbLEDsStrip1];
	
	// Dim params
	mode		ModeDim;
	direction	DirDim;
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

	// Direction control
	inline void Forward()	{ DirDim = FORWARD; }
	inline void Backward()	{ DirDim = BACKWARD; }
	void Reverse(boolean rst); // Reverse direction
	// Status control
	inline void Hold()		{ Status = WAIT; }
	inline void Reset()		{ Status = RESET; }

	void Increment();	// Increment the Index

	void setDimColor();	// Set all pixels to their dimmed color (synchronously)
	
	void setColor(uint32_t color);	// Set all pixels to a color (synchronously)

	// Returns the Red component of a 32-bit color
	uint8_t Red(uint32_t color);

	// Returns the Green component of a 32-bit color
	uint8_t Green(uint32_t color);

	// Returns the Blue component of a 32-bit color
	uint8_t Blue(uint32_t color);

	// Input a value 0 to 255 to get a color value.
	// The colours are a transition r - g - b - back to r.
	uint32_t Wheel(uint8_t WheelPos);

	void FadeInit(uint8_t inc);
	void FadeUpdate();

	void ProgressiveFadeInit(uint8_t inc, uint8_t thr, boolean startpos);
	void ProgressiveFadeUpdate();
};
