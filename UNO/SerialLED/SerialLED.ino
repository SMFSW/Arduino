/* 
  ArduinoSerialLED.ino 
  turn LED 'L' on and off via 'Serial' Functionality on Arduino Board 
 */ 
  
 // use output pin no. 13 / on board LED 'L' 
 const unsigned int LED_PIN = 13;  
 
 // use standard baud rate 9600 baud. 
 const unsigned long BAUD_RATE = 9600;  
 // Other baud rates are possible, too, e.g. 57600. Just make sure your  
 // Docklight project settings are adjusted accordingly  
 // (Docklight menu Tools -> Project Settings ... -> Communication). 
  
 // init command buffer 
 String command = ""; 
 boolean command_available = false;   
 
// the setup routine runs once when you press reset: 
void setup() { 
  // initialize serial communication according to BAUD_RATE: 
  Serial.begin(BAUD_RATE); // other settings are SERIAL_8N1, 8 data bits, no parity, one stop bit 
  while (!Serial) { 
    ; // wait for serial port to connect. Needed for Leonardo only 
  } 
  pinMode(LED_PIN, OUTPUT); // configure LED_PIN as output 
 
  // Serial.available() < 0 would indicate an error on initialization 
  if (Serial.available() >= 0)  
     Serial.println("Ready");   // send Ready<CR><LF> at start/reset 
} 
 
// after completing setup(), this gets called consecutively  
void loop() { 
  if (command_available) { 
    // commands received need to be terminated by <CR><LF> characters 
  // (same as Serial.println does it)  
    if (command == "on\r\n") { 
      digitalWrite(LED_PIN, HIGH); 
      Serial.println("LED is on"); 
    } 
    else if (command == "off\r\n") { 
      digitalWrite(LED_PIN, LOW); 
      Serial.println("LED is off"); 
    } 
    else { 
      Serial.print("Unknown command: "+ command); 
    } 
    command = ""; // clear input buffer 
    command_available = false; 
  } 
} 
 
// event Handling - reading characters 
void serialEvent(){ 
  while (Serial.available() && !command_available) { 
    const char c = Serial.read(); 
    if (c == '\n') { // Linefeed character is end-of-command mark 
      command += c; 
      command_available = true; 
    } 
    else command += c;  // add character for receive string 
  } 
}
