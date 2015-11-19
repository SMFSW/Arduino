#include <Adafruit_NeoPixel.h>
#include "WS2812StripClass.h"

#define	NbLEDs	9

unsigned char LEDBright[NbLEDs];

WS2812Strip ledStrip1(NbLEDs, 6, NEO_GRB + NEO_KHZ800, NULL);

#define	pinPB1	8
#define	pinPB2	9

// Initialize everything and prepare to start
void setup()
{
	Serial.begin(9600);
	
   pinMode(pinPB1, INPUT_PULLUP);
   pinMode(pinPB2, INPUT_PULLUP);
    
    // Initialize all the pixelStrips
    ledStrip1.begin();
    
    // Kick off a pattern
    //ledStrip1.Wave(ledStrip1.Color1, ledStrip1.Color(0,0,255), 128, 100);
    ledStrip1.ProgressiveFade(ledStrip1.Color(0,0,0), ledStrip1.Color(255,255,200), 255, 25, FORWARD);
}

// Main loop
void loop()
{
    // Update the rings.
    ledStrip1.Update();
    
    // Switch patterns on a button press:
    if (digitalRead(pinPB1) == LOW) // Button #1 pressed
    {
        // Switch ledStrip1 to FASE pattern
        ledStrip1.ActivePattern = FADE;
        ledStrip1.Interval = 20;
    }
    else if (digitalRead(pinPB2) == LOW) // Button #2 pressed
    {
        // Switch to alternating color wipes on ledStrips1 and 2
        ledStrip1.ActivePattern = COLOR_WIPE;
    }
    else // Back to normal operation
    {
        // Restore all pattern parameters to normal values
        //ledStrip1.ActivePattern = THEATER_CHASE;
        //ledStrip1.Interval = 100;
    }    
}

//------------------------------------------------------------
//Completion Routines - get called on completion of a pattern
//------------------------------------------------------------

// ledStrip1 Completion Callback
void ledStrip1Complete()
{
    if (digitalRead(pinPB2) == LOW)  // Button #2 pressed
    {
        // Alternate color-wipe patterns with ledStrip2
        //ledStrip2.Interval = 40;
        ledStrip1.Color1 = ledStrip1.Wheel(random(255));
        ledStrip1.Interval = 20000;
    }
    else  // Retrn to normal
    {
      ledStrip1.Reverse();
    }
}


