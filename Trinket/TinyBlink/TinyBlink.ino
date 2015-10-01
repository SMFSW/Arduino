// Wire lib (on tiny)
#include <TinyWireM.h>
#include <USI_TWI_Master.h>

#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// Liquid Crystal (on tiny)
// #include <TinyAdafruit_MCP23008.h>
// #include <TinyLiquidCrystal.h>



/*
  Blink
  Turns on an LED on for a period, then off for another period, repeatedly.

  On the Adafruit Trinket, on-board LED is attached to digital pin 1. */


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 1 as an output (LED sur carte Adafruit Trinket).
  pinMode(1, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(1, LOW);    // turn the LED off by making the voltage LOW
  delay(3000);              // wait for a second
}
