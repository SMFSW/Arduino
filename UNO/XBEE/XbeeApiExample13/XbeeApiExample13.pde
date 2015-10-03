/**
* XBee ACD API Reader Example version 1.3
* by Rob Faludi and Daniel Shiffman and Tom Igoe
* http://www.faludi.com
* http://www.shiffman.net
* http://tigoe.net
* v 1.2  (added multiple samples)
* v 1.3  (added ZNet support for I/O and AT commands)

Updated 3 Dec 2008
*/

import xbee.*;
import processing.serial.*;

// Your Serial Port
Serial port;
// Your XBee Reader object
XBeeReader xbee;

void setup() {
 size(200, 200);
 // println("Available serial ports:");
 // println(Serial.list());
 port = new Serial(this, Serial.list()[0], 9600);
 xbee = new XBeeReader(this,port);

 println("Setting up Xbee");

 // start() issues your initialization commands to the Xbee radio
 // start() is a blocking function (takes ~3 seconds on average) and will time out after 10 seconds
 xbee.startXBee();
}


// This function works just like a "serialEvent()" and is
// called for you when data is available to be read from your XBee radio.
public void xBeeEvent(XBeeReader xbee) {
 // Grab a frame of data
 XBeeDataFrame data = xbee.getXBeeReading();

 // This version of the library only works with IOPackets
 // For ZNet, you would say XBeeDataFrame.ZNET_IOPACKET
 if (data.getApiID() == xbee.SERIES1_IOPACKET) {

   // Get the transmitter address
   int addr = data.getAddress16();

   // Get the RSSI reading in dBM
   int rssi = data.getRSSI();
   int totalSamples = data.getTotalSamples();

   // Get digital and analog data for all Samples
   // If just one Sample per frame, you don't need this loop
   // You can just say getDigital() or getAnalog()
   for (int n = 0; n < totalSamples; n++) {
     print("Sample: " + n + "  ");

     // Current state of each digital channel (-1 indicates channel is not configured)
     int[] digital = data.getDigital(n);
     // Current state of each analog channel (-1 indicates channel is not configured);
     int[] analog = data.getAnalog(n);

     // This example simply prints the data to the message window
     print("16 address: " + addr + " rssi: " + rssi);
     print("  digital: ");
     for (int i = 0; i < digital.length; i++) {
       print(digital[i] + " ");
     }

     print("  analog: ");
     for (int i = 0; i < analog.length; i++) {
       print(analog[i] + " ");
     }
     println();

   }
 }
 else {
   println("Not I/O data: " + data.getApiID());
 }
}

public void draw() {

}
