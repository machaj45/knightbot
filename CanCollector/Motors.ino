
#include "Config.h"

// 0-rotGear, 1-elevator, 2-sun
boolean isMoving[] = {false, false, false};
boolean movDir[] = {false, false, false};

long sunMovStart = 0;
long elevMovStart = 0;
long pushElevStart = 0;

void updateMotors(){
  if(isMoving[0] && ((movDir[0] && endstops[0]) || (!movDir[0] && endstops[1]))){
    analogWrite(ROT_GEAR_MOT_PIN1, 0);
    analogWrite(ROT_GEAR_MOT_PIN2, 0);
    isMoving[0] = false;
  }
//  if(isMoving[1] && ((movDir[1] && endstops[2]) || (!movDir[1] && endstops[3]))){
//    digitalWrite(ELEVATOR_MOT_PIN1, LOW);
//    digitalWrite(ELEVATOR_MOT_PIN2, LOW);
//    isMoving[1] = false;
//  }
  if(isMoving[2] && (millis()-sunMovStart > 500) && endstops[3]){
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

void rotateRotGear(boolean dir){
  if((dir && !endstops[0])||(!dir && !endstops[1])){
    movDir[0] = dir;
    isMoving[0] = true;
    analogWrite(ROT_GEAR_MOT_PIN1, dir?ROT_GEAR_PWM:0);
    analogWrite(ROT_GEAR_MOT_PIN2, dir?0:ROT_GEAR_PWM);
  }
}

void rotateElevator(boolean dir){
  //if((dir && !endstops[2])||(!dir && !endstops[3])){
  //TODO timing start run
    movDir[1] = dir;
    isMoving[1] = true;
    digitalWrite(ELEVATOR_MOT_PIN1, dir?HIGH:LOW);
    digitalWrite(ELEVATOR_MOT_PIN2, dir?LOW:HIGH);
  //}
}

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
