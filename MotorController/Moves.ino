#include "Config.h"

#define BUFFER_SIZE 3     // at least 1
#define POS_ERROR 5       // final destination error in mm
#define KP 8
#define KD 2

int actualLAvgSpeed = 0;
int actualRAvgSpeed = 0;
float calculatedSpeed = 0;
float calculatedLSpeed = 0;
float calculatedRSpeed = 0;
int targetSpeed = DEFAULT_SPEED;  // mm/s
int targetAcc = DEFAULT_ACC;    // mm/s*s
int rSpeeds[BUFFER_SIZE];
int lSpeeds[BUFFER_SIZE];

long targetRTacho = 0;
long targetLTacho = 0;

int moveState = STOP;

boolean isAccelerating = false;

long lastRTachoSpeed = 0, lastLTachoSpeed = 0;

int lastRErr = 0, lastLErr = 0;
int rDir = 1, lDir = 1;

long lastUpdate = 0;
long timeDelta = 0;

//hotfix variable
long hotfixTime = 0;

void initController(){
  for(int i = 0; i < BUFFER_SIZE;i++){
    rSpeeds[i] = 0;
    lSpeeds[i] = 0;
  }
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
  int actualLSpeed = count2mm(lDiff)/(float)((float)timeDelta/1000000);     // not sure if mm are enough for small regulator step
  int actualRSpeed = count2mm(rDiff)/(float)((float)timeDelta/1000000);
  pushSpeeds(actualRSpeed, actualLSpeed);
  avgSpeeds(); // now you have average moving speed

  //check breaking distance for decelerating
  int actAvgSpeed = max(abs(actualRAvgSpeed), abs(actualLAvgSpeed));
  long brakeDist = mm2count(((long)actAvgSpeed*(long)actAvgSpeed)/(long)(2*targetAcc));   //in counts
  if((abs(targetRTacho - rightMotorTacho) <= brakeDist)&&(abs(targetLTacho - leftMotorTacho) <= brakeDist)){isAccelerating = false;}

  //update calculated velocity
  calculatedSpeed += (float)targetAcc * (float)((float)timeDelta/1000000) * (isAccelerating ? 1 : -1);
  if(calculatedSpeed > targetSpeed){calculatedSpeed = targetSpeed;}
  if(calculatedSpeed < 0){calculatedSpeed = 0;}

  if(abs(targetRTacho - rightMotorTacho) >= 100){
    rDir = (targetRTacho - rightMotorTacho) >= 0 ? 1 : -1;
  }
  if(abs(targetLTacho - leftMotorTacho) >= 100){
    lDir = (targetLTacho - leftMotorTacho) >= 0 ? 1 : -1;
  }

  calculatedRSpeed = calculatedSpeed * rDir;
  calculatedLSpeed = calculatedSpeed * lDir;  
  
  switch(moveState){
    case STOP:
        
      break;
    case FLOAT:

      break;
    case FORWARD:
    case FORWARD_DIST:
        
      break;
    case TURN_CONTRA:
        
      break;
    case TURN_WHEEL_F:
        calculatedRSpeed = abs(targetRTacho - rightMotorTacho) < 100 ? 0 : calculatedRSpeed;
        calculatedLSpeed = abs(targetLTacho - leftMotorTacho) < 100 ? 0 : calculatedLSpeed;
      break;
  }
  
  //PD regulator

  int rErr = calculatedRSpeed - actualRAvgSpeed;
  int lErr = calculatedLSpeed - actualLAvgSpeed;
  int rPWM = KP*rErr + KD*(rErr - lastRErr);
  int lPWM = KP*lErr + KD*(lErr - lastLErr);
  
  updateMotors(rPWM, lPWM);
//  Serial.print(calculatedRSpeed);
//  Serial.print("  ");
//  Serial.print(actualRAvgSpeed);
//  Serial.print("  ");
//  Serial.print(rPWM);
//  Serial.print("  ");
//  Serial.println();

  // hotfix to stucking
  if(lastRErr != rErr || lastLErr != lErr){
    hotfixTime = millis();
  }
  if(millis() - hotfixTime > 2000){     // if both left and right wheel errors stay same for 2 sec, take next command
    nextCommand();
  }
  //  end of hotfix
  
  lastRErr = rErr;
  lastLErr = lErr;

//  Serial.print(targetRTacho - rightMotorTacho);
//  Serial.print("  ");
//  Serial.println(targetLTacho - leftMotorTacho);
  // TODO callback to com for next command when completed,              NOT DONE YET
  if(actAvgSpeed < 3 && (abs(targetRTacho - rightMotorTacho) <= 500)&&(abs(targetLTacho - leftMotorTacho) <= 500)){
    nextCommand();
    Serial.print('R');
  }
}

void enableMotors(boolean enable){
  hotfixTime = millis();  //        hotfix
  digitalWrite(R_MOTOR_ENABLE, enable ? HIGH:LOW);
  digitalWrite(L_MOTOR_ENABLE, enable ? HIGH:LOW);
}

void floatMotors(){
  enableMotors(false);
  moveState = FLOAT;
}

void forward(boolean dir){  //true is forward
  forward(999999*(dir ? 1 : -1));
}

void forward(long dist){
  enableMotors(true);
  isAccelerating = true;
  moveState = FORWARD;
  targetRTacho = rightMotorTacho + mm2count(dist);
  targetLTacho = leftMotorTacho + mm2count(dist);
}

void stopMotors(){    //TODO set target tacho as braking distance !!! -------------------------------------------------------------------------------------------------------------------------
  enableMotors(true);
  isAccelerating = false;
  
  
  moveState = STOP;
}

void setMotorSpeed(int ts){
  targetSpeed = ts;
  nextCommand();
}

void setAcceleration(int ta){
  targetAcc = ta;
  nextCommand();
}

void turnContra(int angle){
  enableMotors(true);
  isAccelerating = true;
  moveState = TURN_CONTRA;
  long dist = abs((float)WHEEL_DISTANCE * (float)angle * 3.141592 / (long)360);
  targetRTacho = rightMotorTacho + ((angle > 0 ? 1 : -1)*mm2count(dist));
  targetLTacho = leftMotorTacho + ((angle < 0 ? 1 : -1)*mm2count(dist));
  
}

void turnOneWheel(int angle, boolean dir){  //true is forward
  enableMotors(true);
  isAccelerating = true;
  moveState = TURN_WHEEL_F; //  same update as for TURN_WHEEL_B
  long dist = abs((float)WHEEL_DISTANCE * (float)angle * 3.141592 / 180);
  dist *= dir ? 1 : -1;
  if(angle > 0 ){
    targetRTacho = rightMotorTacho + mm2count(dist);
    targetLTacho = leftMotorTacho;
  }else{
    targetRTacho = rightMotorTacho;
    targetLTacho = leftMotorTacho + mm2count(dist);
  }
}

void turn(float angle, float radius){   //  negative radius means backwards
  // not used now
}

void pushSpeeds(int actRSpeed, int actLSpeed){
  for(int i = 1; i < BUFFER_SIZE;i++){
    rSpeeds[i-1] = rSpeeds[i];
    lSpeeds[i-1] = lSpeeds[i];
  }
  rSpeeds[BUFFER_SIZE-1] = actRSpeed;
  lSpeeds[BUFFER_SIZE-1] = actLSpeed;
}

void avgSpeeds(){
  int sumR = 0;
  int sumL = 0;
  for(int i = 0; i < BUFFER_SIZE;i++){
    sumR += rSpeeds[i];
    sumL += lSpeeds[i];
  }
  actualRAvgSpeed = sumR/BUFFER_SIZE;
  actualLAvgSpeed = sumL/BUFFER_SIZE;
}

void updateMotors(int RPWM,int LPWM){  // range in -255 to 255
  RPWM = constrain(RPWM, -255, 255);
  LPWM = constrain(LPWM, -255, 255);
  if(RPWM >= 0){
    analogWrite(R_MOTOR_A, RPWM);
    analogWrite(R_MOTOR_B, 0);
  }else{
    analogWrite(R_MOTOR_A, 0);
    analogWrite(R_MOTOR_B, RPWM*-1);
  }
  if(LPWM >= 0){
    analogWrite(L_MOTOR_A, LPWM);
    analogWrite(L_MOTOR_B, 0);
  }else{
    analogWrite(L_MOTOR_A, 0);
    analogWrite(L_MOTOR_B, LPWM*-1);
  }
}

