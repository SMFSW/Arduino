// Demo the quad alphanumeric display LED backpack kit
// scrolls through every character, then scrolls Serial
// input onto the display

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

extern unsigned int vitKmh;

void setup_I2C_7Seg() {
  alpha4.begin(0x70);  // pass in the address

  alpha4.writeDigitRaw(3, 0x0);
  alpha4.writeDigitRaw(0, 0xFFFF);
  alpha4.writeDisplay();
  delay(100);
  alpha4.writeDigitRaw(0, 0x0);
  alpha4.writeDigitRaw(1, 0xFFFF);
  alpha4.writeDisplay();
  delay(100);
  alpha4.writeDigitRaw(1, 0x0);
  alpha4.writeDigitRaw(2, 0xFFFF);
  alpha4.writeDisplay();
  delay(100);
  alpha4.writeDigitRaw(2, 0x0);
  alpha4.writeDigitRaw(3, 0xFFFF);
  alpha4.writeDisplay();
  delay(100);
  
  alpha4.clear();
  alpha4.writeDisplay();

  // display every character, 
  for (uint8_t i='!'; i<='6'; i++) {
    alpha4.writeDigitAscii(0, i);
    alpha4.writeDigitAscii(1, i+1);
    alpha4.writeDigitAscii(2, i+2);
    alpha4.writeDigitAscii(3, i+3);
    alpha4.writeDisplay();
    delay(100);
  }
  Serial.println("Start typing to display!");
}


char displaybuffer[4] = {' ', ' ', ' ', ' '};

void loop_I2C_7Seg() {
  //while (! Serial.available()) return;
  
  /*char c = Serial.read();
  if (! isprint(c)) return; // only printable!
  
  // scroll down display
  displaybuffer[0] = displaybuffer[1];
  displaybuffer[1] = displaybuffer[2];
  displaybuffer[2] = displaybuffer[3];
  displaybuffer[3] = c;*/
 
  displaybuffer[0] = 'V';
  displaybuffer[1] = '0' + (vitKmh / 100);
  displaybuffer[2] = '0' + (vitKmh >= 100 ? ((vitKmh - 100)/10) : (vitKmh/10));
  displaybuffer[3] = '0' + (vitKmh % 10);
  
  // set every digit to the buffer
  alpha4.writeDigitAscii(0, displaybuffer[0]);
  alpha4.writeDigitAscii(1, displaybuffer[1]);
  alpha4.writeDigitAscii(2, displaybuffer[2]);
  alpha4.writeDigitAscii(3, displaybuffer[3]);
 
  // write it out!
  alpha4.writeDisplay();
}
