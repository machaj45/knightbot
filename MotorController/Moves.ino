#include "Config.h"

#define STOP 0
#define FLOAT 1
#define FORWARD 2
#define FORWARD_DIST 3
#define TURN_CONTRA 4
#define TURN_WHEEL 5

int actualLSpeed = 0;
int actualRSpeed = 0;
int calculatedLSpeed = 0;
int calculatedRSpeed = 0;
int targetSpeed = DEFAULT_SPEED;  // mm/s
int targetAcc = DEFAULT_ACC;    // mm/s*s

long targetRTacho = 0;
long targetLTacho = 0;

int moveState = STOP;

boolean isAccelerating = true;

long lastRTachoSpeed = 0, lastLTachoSpeed = 0;

long lastUpdate = 0;
int timeDelta = 0;


void initController(){
  lastUpdate = micros();
  lastRTachoSpeed = rightMotorTacho;
  lastLTachoSpeed = leftMotorTacho;
}

void updateMovement(){
  timeDelta = micros() - lastUpdate;
  lastUpdate = micros();
  int rDiff = rightMotorTacho - lastRTachoSpeed;
  int lDiff = leftMotorTacho - lastLTachoSpeed;
  lastRTachoSpeed = rightMotorTacho;
  lastLTachoSpeed = leftMotorTacho;
  actualLSpeed = count2mm(lDiff)/(float)((float)timeDelta/1000000);
  actualRSpeed = count2mm(rDiff)/(float)((float)timeDelta/1000000);

  // TODO speed regulator, speed calculations for each case
  switch(moveState){
    case STOP:

      break;
    case FLOAT:

      break;
    case FORWARD:

      break;
    case FORWARD_DIST:

      break;
    case TURN_CONTRA:
    
      break;
    case TURN_WHEEL:

      break;
  
  }
  // TODO callback to com for next command when completed
  
}

void enableMotors(boolean enable){
  digitalWrite(R_MOTOR_ENABLE, enable ? HIGH:LOW);
  digitalWrite(L_MOTOR_ENABLE, enable ? HIGH:LOW);
}

void floatMotors(){
  enableMotors(false);
}

void forward(boolean dir){
  forward(dir, 99999999);
}
// TODO remove dir
void forward(boolean dir, long dist){
  enableMotors(true);
  isAccelerating = true;
  moveState = FORWARD;
  targetRTacho = rightMotorTacho + mm2count(dist);
  targetLTacho = leftMotorTacho + mm2count(dist);
}

void stopMotors(){
  stopMotors(false);
}

void stopMotors(boolean immediate){
  enableMotors(true);
  isAccelerating = false;
  moveState = STOP;
  if(immediate){
    calculatedRSpeed = 0;
    calculatedLSpeed = 0;  
  }
}

void setMotorSpeed(int ts){
  targetSpeed = ts;
}

void setAcceleration(int ta){
  targetAcc = ta;
}

void turnContra(float angle){
  enableMotors(true);
  isAccelerating = true;
  moveState = TURN_CONTRA;
  int dist = 0; // --------------------------------------   TODO calculate arc length
  targetRTacho = rightMotorTacho + ((angle > 0 ? 1 : -1)*mm2count(dist));
  targetLTacho = leftMotorTacho + ((angle < 0 ? 1 : -1)*mm2count(dist));
  
}

void turnOneWheel(float angle, boolean dir){
  enableMotors(true);
  isAccelerating = true;
  moveState = TURN_WHEEL;
  int dist = 0; // --------------------------------------   TODO calculate arc length, not the same as above !
  dist *= dir ? 1 : -1;
  if(angle > 0 ){
    targetRTacho = rightMotorTacho + mm2count(dist);
  }else{
    targetLTacho = leftMotorTacho + mm2count(dist);
  }
}

void turn(float angle, float radius, boolean toRight){
  // not used now
}
