
#include "Config.h"
#include "PinChangeInt-master/PinChangeInt.h"

volatile long rightMotorTacho = 0;
volatile long leftMotorTacho = 0;

boolean lastRA = false, lastRB = false, lastLA = false, lastLB = false;

float xEst = 0, yEst = 0, fiEst = 0;
long lastRTachoPos = 0, lastLTachoPos = 0;

void setup() {

  Serial.begin(SERIAL_BAUDRATE);

  pinMode(R_MOTOR_ENC_A, INPUT_PULLUP);
  pinMode(R_MOTOR_ENC_B, INPUT_PULLUP);
  pinMode(L_MOTOR_ENC_A, INPUT_PULLUP);
  pinMode(L_MOTOR_ENC_B, INPUT_PULLUP);

  attachPinChangeInterrupt(R_MOTOR_ENC_A, isrRA, CHANGE);
  attachPinChangeInterrupt(R_MOTOR_ENC_B, isrRB, CHANGE);
  attachPinChangeInterrupt(L_MOTOR_ENC_A, isrLA, CHANGE);
  attachPinChangeInterrupt(L_MOTOR_ENC_B, isrLB, CHANGE);

  delay(1000);

  initCom();
  initController();
}

void loop() {

  //updatePosEst();
  //updateMovement();
  //updateCom();

  delay(10);    // for bigger distances between regulator steps

}


void isrRA() {
  lastRA = digitalRead(R_MOTOR_ENC_A) == HIGH;
  rightMotorTacho += (lastRA != lastRB) ? 1 : -1;
}

void isrRB() {
  lastRB = digitalRead(R_MOTOR_ENC_B) == HIGH;
  rightMotorTacho += (lastRA == lastRB) ? 1 : -1;
}

void isrLA() {
  lastLA = digitalRead(L_MOTOR_ENC_A) == HIGH;
  leftMotorTacho += (lastLA != lastLB) ? 1 : -1;
}

void isrLB() {
  lastLB = digitalRead(L_MOTOR_ENC_B) == HIGH;
  leftMotorTacho += (lastLA == lastLB) ? 1 : -1;
}

void updatePosEst() {
  int rDiff = rightMotorTacho - lastRTachoPos;
  int lDiff = leftMotorTacho - lastLTachoPos;
  lastRTachoPos = rightMotorTacho;
  lastLTachoPos = leftMotorTacho;
  
  //todo

}

float count2mm(long count){
  return (count/ENC_STEPS_PER_ROTATION)*WHEEL_DIAMETER*PI;
}

long mm2count(int mm){
  return (mm/(WHEEL_DIAMETER*PI))*ENC_STEPS_PER_ROTATION;
}
