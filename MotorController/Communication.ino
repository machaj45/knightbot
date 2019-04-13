#include "Config.h"

// Receiving commands at this format:
//    "num of command" "command argument""end of line"
//
//commands:   (zero is reserved for parseInt() error)
// all from Moves.ino:
// 1 - STOP (0/1)   // bool immediate
// 2 - FLOAT
// 3 - FORWARD (0/1)   // bool dir
// 4 - FORWARD_DIST (signed dist)
// 5 - TURN_CONTRA (signed angle)     angle in degrees!
// 6 - TURN_WHEEL_F (signed angle)    angle in degrees!
// 7 - TURN_WHEEL_B (signed angle)    angle in degrees!
//returns 1 if successfully added to queue, 0 otherwise
// plus these:
//#define GET_POS 10   //returns two numbers: "xpos" "ypos"
//

#define QUEUE_SIZE 20
byte commands[QUEUE_SIZE];
int params[QUEUE_SIZE];
byte queueEnd = 0;
byte topCommand = 0;
int topParam = 0;

void initCom(){
  for(int i = 0; i < QUEUE_SIZE;i++){
    commands[i] = 0;
    params[i] = 0;
  }
  Serial.setTimeout(10);
  while(true){
    Serial.print("M");
    delay(200);
    if(Serial.available() && Serial.read() == "M"){
      while(Serial.available()){
        Serial.read();
      }
      break;
    }
    delay(1000);
  }
}


// TODO add command queue ?
// TODO set job to motorcontroller when last completed


void updateCom(){
  //fetch data from serial to queue if ready
  if(Serial.available()){
    String data = Serial.readStringUntil('n');//todo switchcase throughfall
    int idx = data.indexOf(' ');
    
    byte command = data.substring(idx).toInt();
    int param = data.substring(idx).toInt();        //TODO
    
  }

  // update move if last done
  //TODO
}

//returns 0 if queue full
byte pushQueue(byte command, int param){
  if(queueEnd  >= QUEUE_SIZE){return 0;}
  commands[queueEnd] = command;
  params[queueEnd] = param;
  queueEnd++;
  return 1;
}

void popQueue(){
  topCommand = commands[0];
  topParam = params[0];
  for(int i = 1; i < queueEnd; i++){
    commands[i-1] = commands[i];
    params[i-1] = params[i];
  }
}


