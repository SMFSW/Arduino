#define ENCODER_DO_NOT_USE_INTERRUPTS
#include "Encoder.h"

#define swPin 7
Encoder pot(5, 6);

static long memoencode = 750L;
static boolean memosw = false;

void setup_Encoder()
{
  pinMode(swPin, INPUT_PULLUP);   // Onboard LED
  pot.write(750);
}

int loop_Encoder()
{
  long encode = pot.read();
  
  if (  (digitalRead(swPin) == LOW)
      &&(memosw == false))
  {
    memosw = true;
    pot.write(750);
  }
  else  { memosw = false; }

  if (memoencode != encode)
  {
    memoencode = encode;
    //Serial.println(encode);
  }

  return encode;
}
