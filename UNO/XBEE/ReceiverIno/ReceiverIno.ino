#include <XBee.h>
// XBEE setup -------------------------------
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle
Rx16Response rx16 = Rx16Response();
Rx64Response rx64 = Rx64Response();
int pinSpeaker = 10;
// LED indicating XBee status
int statusLed = 4;
int errorLed = 3;
int dataLed = 5;
// XBEE setup -------------------------------
void setup(void)
{
 pinMode(statusLed, OUTPUT);
 pinMode(errorLed, OUTPUT);
 pinMode(dataLed, OUTPUT);
pinMode(pinSpeaker, OUTPUT);
xbee.begin(9600);
 flashLed(statusLed, 3, 50);
}
void loop(void)
{
 xbee.readPacket();
if (xbee.getResponse().isAvailable()) {
 // got something
if (xbee.getResponse().getApiId() == RX_16_RESPONSE || xbee.getResponse().getApiId() == RX_64_RESPONSE) {
 // got a rx packet
if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
 xbee.getResponse().getRx16Response(rx16);
//READ VALUE1
 uint8_t analogHigh = rx16.getData(0);
 uint8_t analogLow = rx16.getData(1);
 int value1 = analogLow + (analogHigh * 256);
//READ VALUE2
 uint8_t analogHigh2 = rx16.getData(2);
 uint8_t analogLow2 = rx16.getData(3);
 int value2 = analogLow2 + (analogHigh2 * 256); 
playTone(value1, value2);
 delay(value1);
flashLed(dataLed, 3, 50);
} else {
 xbee.getResponse().getRx64Response(rx64);
}
flashLed(statusLed, 1, 10);
} else {
 // not something we were expecting
 flashLed(errorLed, 1, 25);
 }
 }
}
// duration in mSecs, frequency in hertz
void playTone(long duration, int freq) {
 duration *= 1000;
 int period = (1.0 / freq) * 1000000;
 long elapsed_time = 0;
 while (elapsed_time < duration) {
 digitalWrite(pinSpeaker,HIGH);
 delayMicroseconds(period / 2);
 digitalWrite(pinSpeaker, LOW);
 delayMicroseconds(period / 2);
 elapsed_time += (period);
 }
}
// flash control LEDs to indicate status of the XBee
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

