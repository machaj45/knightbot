#include "Config.h"

void initCom(){
  Serial.setTimeout(5);
  while(false){           // just for debug, to be changed to true!!!
    Serial.print("D");
    delay(200);
    if(Serial.available() && Serial.read() == 'D'){
      flushSerial();
      break;
    }
    delay(500);
  }
}

void updateCom(){
  while(Serial.available()){
    char c = Serial.read();
    if(c == 'S'){
      delay(1);
      c = Serial.read();
      int lineNum =  int(c) - 48;
      if(lineNum == 1){
        getReadyToPush();
      }else if(lineNum == 2){
        getReadyToPop();
      }else{
        //flushSerial();
      }
    }else if(c == 'C'){
      Serial.print('C');
      Serial.print(cansCollected);
    }
  }
}

void flushSerial(){   //discards all serial data
  while(Serial.available()){
    Serial.read();
  }
}
