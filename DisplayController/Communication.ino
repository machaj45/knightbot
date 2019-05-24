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
    delay(1000);
  }
}

void updateCom(){
  if(Serial.available() > 1){
    char c = Serial.read();
    if(c == 'L'){
      c = Serial.read();
      int lineNum =  int(c) - 48;
      if(lineNum >= 0 && lineNum < 6){
        String line = Serial.readStringUntil('!');          // '!' is placeholder for terminal testing, will be replaced by '\n'
        if(line.length() > 21){
          line = line.substring(0, 22);
        }
        lines[lineNum] = line;
        update = true;
      }else{
        flushSerial();
      }
    }else if(c == 'P'){
      c = Serial.read();
      digitalWrite(PWR_LED, (c == '0') ? LOW : HIGH);
    }else{
      flushSerial();
    }
  }else if(Serial.available()){
    flushSerial();
  }
}

void flushSerial(){   //discards all serial data
  while(Serial.available()){
    Serial.read();
  }
}
