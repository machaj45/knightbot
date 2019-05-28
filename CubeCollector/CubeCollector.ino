
#include "Config.h"
#include <Servo.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>

SoftwareSerial OurSerial(10,11);  // RX,TX

Servo Servos [NUMBER_OF_SEVOS];
int Servos_postion[NUMBER_OF_SEVOS];

const byte inPin2 = 2;
const byte inPin3 = 3;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup() {

  // initialization of servos array for cube sorting 
  for(int i = 0;i<NUMBER_OF_SEVOS;i++){
    Servos[i].attach(i+OFFSET);
  }

  pinMode(inPin2, INPUT_PULLUP);
  pinMode(inPin3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(inPin2), TcsInterrupt, FALLING);  
  attachInterrupt(digitalPinToInterrupt(inPin3), interrupt_tree, CHANGE);  

  Serial.begin(SERIAL_BAUDRATE);
  while(!Serial){
    
  }
  if(DEBUG){
    Serial.println("Serial comunication is now enabled");
  }
  OurSerial.begin(SERIAL_BAUDRATE);
  while(!OurSerial){
    
  }
  if(DEBUG){
    Serial.println("OurSerial comunication is now enabled");
  }
  if (tcs.begin()) {
    if(DEBUG){
      Serial.println("Found sensor");
    }
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
  tcs.write8(TCS34725_PERS, TCS34725_PERS_NONE); 
  tcs.setInterrupt(true);
}

void loop() {
  
  for(int i = 0;i<NUMBER_OF_SEVOS;i++){
    Servos[i].write(Servos_postion[i]);
  }
  
}

void interruptPin2(){

 return 0;
}
void interruptPin3(){
 return 0;
}
void getRawData_noDelay(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c)
{
  *c = tcs.read16(TCS34725_CDATAL);
  *r = tcs.read16(TCS34725_RDATAL);
  *g = tcs.read16(TCS34725_GDATAL);
  *b = tcs.read16(TCS34725_BDATAL);
}
void TcsInterrupt(){
    uint16_t r, g, b, c, colorTemp, lux;
    getRawData_noDelay(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature(r, g, b);
    lux = tcs.calculateLux(r, g, b);
    if(DEBUG){
      Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
      Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
      Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
      Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
      Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
      Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
      Serial.println(" ");
    }
    Serial.flush();

    tcs.clearInterrupt();
}
