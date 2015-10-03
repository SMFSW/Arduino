
/*
  XBEE #1 RECEIVER
  ================
  Button-Arduino-Xbee#2 > Xbee#1-Arduino-led
  Most code from : XBee Analog Duplex sender
  http://www.faludi.com/classes/sociableobjects/code/XBee_Analog_Duplex_Sender.pde
 */
 
const int led = 12; 
String inputString = "";
int receiveValue;

void setup() {
  Serial.begin(9600);      
  pinMode (led, OUTPUT);
  setDestination();
  blink(led, 3);
}

void loop() {
  // Receive serial data from xbee #2
  if (Serial.available() > 0) {
     handleSerial();
  } 
}

// Xbee configuration
void setDestination() {
  Serial.print("+++");
  char thisByte = 0;
  while (thisByte != '\r') {
    if (Serial.available() > 0) {
      thisByte = Serial.read(); 
    }
  }
  Serial.print("ATRE\r");
  Serial.print("ATMY0\r"); 
  Serial.print("ATID1111\r");
  Serial.print("ATCN\r");
}

// Blink 
void blink(int thisPin, int howManyTimes) {
  for (int blinks=0; blinks< howManyTimes; blinks++) {
    digitalWrite(thisPin, HIGH);
    delay(200);
    digitalWrite(thisPin, LOW);
    delay(200);  
  }
}

void handleSerial() {
  char inByte = Serial.read();

   // save only ASCII numeric characters (ASCII 0 - 9):
  if (isDigit(inByte)){
    inputString = inputString + inByte;
  }
  
  // if you get an ASCII newline:
  if (inByte == '\n') {
    // convert the string to a number:
     receiveValue = inputString.toInt();
   // set the analog output LED:
    digitalWrite(led, receiveValue);
    // clear the input string for the 
    // next value:
    inputString = "";
    //Serial.print(receiveValue);
  }
}
