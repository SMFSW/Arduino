#include <XBee.h>
//--- XBEE-SETUP ---------------------------
 XBee xbee = XBee();
 unsigned long start = millis();
// allocate two bytes for to hold a 10-bit analog reading
uint8_t payload[7];
// payload array lenght needs to be at least one higher than the number of bytes to be transmitted
// 16-bit addressing: Enter address of remote XBee, typically the coordinator
  Tx16Request tx_5001 = Tx16Request(0x5001, payload, sizeof(payload));
  TxStatusResponse txStatus = TxStatusResponse();
//values to be transmitted 10 Bit, i.e. (0-1023)
int value1 = 0;
int value2 = 0;
int statusLed = 11;
int errorLed = 12;
void setup(void)
{
// initialise Xbee feedback LEDs
 pinMode(statusLed, OUTPUT);
 pinMode(errorLed, OUTPUT);
xbee.begin(9600);
Serial.begin(9600);
}
void loop(void)
{ 
// start transmitting after a startup delay. Note: this will rollover to 0 eventually so not best way to handle
 if (millis() - start > 5000) {
value1 = analogRead(0); //read from pin0
 Serial.print("Value 1: ");
 Serial.println(value1);
value2 = analogRead(1);
 Serial.print("Value 2: ");
 Serial.println(value2);
//value1 INTO PAYLOAD
 //break down 10-bit reading into two bytes and place in payload
 payload[0] = value1 >> 8 & 0xff; // payload[0] = MSB.
 payload[1] = value1 & 0xff; // 0xff = 1111 1111, i.e. cuts of the MSB from pin5 and all is left is the LSB
//value2 INTO PAYLOAD
 payload[2] = value2 >> 8 & 0xff;
 payload[3] = value2 & 0xff; //LSB
xbee.send(tx_5001);
// flash TX indicator
 flashLed(statusLed, 1, 100);
 }
// after sending a tx request, we expect a status response
 // wait up to 5 seconds for the status response
 if (xbee.readPacket(5000)) {
// got a response!
 // should be a znet tx status
if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
 xbee.getResponse().getZBTxStatusResponse(txStatus);
// get the delivery status, the fifth byte
 if (txStatus.getStatus() == SUCCESS) {
// success. time to celebrate
 flashLed(statusLed, 5, 50);
 } else {
// the remote XBee did not receive our packet. is it powered on?
 flashLed(errorLed, 3, 500);
 }
 }
 } else {
 // local XBee did not provide a timely TX Status Response -- should not happen
 flashLed(errorLed, 2, 50);
 }
delay(2000);
}
void flashLed(int pin, int times, int wait) {
for (int i = 0; i < times; i++) {
 digitalWrite(pin, HIGH);
 delay(wait);
 digitalWrite(pin, LOW);
if (i + 1 < times) {
 delay(wait);
 }
 }
}

