#include <SeqTimer.h>

const int xPin = A0;
const int yPin = A1;
const int swPin = 8;

SeqTimer timJoystick;

void setup()
{
	pinMode(swPin,INPUT_PULLUP);
	//digitalWrite(swPin, HIGH);
	timJoystick.init(500); // Maj toutes les 500ms
	Serial.begin(9600);
}

void loop()
{
	if (timJoystick.getTimer())
	{
		
		
		Serial.print("X: ");
		Serial.print(analogRead(xPin),DEC);
		Serial.print("|Y: ");
		Serial.print(analogRead(yPin),DEC);
		Serial.print("|Z: ");
		Serial.println(digitalRead(swPin));
	}
}
