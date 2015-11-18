/* 
  Web server sketch for IDE v1.0.5 and w5100/w5200
  Originally posted November 2013 by SurferTim
  Last modified 6 June 2015      
*/

/*
  If you are using the w5200 model shield
  from Seeed Studios (Radio Shack),
  you must change the include file names to the following.
/*#include <EthernetV2_0.h>
#include <utility/w5200.h>
#include <utility/socketV2_0.h>*/

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <utility/w5100.h>
#include <utility/socket.h>

// comment out the next line to eliminate the Serial.print stuff
// saves about 1.6K of program memory
#define ServerDEBUG

// this must be unique
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC };

// change to your network settings
IPAddress ip( 192,168,2,2 );
IPAddress gateway( 192,168,2,1 );
IPAddress subnet( 255,255,255,0 );

EthernetServer server(80);

const char legalChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890/.-_~";
unsigned int requestNumber = 0;

unsigned long connectTime[MAX_SOCK_NUM];

int freeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int) __brkval);  
}

void setup()
{
  Serial.begin(115200);

  // disable w5100 SPI while starting SD
  digitalWrite(10,HIGH);

#ifdef ServerDEBUG
  Serial.print(F("Starting SD.."));
#endif

  if(!SD.begin(4)) {
#ifdef ServerDEBUG
    Serial.println(F("failed"));
#endif
  }
  else {
#ifdef ServerDEBUG
    Serial.println(F("ok"));
#endif
  }

  Ethernet.begin(mac, ip, gateway, gateway, subnet);

  delay(2000);
  server.begin();

  unsigned long thisTime = millis();

  for(int i=0;i<MAX_SOCK_NUM;i++) {
    connectTime[i] = thisTime;
  }

#ifdef ServerDEBUG
  Serial.println(F("Ready"));
#endif
}

int loopCount = 0;

void loop()
{
  checkServer();
  myStuff();
}

void myStuff() {
  if(Serial.available()) {
    if(Serial.read() == 'r') ShowSockStatus();    
  }

  checkSockStatus();
}

void checkServer() {

  EthernetClient client = server.available();
  if(client) {
    boolean currentLineIsBlank = true;
    boolean currentLineIsGet = true;
    int tCount = 0;
    char tBuf[65];
    int r,t;
    char *pch;
    char methodBuffer[8];
    char requestBuffer[48];
    char pageBuffer[48];
    char paramBuffer[48];
    char protocolBuffer[9];
    char fileName[32];
    char fileType[4];
    int clientCount = 0;

    requestNumber++;
#ifdef ServerDEBUG
    Serial.print(F("\r\nClient request #"));
    Serial.print(requestNumber);
    Serial.print(F(": "));
#endif

    // this controls the timeout
    int loopCount = 0;

    while (client.connected()) {
      while(client.available()) {
        // if packet, reset loopCount
//        loopCount = 0;
        char c = client.read();

        if(currentLineIsGet && tCount < 63)
        {
          tBuf[tCount] = c;
          tCount++;
          tBuf[tCount] = 0;
        }

        if (c == '\n' && currentLineIsBlank) {
#ifdef ServerDEBUG
          Serial.print(tBuf);
#endif
//          Serial.print(F("POST data: "));
          while(client.available()) client.read();

          int scanCount = sscanf(tBuf,"%7s %47s %8s",methodBuffer,requestBuffer,protocolBuffer);

          if(scanCount != 3) {

#ifdef ServerDEBUG
            Serial.println(F("bad request"));
#endif
            sendBadRequest(client);
            return;
          }

          char* pch = strtok(requestBuffer,"?");
          if(pch != NULL) {
            strncpy(fileName,pch,31);
            strncpy(tBuf,pch,31);

            pch = strtok(NULL,"?");
            if(pch != NULL) {
              strcpy(paramBuffer,pch);
            }            
            else paramBuffer[0] = 0;            
          }

          strtoupper(requestBuffer);
          strtoupper(tBuf);

          for(int x = 0; x < strlen(requestBuffer); x++) {
            if(strchr(legalChars,requestBuffer[x]) == NULL) {
              Serial.println(F("bad character"));  
              sendBadRequest(client);
              return;
            }            
          }

#ifdef ServerDEBUG
          Serial.print(F("file = "));
          Serial.println(requestBuffer);
#endif
          pch = strtok(tBuf,".");

          if(pch != NULL) {
            pch = strtok(NULL,".");

            if(pch != NULL) strncpy(fileType,pch,4);
            else fileType[0] = 0;

#ifdef ServerDEBUG
            Serial.print(F("file type = "));
            Serial.println(fileType);
#endif
          }

#ifdef ServerDEBUG
          Serial.print(F("method = "));
          Serial.println(methodBuffer);
#endif
          if(strcmp(methodBuffer,"GET") != 0 && strcmp(methodBuffer,"HEAD") != 0) {
            sendBadRequest(client);
            return;
          }

#ifdef ServerDEBUG
          Serial.print(F("params = "));
          Serial.println(paramBuffer);

          Serial.print(F("protocol = "));
          Serial.println(protocolBuffer);
#endif          
          // if dynamic page name 
          if(strcmp(requestBuffer,"/MYTEST.PHP") == 0) {
#ifdef ServerDEBUG
            Serial.println(F("dynamic page"));            
#endif
          }
          else {
            if(strcmp(fileName,"/") == 0) {
              strcpy(fileName,"/INDEX.HTM");
              strcpy(fileType,"HTM");

#ifdef ServerDEBUG
              Serial.print(F("Home page "));            
#endif
            }

#ifdef ServerDEBUG
            Serial.println(F("SD file"));            
#endif
            if(strlen(fileName) > 30) {
#ifdef ServerDEBUG
              Serial.println(F("filename too long"));
#endif
              sendBadRequest(client);

              return;
            }
            else if(strlen(fileType) > 3 || strlen(fileType) < 1) {

#ifdef ServerDEBUG
              Serial.println(F("file type invalid size"));
#endif
              sendBadRequest(client);
              return;
            }
            else {
#ifdef ServerDEBUG
              Serial.println(F("filename format ok"));
#endif
              if(SD.exists(fileName)) {
#ifdef ServerDEBUG
                // SRAM check
                Serial.print(F("SRAM = "));
                Serial.println(freeRam());

                Serial.print(F("file found.."));                
#endif


                File myFile = SD.open(fileName);

                if(!myFile) {
#ifdef ServerDEBUG
                  Serial.println(F("open error"));
#endif
                  sendFileNotFound(client);
                  return;
                }
#ifdef ServerDEBUG
                else Serial.print(F("opened.."));
#endif

                strcpy_P(tBuf,PSTR("HTTP/1.0 200 OK\r\nContent-Type: "));

                // send Content-Type
                if(strcmp(fileType,"HTM") == 0) strcat_P(tBuf,PSTR("text/html"));
                else if(strcmp(fileType,"PHP") == 0) strcat_P(tBuf,PSTR("text/html"));
                else if(strcmp(fileType,"TXT") == 0) strcat_P(tBuf,PSTR("text/plain"));
                else if(strcmp(fileType,"CSS") == 0) strcat_P(tBuf,PSTR("text/css"));
                else if(strcmp(fileType,"GIF") == 0) strcat_P(tBuf,PSTR("image/gif"));
                else if(strcmp(fileType,"JPG") == 0) strcat_P(tBuf,PSTR("image/jpeg"));
                else if(strcmp(fileType,"JS") == 0) strcat_P(tBuf,PSTR("application/javascript"));
                else if(strcmp(fileType,"ICO") == 0) strcat_P(tBuf,PSTR("image/x-icon"));
                else if(strcmp(fileType,"PNG") == 0) strcat_P(tBuf,PSTR("image/png"));
                else if(strcmp(fileType,"PDF") == 0) strcat_P(tBuf,PSTR("application/pdf"));
                else if(strcmp(fileType,"ZIP") == 0) strcat_P(tBuf,PSTR("application/zip"));
                else strcat_P(tBuf,PSTR("text/plain"));

                strcat_P(tBuf,PSTR("\r\nConnection: close\r\n\r\n"));
                client.write(tBuf);

                if(strcmp(methodBuffer,"GET") == 0)  {
#ifdef ServerDEBUG
                  Serial.print(F("send.."));
#endif
                  while(myFile.available()) {
                    clientCount = myFile.read(tBuf,64);
                    client.write((byte*)tBuf,clientCount);
                  }
                }
                myFile.close();              
#ifdef ServerDEBUG
                Serial.println(F("closed"));
#endif
                client.stop();                
#ifdef ServerDEBUG
                Serial.println(F("disconnected"));
#endif
                return;
              }
              else {
#ifdef ServerDEBUG
                Serial.println(F("File not found"));
#endif
                sendFileNotFound(client);
                return;
              }

            }
          }

          pch = strtok(paramBuffer,"&");

          while(pch != NULL)
          {
            if(strncmp(pch,"t=",2) == 0)
            {
              t = atoi(pch+2);
#ifdef ServerDEBUG
              Serial.print("t=");
              Serial.println(t,DEC);             
#endif
            }

            if(strncmp(pch,"r=",2) == 0)
            {
              r = atoi(pch+2);
#ifdef ServerDEBUG
              Serial.print("r=");              
              Serial.println(r,DEC);
#endif
            }


            pch = strtok(NULL,"& ");
          }
#ifdef ServerDEBUG
          Serial.println(F("Sending response"));
#endif

          strcpy_P(tBuf,PSTR("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"));
          client.write(tBuf);

          if(strcmp(methodBuffer,"GET") == 0) {

            strcpy_P(tBuf,PSTR("<html><head><script type=\"text/javascript\">"));
            client.write(tBuf);
            strcpy_P(tBuf,PSTR("function show_alert() {alert(\"This is an alert\");}"));
            client.write(tBuf);
            strcpy_P(tBuf,PSTR("</script></head>"));
            client.write(tBuf);

            strcpy_P(tBuf,PSTR("<body><H1>TEST</H1><form method=GET onSubmit=\"show_alert()\">"));
            client.write(tBuf);
            strcpy_P(tBuf,PSTR("T: <input type=text name=t><br>"));
            client.write(tBuf);
            strcpy_P(tBuf,PSTR("R: <input type=text name=r><br><input type=submit></form></body></html>"));
            client.write(tBuf);
          }

          client.stop();
        }
        else if (c == '\n') {
          currentLineIsBlank = true;
          currentLineIsGet = false;
        } 
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }

      loopCount++;

      // if 1000ms has passed since last packet
      if(loopCount > 1000) {
        // close connection
        client.stop();
#ifdef ServerDEBUG
        Serial.println("\r\nTimeout");
#endif
      }

      // delay 1ms for timeout timing
      delay(1);
    }
#ifdef ServerDEBUG
    Serial.println(F("disconnected"));
#endif
  }
}

void sendFileNotFound(EthernetClient thisClient) {
  char tBuf[64];
  strcpy_P(tBuf,PSTR("HTTP/1.0 404 File Not Found\r\n"));
  thisClient.write(tBuf);
  strcpy_P(tBuf,PSTR("Content-Type: text/html\r\nConnection: close\r\n\r\n"));
  thisClient.write(tBuf);
  strcpy_P(tBuf,PSTR("<html><body><H1>FILE NOT FOUND</H1></body></html>"));
  thisClient.write(tBuf);
  thisClient.stop();  
#ifdef ServerDEBUG
  Serial.println(F("disconnected"));
#endif
}

void sendBadRequest(EthernetClient thisClient) {
  char tBuf[64];
  strcpy_P(tBuf,PSTR("HTTP/1.0 400 Bad Request\r\n"));
  thisClient.write(tBuf);
  strcpy_P(tBuf,PSTR("Content-Type: text/html\r\nConnection: close\r\n\r\n"));
  thisClient.write(tBuf);
  strcpy_P(tBuf,PSTR("<html><body><H1>BAD REQUEST</H1></body></html>"));
  thisClient.write(tBuf);
  thisClient.stop();  
#ifdef ServerDEBUG
  Serial.println(F("disconnected"));
#endif
}

void  strtoupper(char* aBuf) {

  for(int x = 0; x<strlen(aBuf);x++) {
    aBuf[x] = toupper(aBuf[x]);
  }
}

byte socketStat[MAX_SOCK_NUM];

void ShowSockStatus()
{
  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    Serial.print(F("Socket#"));
    Serial.print(i);
    uint8_t s = W5100.readSnSR(i);
    socketStat[i] = s;
    Serial.print(F(":0x"));
    Serial.print(s,16);
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
    Serial.println(F(")"));
  }
}

void checkSockStatus()
{
  unsigned long thisTime = millis();

  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    uint8_t s = W5100.readSnSR(i);

    if((s == 0x17) || (s == 0x1C)) {
        if(thisTime - connectTime[i] > 30000UL) {
          Serial.print(F("\r\nSocket frozen: "));
          Serial.println(i);
          close(i);
        }
    }
    else connectTime[i] = thisTime;

    socketStat[i] = W5100.readSnSR(i);
  }
}
