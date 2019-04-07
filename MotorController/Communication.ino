
#include "Config.h"


void initCom(){
  while(true){
    Serial.print("M");
    delay(200);
    if(Serial.available() && Serial.read() == "M"){
      break;
    }
    delay(1000);
  }
}


// TODO define move commands
// TODO set job to motorcontroller when last completed

void updateCom(){
  
}
