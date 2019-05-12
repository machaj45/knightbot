
#include "Config.h"



void updateMotors(){
  
}

void rotateSun(boolean dir){
  if((dir && !endstops[0])||(!dir && !endstops[1])){
    analogWrite(SUN_MOT_PIN1, dir?SUN_GEAR_PWM:0)
    analogWrite(SUN_MOT_PIN1, dir?0:SUN_GEAR_PWM)
  }
}

void rotateRotGear(boolean dir){
  
}

void rotateElevator(boolean dir){
  
}

//time based move
void pushElevator(){
  
}
