/*
   Telnet server sketch for IDE v1.0.5+ and w5100/w5200
   Posted 4 January 2015 by SurferTim
   Last edit 6 January 2015 by SurferTim
*/
#include <SPI.h>
#include <Ethernet.h>
#include <utility/w5100.h>
#include <utility/socket.h>

#define port 80

byte socketStat[MAX_SOCK_NUM];
byte connectStatus[MAX_SOCK_NUM];

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC };

// change to your network settings
IPAddress ip( 192,168,0,100 );
IPAddress gateway( 192,168,0,254 );
IPAddress subnet( 255,255,255,0 );

EthernetServer server(port);

void setup()
{
  Serial.begin(115200);

  // disable SD
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);

  Ethernet.begin(mac, ip, gateway, gateway, subnet);
  Serial.println(Ethernet.localIP());

  delay(2000);
  server.begin();
}

void loop() {
  ShowSockStatus();
  delay(5000);  
}
void ShowSockStatus()
{
  byte listening = 0;

  Serial.println();
  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    Serial.print(F("Socket#"));
    Serial.print(i);
    uint8_t s = W5100.readSnSR(i);
    socketStat[i] = s;

    if(s == 0x1C) {
      close(i);
      connectStatus[i] = 0;
    }

    if(s == 0x14) listening = 1;      

    if(s == 0x17 && connectStatus[i] == 0) {
      connectStatus[i] = 1;
// comment out this send to stop initial "Hello"
      send(i,(unsigned char*)"Hello\r\n",7);
    }

    Serial.print(F(":0x"));
    if(s < 16) Serial.print(F("0"));
    Serial.print(s,HEX);
    Serial.print(F(" "));
    Serial.print(W5100.readSnPORT(i));
    Serial.print(F(" D:"));
    uint8_t dip[4];
    W5100.readSnDIPR(i, dip);
    for (int j=0; j<4; j++) {
      Serial.print(dip[j],10);
      if (j<3) Serial.print(".");
    }
    Serial.print(F("("));
    Serial.print(W5100.readSnDPORT(i));
    Serial.print(F(") "));
    Serial.print(F("RX:"));
    unsigned int rxCount = W5100.getRXReceivedSize(i);        

    Serial.print(rxCount);
    Serial.print(F("  "));

    if(rxCount > 32) rxCount = 32;

    if(rxCount > 0) {
      unsigned char rxBuffer[33];

      recv(i,rxBuffer,rxCount);

      rxBuffer[rxCount] = 0;

      Serial.print((char*)rxBuffer);
      send(i,(unsigned char*)"ok ",3);
      send(i,(unsigned char*)rxBuffer,strlen((char*)rxBuffer));

      if(strncmp((char*)rxBuffer,"quit",4) == 0) {
        disconnect(i);

        unsigned long start = millis();

        while(W5100.readSnSR(i) != SnSR::CLOSED && millis() - start < 1000)
           delay(1);

        if(W5100.readSnSR(i) != SnSR::CLOSED) close(i); 
        connectStatus[i] = 0;
      }
    }

    Serial.println();

  }

  if(!listening) {
    Serial.println(F("Not listening"));

    for(int i = 0;i < MAX_SOCK_NUM; i++) {
      if(socketStat[i] == 0) {
        socket(i,SnMR::TCP,port,0);
        listen(i);
        break;      
      }  
    }    
  }
}
