#include <Adafruit_NeoPixel.h>
#include "WS2812StripClass.h"

// Define some WS2812Strip for the two rings and the stick
//  as well as some completion routines
WS2812Strip ledStrip1(12, 7, NEO_GRB + NEO_KHZ800, &ledStrip1Complete);
WS2812Strip ledStrip2(12, 12, NEO_GRB + NEO_KHZ800, &ledStrip2Complete);
WS2812Strip ledStrip3(30, 11, NEO_GRB + NEO_KHZ800, &ledStrip3Complete);

#define	pinPB1	8
#define	pinPB2	9

// Initialize everything and prepare to start
void setup()
{
  //Serial.begin(115200);

   pinMode(pinPB1, INPUT_PULLUP);
   pinMode(pinPB2, INPUT_PULLUP);
    
    // Initialize all the pixelStrips
    ledStrip1.begin();
    ledStrip2.begin();
    ledStrip3.begin();
    
    // Kick off a pattern
    //ledStrip1.TheaterChase(ledStrip1.Color(255,255,0), ledStrip1.Color(0,0,50), 100);
    ledStrip1.Wave(ledStrip1.Color1, ledStrip1.Color(0,0,255), 128, 100);
    ledStrip2.RainbowChase(100);
    ledStrip2.Color1 = ledStrip1.Color1;
    ledStrip3.Scanner(ledStrip1.Color(255,0,0), 100);
}

// Main loop
void loop()
{
    // Update the rings.
    ledStrip1.Update();
    ledStrip2.Update();    
    
    // Switch patterns on a button press:
    if (digitalRead(pinPB1) == LOW) // Button #1 pressed
    {
        // Switch ledStrip1 to FASE pattern
        ledStrip1.ActivePattern = FADE;
        ledStrip1.Interval = 20;
        // Speed up the rainbow on ledStrip2
        ledStrip2.Interval = 0;
        // Set stick to all red
        ledStrip3.ColorSet(ledStrip3.Color(255, 0, 0));
    }
    else if (digitalRead(pinPB2) == LOW) // Button #2 pressed
    {
        // Switch to alternating color wipes on ledStrips1 and 2
        ledStrip1.ActivePattern = COLOR_WIPE;
        ledStrip2.ActivePattern = COLOR_WIPE;
        ledStrip2.TotalSteps = ledStrip2.numPixels();
        // And update tbe stick
        ledStrip3.Update();
    }
    else // Back to normal operation
    {
        // Restore all pattern parameters to normal values
        //ledStrip1.ActivePattern = THEATER_CHASE;
        //ledStrip1.Interval = 100;
        ledStrip2.ActivePattern = RAINBOW_CHASE;
        //ledStrip2.TotalSteps = 255;
        //ledStrip2.Interval = min(10, ledStrip2.Interval);
        // And update tbe stick
        ledStrip3.Update();
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

// ledStrip 2 Completion Callback
void ledStrip2Complete()
{
    if (digitalRead(pinPB1) == LOW)  // Button #2 pressed
    {
        // Alternate color-wipe patterns with ledStrip1
        //ledStrip1.Interval = 20;
        ledStrip2.Color1 = ledStrip2.Wheel(random(255));
        ledStrip2.Interval = 20000;
    }
    else  // Retrn to normal
    {
        //ledStrip2.RainbowCycle(random(0,10));
    }
}

// Stick Completion Callback
void ledStrip3Complete()
{
    // Random color change for next scan
    ledStrip3.Color1 = ledStrip3.Wheel(random(255));
}

