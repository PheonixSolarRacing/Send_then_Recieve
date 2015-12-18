#include "mcp_can.h"
#include "mcp_can_dfs.h"                                               //The key to including a library inside the Sketch Folder is tohave all the files in the sketch folder, and then #include all the ".h" files
#include "REGS2515.h"
#include <SPI.h>                                                       //needed here in the main sketch for some reason


MCP_CAN CAN0(10);                                      // Set CS to pin 10
long unsigned int rxID;
long unsigned int txID;
unsigned char len = 8;
unsigned char txBuf[8] = {0, 1, 2, 3, 4, 5, 6, 7};
unsigned char rxBuf[8] = {0, 1, 2, 3, 4, 5, 6, 7};
boolean wait = 1;
boolean ouch = 0;
long Fails = 0;
long Sucsesses = 0;
long Time = 0;
long hysteresisTime = 0;
long hysteresis = 3000;
int ouchCounter = 0;
void setup()
{
  Serial.begin(115200);
  pinMode(0,INPUT);
  // init can bus, baudrate: 500k
  delay(5000);
  if(CAN0.begin(CAN_125KBPS) == CAN_OK) Serial.print("can init ok!!\r\n");
  else Serial.print("Can init fail!!\r\n");
  
}


void loop()
{
  wait = 1;
  Serial.print("Sending: ID:420 Data:");
  for(int i = 0; i <= 7;i++){
    txBuf[i] = random(0,255);
    Serial.print(txBuf[i]);
    Serial.print(",");
  }
  
  CAN0.sendMsgBuf(420, 0, 8, txBuf);// send data:  id = 420, standrad flame, data len = 8, txBuf: data buffer
  Time = millis();
  while(wait){
    if(millis() >= (Time + 30)){
      wait = 0;
      Serial.print(" TIMEOUT ");
    }
    if(!digitalRead(0)){ 
      wait = 0;
      readInMessage();
    }
    
    if(!digitalRead(0)){ 
      wait = 0;
      readInMessage();
    }
  }

  Serial.println();
  errorCheck();
  
  //delay(5);
  Serial.print(" ouch:");
  Serial.print(ouchCounter);
  Serial.print(" ");              
}

void readInMessage(){
    Serial.print("      ");
    CAN0.readMsgBuf(&len, rxBuf);
    rxID = CAN0.getCanId();
    Serial.print(" ID: ");
    Serial.print(rxID);
    Serial.print(" Data:");
    for(int i = 0; i<len; i++){
      Serial.print(rxBuf[i]);
      Serial.print(", ");
    }
}

void errorCheck(){
  byte ow = CAN0.getTxErrors();
  byte ou = CAN0.getRxErrors();
  if((ow>0)||(ou>0)){
    ouch=1;
    
    if((millis() + hysteresis) > hysteresisTime){
      hysteresisTime = millis();
      ouchCounter++; 
    }
    
  }
  Serial.print(ow);  //
  Serial.print(",");
  Serial.print(ou);
  Serial.print("   ");
  Serial.print(CAN0.getErrorFlag(RX1OVR));
  Serial.print(CAN0.getErrorFlag(RX0OVR));
  Serial.print(CAN0.getErrorFlag(TXBO));
  Serial.print(CAN0.getErrorFlag(TXEP));
  Serial.print(CAN0.getErrorFlag(RXEP));
  Serial.print(CAN0.getErrorFlag(TXWAR));
  Serial.print(CAN0.getErrorFlag(RXWAR));
  Serial.print(CAN0.getErrorFlag(EWARN));
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
