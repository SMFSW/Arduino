/*
This example is for Series 2 XBee
Receives a ZB RX packet and sets a PWM value based on packet data.
Error led is flashed if an unexpected packet is received
*/

#include <SeqTimer.h>
#include <XBee.h>
#include <LiquidCrystal.h>
 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel
SeqTimer timerLCD;

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();


int statusLed = 13;
int errorLed = 13;
int dataLed = 13;

void flashLed(int pin, int times, int wait) {
	for (int i = 0; i < times; i++)
	{
		digitalWrite(pin, HIGH);
		delay(wait);
		digitalWrite(pin, LOW);
		
		if (i + 1 < times) { delay(wait); }
	}
}



void setup(){
	lcd.begin(16, 2);		// start the library

	pinMode(statusLed, OUTPUT);
	pinMode(errorLed, OUTPUT);
	pinMode(dataLed,  OUTPUT);

	timerLCD.init(250);

	// start serial
	Serial.begin(9600);
	xbee.begin(Serial);

	flashLed(statusLed, 3, 50);
}


void loop()
{
	if (timerLCD.getTimer())	{ displayLCD(); }
	
	xbee.readPacket();

	if (xbee.getResponse().isAvailable())	// got something
	{
		if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)	// got a zb rx packet
		{
			
			// now fill our zb rx class
			xbee.getResponse().getZBRxResponse(rx);
			
			if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED)	{ flashLed(statusLed, 10, 10); }	// the sender got an ACK
			else											{ flashLed(errorLed, 2, 20); }		// we got it (obviously) but sender didn't get an ACK
			
			// set dataLed PWM to value of the first byte in the data
			analogWrite(dataLed, rx.getData(0));
		}
		else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE)
		{
			xbee.getResponse().getModemStatusResponse(msr);
			// the local XBee sends this response on certain events, like association/dissociation
			
			if (msr.getStatus() == ASSOCIATED) { flashLed(statusLed, 10, 10); }	// yay this is great.  flash led
			else if (msr.getStatus() == DISASSOCIATED) { flashLed(errorLed, 10, 10); }	// this is awful.. flash led to show our discontent
			else { flashLed(statusLed, 5, 10); }// another status
		}
		else
		{
			// not something we were expecting
			flashLed(errorLed, 1, 25);    
		}
	}
	else if (xbee.getResponse().isError())
	{
		/*nss*/Serial.print("Error reading packet.  Error code: ");  
		/*nss*/Serial.println(xbee.getResponse().getErrorCode());
	}
}
