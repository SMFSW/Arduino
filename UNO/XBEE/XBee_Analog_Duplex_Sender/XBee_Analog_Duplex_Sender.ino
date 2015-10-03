/*
  XBee Analog Duplex sender
  Language: Wiring/Arduino

  This sketch configures an XBee radio via the serial port,
  sends the value of an analog sensor out, and listens for 
  input from the radio, using it to set the value of
  a PWM output.
  
  Thanks to Robert Faludi for the critique and improvements
*/
#define sensorPin 0         // input sensor
#define txLed 2             // LED to indicate outgoing data
#define rxLed 3             // LED to indicate incoming data
#define analogLed 9         // LED that will change brightness with incoming value
#define threshold 10        // how much change you need to see on 
                            // the sensor before sending

int lastSensorReading = 0;  // previous state of the switch       

int inByte= -1;             // incoming byte from serial RX
char inString[6];           // string for incoming serial data
int stringPos = 0;          // string index counter

void setup() {
  // configure serial communications:
  Serial.begin(9600);      

  // configure output pins:
  pinMode(txLed, OUTPUT);
  pinMode(rxLed, OUTPUT);
  pinMode (analogLed, OUTPUT);

  // set XBee's destination address:
  setDestination();
  // blink the TX LED indicating that the main program's about to start:
  blink(3);
}
void setDestination() {
  // put the radio in command mode:
  Serial.print("+++");
  // wait for the radio to respond with "OK\r"
  char thisByte = 0;
  while (thisByte != '\r') {
    if (Serial.available() > 0) {
      thisByte = Serial.read(); 
    }
  }

  // set the destination address, using 16-bit addressing.
  // if you're using two radios, one radio's destination 
  // should be the other radio's MY address, and vice versa:
  Serial.print("ATDH0, DL5678\r");
  // set my address using 16-bit addressing:
  Serial.print("ATMY1234\r"); 
  // set the PAN ID. If you're working in a place where many people
  // are using XBees, you should set your own PAN ID distinct
  // from other projects.
  Serial.print("ATID1111\r");
  // put the radio in data mode:
  Serial.print("ATCN\r");
}
// Blink the tx LED:
void blink(int howManyTimes) {
  for (int i=0; i< howManyTimes; i++) {
    digitalWrite(txLed, HIGH);
    delay(200);
    digitalWrite(txLed, LOW);
    delay(200);  
  }
}

void loop() {
  // listen for incoming serial data:
  if (Serial.available() > 0) {
    // turn on the RX LED whenever you're reading data:
    digitalWrite(rxLed, HIGH);
    handleSerial();
  } 
  else {
    // turn off the receive LED when there's no incoming data:
    digitalWrite(rxLed, LOW); 
  }

  // listen to the potentiometer:
  char sensorValue = readSensor();

  // if there's something to send, send it:
  if (sensorValue > 0) {
    //light the tx LED to say you're sending:
    digitalWrite(txLed, HIGH);

    Serial.print(sensorValue, DEC );
    Serial.print("\r");

    // turn off the tx LED:
    digitalWrite(txLed, LOW);
  }
}
void handleSerial() {
  inByte = Serial.read();
  // save only ASCII numeric characters (ASCII 0 - 9):
  if ((inByte >= '0') && (inByte <= '9')){
    inString[stringPos] = inByte;
    stringPos++;
  }
  // if you get an ASCII carriage return:

  if (inByte == '\r') {
    // convert the string to a number:
    int brightness = atoi(inString);
    // set the analog output LED:
    analogWrite(analogLed, brightness);

    // put zeroes in the array
    for (int c = 0; c < stringPos; c++) {
      inString[c] = 0;
    } 
    // reset the string pointer:
    stringPos = 0;
  }
}

char readSensor() {
  char message = 0;
  // read the sensor:
  int sensorReading = analogRead(sensorPin);

  // look for a change from the last reading
  // that's greater than the threshold:
  if (abs(sensorReading - lastSensorReading) > threshold) {
    message = sensorReading/4;
    lastSensorReading = sensorReading;
  } 
  return message;
}
