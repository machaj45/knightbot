#include "Config.h"

// Receiving commands at this format:
//    "num of command" "command parameter"
// note: neither command nor parameter can be 0!
//
//commands:   (zero is reserved for parseInt() error)
// all from Config.h:
// 1 - STOP (-1/1)   // bool immediate (flushes the queue and forces stop)
// 2 - FLOAT (-1/1)  // bool immediate (flushes the queue and forces float)
// 3 - FORWARD (-1/1)   // bool dir
// 4 - FORWARD_DIST (signed dist)     watch out maximum of +- 32 767 mm!
// 5 - TURN_CONTRA (signed angle)     angle in degrees!
// 6 - TURN_WHEEL_F (signed angle)    angle in degrees!
// 7 - TURN_WHEEL_B (signed angle)    angle in degrees!
// 8 - SET_SPEED (speed)              mm/s
// 9 - SET_ACC (acceleration)         mm/s*s
//returns 1 if successfully added to queue, 0 otherwise
// plus these:
#define GET_POS 20   //returns three numbers: "xpos" "ypos" "theta"
#define QUEUE_LEN 21   //returns number of commands waiting in queue
//todo get tacho count ?

#define QUEUE_SIZE 20
byte commands[QUEUE_SIZE];
int params[QUEUE_SIZE];
byte queueEnd = 0;
byte topCommand = 0;
int topParam = 0;

boolean fetchNextCommand = true;

void initCom(){
  //this initialisation is probably useless, can be removed
  for(int i = 0; i < QUEUE_SIZE;i++){
    commands[i] = 0;
    params[i] = 0;
  }
  Serial.setTimeout(1);
  while(false){           // just for debug, to be changed to true!!!
    Serial.print("M");
    delay(200);
    if(Serial.available() && Serial.read() == 'M'){
      flushSerial();
      break;
    }
    delay(1000);
  }
}

void updateCom(){
  
  //fetch data from serial to queue if ready
  if(Serial.available()){
    byte command = Serial.parseInt();
    if(command == 0){                   //zero means parseInt() timeout
      flushSerial();
      Serial.print(0);
    }else{
      int param = Serial.parseInt();    // maybe use parseFloat() for bigger distances?
      if(param == 0){
        flushSerial();
        Serial.print(0);
      }else if(isInfo(command)){
        flushSerial();
        //Serial.print(1);
        
        // TODO execute command
        //hotfix no.2
        if(command == QUEUE_LEN){
          Serial.println(queueEnd);
        }
        
      }else{
        if(isImmediate(command) && param > 0){
          Serial.println("kvaaaaak");
          flushQueue();
          nextCommand();
        }
        byte err = pushQueue(command, param);
        flushSerial();
        Serial.print(err);    // if queue full, err = 0, else 1
      }
    }
  }

  // update move if last done
  if(fetchNextCommand && queueEnd > 0){
    fetchNextCommand = false;
    popQueue();
    switch(topCommand){
      case STOP:
          stopMotors(); // param dont care
        break;
      case FLOAT:
          floatMotors(); // param dont care
        break;
      case FORWARD:
          forward(topParam > 0 ? true:false);
        break;
      case FORWARD_DIST:
          forward((long)topParam);
        break;
      case TURN_CONTRA:
          turnContra(topParam);
        break;
      case TURN_WHEEL_F:
          turnOneWheel(topParam, true);
        break;
      case TURN_WHEEL_B:
          turnOneWheel(topParam, false);
        break;
      case SET_SPEED:
          setMotorSpeed(topParam);
        break;
      case SET_ACC:
          setAcceleration(topParam);
        break;
      default:
          // --------------------------- maybe report error ?
          fetchNextCommand = true;
        break;
    }
  }
}

boolean isImmediate(byte command){
  return (command == STOP || command == FLOAT) ? true:false;
}

boolean isInfo(byte command){
  return (command == GET_POS || command == QUEUE_LEN) ? true:false;
}

void nextCommand(){
  fetchNextCommand = true;
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
  if(queueEnd == 0){return;}
  for(int i = 1; i < queueEnd; i++){
    commands[i-1] = commands[i];
    params[i-1] = params[i];
  }
  queueEnd--;
}

void flushQueue(){
  queueEnd = 0;
}

void flushSerial(){   //discards all serial data
  while(Serial.available()){
    Serial.read();
  }
}
