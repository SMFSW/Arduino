#include <Adafruit_NeoPixel.h>
#include "NeoPixClass.h"

// Define some NeoPatterns for the two rings and the stick
//  as well as some completion routines
NeoPatterns Ring1(12, 0, NEO_GRB + NEO_KHZ800, &Ring1Complete);
NeoPatterns Ring2(12, 1, NEO_GRB + NEO_KHZ800, &Ring2Complete);
NeoPatterns Ring3(12, 2, NEO_GRB + NEO_KHZ800, &Ring3Complete);

// Initialize everything and prepare to start
void setup()
{
  //Serial.begin(115200);

   pinMode(8, INPUT_PULLUP);
   pinMode(9, INPUT_PULLUP);
    
    // Initialize all the pixelStrips
    Ring1.begin();
    Ring2.begin();
    Ring3.begin();
    
    // Kick off a pattern
    Ring1.TheaterChase(Ring1.Color(255,255,0), Ring1.Color(0,0,50), 100);
    Ring2.RainbowCycle(3);
    Ring2.Color1 = Ring1.Color1;
    Ring3.Scanner(Ring1.Color(255,0,0), 55);
}

// Main loop
void loop()
{
    // Update the rings.
    Ring1.Update();
    Ring2.Update();    
    
    // Switch patterns on a button press:
    if (digitalRead(8) == LOW) // Button #1 pressed
    {
        // Switch Ring1 to FASE pattern
        Ring1.ActivePattern = FADE;
        Ring1.Interval = 20;
        // Speed up the rainbow on Ring2
        Ring2.Interval = 0;
        // Set stick to all red
        Ring3.ColorSet(Ring3.Color(255, 0, 0));
    }
    else if (digitalRead(9) == LOW) // Button #2 pressed
    {
        // Switch to alternating color wipes on Rings1 and 2
        Ring1.ActivePattern = COLOR_WIPE;
        Ring2.ActivePattern = COLOR_WIPE;
        Ring2.TotalSteps = Ring2.numPixels();
        // And update tbe stick
        Ring3.Update();
    }
    else // Back to normal operation
    {
        // Restore all pattern parameters to normal values
        Ring1.ActivePattern = THEATER_CHASE;
        Ring1.Interval = 100;
        Ring2.ActivePattern = RAINBOW_CYCLE;
        Ring2.TotalSteps = 255;
        Ring2.Interval = min(10, Ring2.Interval);
        // And update tbe stick
        Ring3.Update();
    }    
}

//------------------------------------------------------------
//Completion Routines - get called on completion of a pattern
//------------------------------------------------------------

// Ring1 Completion Callback
void Ring1Complete()
{
    if (digitalRead(9) == LOW)  // Button #2 pressed
    {
        // Alternate color-wipe patterns with Ring2
        Ring2.Interval = 40;
        Ring1.Color1 = Ring1.Wheel(random(255));
        Ring1.Interval = 20000;
    }
    else  // Retrn to normal
    {
      Ring1.Reverse();
    }
}

// Ring 2 Completion Callback
void Ring2Complete()
{
    if (digitalRead(9) == LOW)  // Button #2 pressed
    {
        // Alternate color-wipe patterns with Ring1
        Ring1.Interval = 20;
        Ring2.Color1 = Ring2.Wheel(random(255));
        Ring2.Interval = 20000;
    }
    else  // Retrn to normal
    {
        Ring2.RainbowCycle(random(0,10));
    }
}

// Stick Completion Callback
void Ring3Complete()
{
    // Random color change for next scan
    Ring3.Color1 = Ring3.Wheel(random(255));
}

