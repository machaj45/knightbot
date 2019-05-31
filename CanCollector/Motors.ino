
#include "Config.h"

#define ROTGEAR 0
#define ELEV 1
#define SUN 2

// 0-rotGear, 1-elevator, 2-sun
boolean isMoving[] = {false, false, false};
boolean movDir[] = {false, false, false};

long sunMovStart = 0;
long elevMovStart = 0;
long pushElevStart = 0;
long sleepTime = 0;

int initState = 0;

int state = 0;
int substate = 0;

void initMotors(){
  switch(initState){
    case 0:
        rotateElevator(false);
        initState++;
      break;
    case 1:
        if(!isMoving[ELEV]){
          initState++;
        }
      break;
    case 2:
        rotateRotGear(false);
        rotateSun(false);
        initState++;
      break;
    case 3:
        if(!isMoving[ROTGEAR] && !isMoving[SUN]){
          initState++;
        }
      break;
    case 4:
        rotateElevator(true);
        initState++;
      break;
    case 5:
        if(!isMoving[ELEV]){
          initState++;
        }
      break;
    case 6:
        motorsInited = true;
        state = 2;
        substate = 6; //set to last !!!
      break;
  }
}

void updateStateMachine(){
   switch(state){
      case PICK:
          switch(substate){
              case 0:
                  Serial.print('P');
                  pushElevator();
                  substate++;
                break;
              case 1:
                  if(pushElevStart == 0){
                    substate++;
                  }
                break;
              case 2:
                  rotateElevator(false);
                  substate++;
                break;
              case 3:
                  if(!isMoving[ELEV]){
                    substate++;
                  }
                break;
              case 4:
                  Serial.print('T');
                  rotateRotGear(true);
                  rotateSun(true);
                  substate++;
                break;
              case 5:
                  if(!isMoving[ROTGEAR] && !isMoving[SUN]){
                    substate++;
                  }
                break;
              case 6:
                  releaseCan();
                  cansCollected++;
                  substate++;
                  sleepTime = millis();
                break;
              case 7:
                  if(millis() - sleepTime >= 1000){
                    state = MOVING;
                    substate = 0;
                  }
                break;
            }
        break;
      case DEPLOY:
          switch(substate){
              case 0:
                  pushElevator();
                  substate++;
                break;
              case 1:
                  if(pushElevStart == 0){
                    substate++;
                  }
                break;
              case 2:
                  rotateElevator(false);
                  substate++;
                break;
              case 3:
                  if(!isMoving[ELEV]){
                    substate++;
                  }
                break;
              case 4:
                  rotateRotGear(false);
                  rotateSun(false);
                  substate++;
                break;
              case 5:
                  if(!isMoving[ROTGEAR] && !isMoving[SUN]){
                    substate++;
                  }
                break;
              case 6:
                  rotateElevator(true);
                  substate++;
                break;
              case 7:
                  if(!isMoving[ELEV]){
                    substate++;
                  }
                break;
              case 8:
                  releaseCan();
                  cansCollected--;
                  state = MOVING;
                  substate = 0;
                break;
            }
        break;
      case MOVING:
          if(!mode){ //e.g. ready for push
            switch(substate){
              case 0:
                  rotateElevator(false);
                  substate++;
                break;
              case 1:
                  if(!isMoving[ELEV]){
                    substate++;
                  }
                break;
              case 2:
                  rotateRotGear(false);
                  substate++;
                break;
              case 3:
                  if(!isMoving[ROTGEAR]){
                    substate++;
                  }
                break;
              case 4:
                  rotateElevator(true);
                  substate++;
                break;
              case 5:
                  if(!isMoving[ELEV]){
                    substate++;
                  }
                break;
              case 6:
                  operatingCan = false;
                  if(mode != modeRequest){
                    mode = modeRequest;
                    substate = 0;
                  }
                  if(!mode && canPresent && cansCollected <= 8){
                    state = PICK;
                    substate = 0;
                    operatingCan = true;
                  }
                break;
            }
          }else{
            switch(substate){
              case 0:
                  rotateElevator(false);
                  substate++;
                break;
              case 1:
                  if(!isMoving[ELEV]){
                    substate++;
                  }
                break;
              case 2:
                  rotateRotGear(true);
                  substate++;
                break;
              case 3:
                  if(!isMoving[ROTGEAR]){
                    substate++;
                  }
                break;
              case 4:
                  if(mode != modeRequest){
                    mode = modeRequest;
                    substate = 0;
                  }
                  if(mode && cansCollected > 0){
                    state = DEPLOY;
                    substate = 0;
                  }
                break;
            }
          }
        break;
    }
}


void updateMotors(){
  if(isMoving[0] && ((movDir[0] && endstops[0]) || (!movDir[0] && endstops[1]))){
    analogWrite(ROT_GEAR_MOT_PIN1, 0);
    analogWrite(ROT_GEAR_MOT_PIN2, 0);
    isMoving[0] = false;
  }
  if(isMoving[1] && ((movDir[1] && endstops[2]) || (!movDir[1] && endstops[3]))){
    digitalWrite(ELEVATOR_MOT_PIN1, LOW);
    digitalWrite(ELEVATOR_MOT_PIN2, LOW);
    isMoving[1] = false;
  }
  if(isMoving[2] && (millis()-sunMovStart > 500) && endstops[4]){
    analogWrite(SUN_MOT_PIN1, 0);
    analogWrite(SUN_MOT_PIN2, 0);
    isMoving[2] = false;
  }
  
  if((pushElevStart != 0) && (millis()-pushElevStart >= ELEV_PUSH_TIME)){
    pushElevStart = 0;
    digitalWrite(ELEVATOR_MOT_PIN1, LOW);
    digitalWrite(ELEVATOR_MOT_PIN2, LOW);
  }
}

//dir - true = clockwise
void rotateRotGear(boolean dir){
  if((dir && !endstops[0])||(!dir && !endstops[1])){
    movDir[0] = dir;
    isMoving[0] = true;
    analogWrite(ROT_GEAR_MOT_PIN1, dir?ROT_GEAR_PWM:0);
    analogWrite(ROT_GEAR_MOT_PIN2, dir?0:ROT_GEAR_PWM);
  }
}

//dir - true = down
void rotateElevator(boolean dir){
  if(!isMoving[1]){
    movDir[1] = dir;
    isMoving[1] = true;
    elevMovStart = millis();
    digitalWrite(ELEVATOR_MOT_PIN1, dir?HIGH:LOW);
    digitalWrite(ELEVATOR_MOT_PIN2, dir?LOW:HIGH);
  }
}

//dir - true = clockwise
void rotateSun(boolean dir){
  movDir[2] = dir;
  isMoving[2] = true;
  sunMovStart = millis();
  analogWrite(SUN_MOT_PIN1, dir?SUN_GEAR_PWM:0);
  analogWrite(SUN_MOT_PIN2, dir?0:SUN_GEAR_PWM);
}

//time based move
void pushElevator(){
  if(!isMoving[1]){
    pushElevStart = millis();
    digitalWrite(ELEVATOR_MOT_PIN1, ELEV_PUSH_DIR?HIGH:LOW);
    digitalWrite(ELEVATOR_MOT_PIN2, ELEV_PUSH_DIR?LOW:HIGH);
  }
}
