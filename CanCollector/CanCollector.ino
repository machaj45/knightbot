
#include "Config.h"
#include <Servo.h>

#define PICK 0
#define DEPLOY 1
#define MOVING 2

Servo magServo;
boolean releaseCanAction = false;
long canTime = 0;

boolean motorsInited = false;

// mode of collector:
//false: ready to collect cans
//true: ready to deploy cans
boolean mode = false;

boolean operatingCan = false;

byte cansCollected = 0;

boolean readyToPush = false;
boolean readyToPop = false;

//motor endstops:
//rot-gear start, rot-gear end, elevator top, elevator bot, sun encoder
boolean endstops[] = {false, false, false, false,false};
boolean canPresent = false;

void setup() {
  
  Serial.begin(SERIAL_BAUDRATE);
  pinMode(ROT_GEAR_START_PIN, INPUT_PULLUP);
  pinMode(ROT_GEAR_END_PIN, INPUT_PULLUP);
  pinMode(ELEVATOR_ENDSTOP_PIN, INPUT_PULLUP);
  pinMode(ELEVATOR_END_PIN, INPUT_PULLUP);
  pinMode(ELEVATOR_END_GND_PIN, OUTPUT);
  digitalWrite(ELEVATOR_END_GND_PIN, LOW);
  pinMode(CAN_CHECK_PIN, INPUT);
  magServo.attach(MAG_SERVO_PIN);
  pinMode(SUN_MOT_PIN1, OUTPUT);
  pinMode(SUN_MOT_PIN2, OUTPUT);
  pinMode(ROT_GEAR_MOT_PIN1, OUTPUT);
  pinMode(ROT_GEAR_MOT_PIN2, OUTPUT);
  pinMode(ELEVATOR_MOT_PIN1, OUTPUT);
  pinMode(ELEVATOR_MOT_PIN2, OUTPUT);
  delay(10);  //mandatory for correct function of millis()
  magServo.write(MAG_SERVO_GRAB_DEG);

  //pushElevator();
  //rotateSun(false);
  //rotateRotGear(true);
  //rotateSun(true);
  //releaseCan();
  //rotateElevator(false);
  //motorsInited = true;
}

void loop() {

  if(!motorsInited){
    initMotors();
  }else{
    updateStateMachine();
  }
  
  updateServo();
  //updateCom();
  updateSensors();
  updateMotors();
}

void updateSensors(){
  endstops[0] = digitalRead(ROT_GEAR_START_PIN) == LOW;
  endstops[1] = digitalRead(ROT_GEAR_END_PIN) == LOW;
  endstops[2] = digitalRead(ELEVATOR_ENDSTOP_PIN) == LOW;
  endstops[3] = digitalRead(ELEVATOR_END_PIN) == LOW;
  endstops[4] = analogRead(SUN_ENC_PIN) > SUN_ENC_TRES;
  canPresent = digitalRead(CAN_CHECK_PIN) == LOW;
}

void getReadyToPush(){
  mode = false;// must be changed only when not operating can
}

void getReadyToPop(){
  mode = true;
}

void pushCan(){
  
}

void popCan(){
  
}


void releaseCan(){
  releaseCanAction = true;
}

//async magnet handling
void updateServo(){
  if(releaseCanAction){
    if(canTime == 0){
      magServo.write(MAG_SERVO_RELEASE_DEG);
      canTime = millis();
    }
    if(millis() - canTime > 500){
      magServo.write(MAG_SERVO_GRAB_DEG);
      canTime = 0;
      releaseCanAction = false;
    }
  }
}

void printEndstops(){
  Serial.print(endstops[0]);
  Serial.print("  ");
  Serial.print(endstops[1]);
  Serial.print("  ");
  Serial.print(endstops[2]);
  Serial.print("  ");
  Serial.print(endstops[3]);
  Serial.print("  ");
  Serial.print(endstops[4]);
  Serial.print("  ");
  Serial.println(canPresent);
}
