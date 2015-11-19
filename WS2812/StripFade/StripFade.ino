#include <Adafruit_NeoPixel.h>
#include "WS2812StripClass.h"

#define	NbLEDsStrip1	30//9
#define  NbLEDsStrip2  9

void ledStrip1Complete(); // Defines scenario completion routine call from WS2812 library
void ledStrip2Complete(); // Defines scenario completion routine call from WS2812 library
void ledStrip3Complete(); // Defines scenario completion routine call from WS2812 library

WS2812Strip ledStrip1(NbLEDsStrip1, 6, NEO_GRB + NEO_KHZ800, ledStrip1Complete);
WS2812Strip ledStrip2(NbLEDsStrip2, 4, NEO_GRB + NEO_KHZ800, ledStrip2Complete);

WS2812Strip ledStrip3(12, 2, NEO_GRB + NEO_KHZ800, ledStrip3Complete);

#define	pinPB1	8
#define	pinPB2	9

static boolean memoBP1 = false, memoBP2 = false;

// Initialize everything and prepare to start
void setup()
{
	//Serial.begin(9600);
	
   pinMode(pinPB1, INPUT_PULLUP);
   pinMode(pinPB2, INPUT_PULLUP);
    
    // Initialize all the pixelStrips
    ledStrip1.begin();
    ledStrip2.begin();
    ledStrip3.begin();
    
    // Kick off a pattern
    //ledStrip1.Wave(ledStrip1.Color1, ledStrip1.Color(0,0,255), 128, 100);
    ledStrip1.ProgressiveFade(ledStrip1.Color(0,0,0), ledStrip1.Color(255,255,255), 255, 50, FORWARD, WAIT);
    ledStrip2.ProgressiveFade(ledStrip2.Color(0,0,0), ledStrip2.Color(255,255,255), 255, 75, FORWARD, WAIT);
    
    ledStrip3.Scanner(ledStrip3.Wheel(random(255)), 200);
}

// Main loop
void loop()
{
    // Update the rings.
    ledStrip1.Update();
    ledStrip2.Update();
    ledStrip3.Update();
    
    // Switch patterns on a button press:
    if (digitalRead(pinPB1) == LOW) // Button #1 pressed
    {
        if (memoBP1 == false)
        {
          memoBP1 = true;
          ledStrip1.Reverse(false);
          ledStrip2.Reverse(false);
          
          //ledStrip1.ActivePattern = FADE;
          //ledStrip1.Interval = 20;

          ledStrip3.Color1 = ledStrip3.Wheel(random(255));
        }
    }
    else { memoBP1 = false; }
    
    if (digitalRead(pinPB2) == LOW) // Button #2 pressed
    {
        if (memoBP2 == false)
        {
          memoBP2 = true;
          
          ledStrip1.ProgressiveFade(ledStrip1.Color2, ledStrip1.Wheel(random(255)), 255, 50, FORWARD);
          ledStrip2.ProgressiveFade(ledStrip2.Color2, ledStrip1.Wheel(random(255)), 1024, 200, FORWARD);
          //ledStrip1.ActivePattern = COLOR_WIPE;
        }
    }
    else { memoBP2 = false; }
}

//------------------------------------------------------------
//Completion Routines - get called on completion of a pattern
//------------------------------------------------------------

// ledStrip1 Completion Callback
void ledStrip1Complete()
{
}

// ledStrip2 Completion Callback
void ledStrip2Complete()
{
}

// ledStrip3 Completion Callback
void ledStrip3Complete()
{
  ledStrip3.Reverse(false);
}


