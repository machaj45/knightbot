
#include "Config.h"
#include <SDL_Arduino_INA3221.h>
#include "Adafruit_VL53L0X.h"

void printDisplayInfo();
void clearDisplay();
void waitForInit();
void initStuff();
int retrieveCanCount();
void bb();
void dischargeCans();
void moveEngines(int command, int parameter);
void forward();
void turnLeft();
void turnRight();
int readLine();

SDL_Arduino_INA3221 ina3221;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measure;

void setup() {

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  MSerial.begin(SERIAL1_BAUDRATE);             //motor controller
  MSerial.setTimeout(5);
  CSerial.begin(SERIAL2_BAUDRATE);             //picker
  CSerial.setTimeout(5);
  DSerial.begin(SERIAL3_BAUDRATE);             //display
  DSerial.setTimeout(1);
  Serial.begin(9600);
  Serial.setTimeout(5);
  pinMode(OPTO0, INPUT);
  pinMode(OPTO1, INPUT);
  pinMode(OPTO2, INPUT);
  pinMode(OPTO3, INPUT);
  pinMode(OPTO4, INPUT);
  pinMode(OPTO5, INPUT);
  pinMode(OPTO6, INPUT);
  pinMode(OPTO7, INPUT);
  while (MSerial.peek() == 'M') {
    MSerial.read();
  }
  while (CSerial.peek() == 'C') {
    CSerial.read();
  }

  lox.begin();

  ina3221.begin();
  clearDisplay();
  digitalWrite(LED1, HIGH);
  delay(1000);
  CSerial.print("S1");
  digitalWrite(LED1 , LOW);


  MSerial.println("9 80");
  digitalWrite(LED2, HIGH);
  while (DSerial.read() != 'B') {}
  digitalWrite(LED2, LOW);
  moveEngines(4, 500);
  delay(500);
  moveEngines(5, -90);
  delay(500);
  moveEngines(4, 500);
  delay(500);
  moveEngines(5, -90);
  delay(500);
  moveEngines(4, 500);
  delay(500);
  moveEngines(5, 90);
  delay(500);
  moveEngines(4, 500);
  delay(500);
  moveEngines(5, 90);
  delay(500);
  moveEngines(4, 750);
  delay(500);
  int i = 0;
  while(i<=5){
    i++;
    moveEngines(4, 740);
    delay(500);
    moveEngines(4, -740);
    delay(500);
    moveEngines(5, 90);
    delay(500);
    moveEngines(4, 200);
    delay(500);
    moveEngines(5, -90);
    delay(500);
  }
}


void loop() {


}


int readLine() {
  int num = 0;
  num += digitalRead(OPTO0);
  num += digitalRead(OPTO1);
  num += digitalRead(OPTO2);
  num += digitalRead(OPTO3);
  num += digitalRead(OPTO4);
  num += digitalRead(OPTO5);
  num += digitalRead(OPTO6);
  num += digitalRead(OPTO7);
  DSerial.print("L3 OPTO ");
  DSerial.println(num);
  Serial.println(num);
  return num;

}

void moveEngines(int command, int parameter) {
  MSerial.print(command);
  MSerial.print(" ");
  MSerial.print(parameter);
  MSerial.print(" ");
  //MSerial.println("4 100");
  //delay(50);
  DSerial.print("L2 ");
  DSerial.print(command);
  DSerial.print(" ");
  DSerial.println(parameter);
  //DSerial.print("4 100 ");
  int s = MSerial.read();
  DSerial.print(s);
  DSerial.print(" | ");
  DSerial.println((char)s);
}

void forward() {
  while (checkObstacle()) {}
  Serial.println("Forward!");
  while (MSerial.available()) {
    MSerial.read();
  }
  DSerial.println("L1Forward");
  moveEngines(4, 200);
  delay(200);
  delay(2000);
}

bool checkObstacle() {
  /*bool enemy = false;
  MSerial.println("5 30");
  delay(500);
  MSerial.println("5 -60");
  for (int i = 0; i < 3000; i++) {
    if (distance() < 300) {
      enemy = true;
    }
    delay(1);
  }
  MSerial.println("5 30");
  delay(500);

  return enemy;*/
  if (distance<400){
    return true;
  }
  return false;
  
}


void turnLeft() {
  Serial.println("Turn left!");
  while (MSerial.available()) {
    MSerial.read();
  }
  DSerial.println("L1Left");
  moveEngines(5, 90);
  delay(2000);
}

void turnRight() {
  Serial.println("Turn right!");
  DSerial.println("L1Right");
  while (MSerial.available()) {
    MSerial.read();
  }
  moveEngines(5, -90);
  delay(500);
  while (MSerial.read() != 'R') {
    Serial.println("Waiting for R");
  }
}

int distance() {
  VL53L0X_RangingMeasurementData_t measure;
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  int dist = 9999;
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    dist = measure.RangeMilliMeter;
    Serial.print("Distance (mm): "); Serial.println(dist);
    DSerial.print("L4 Distance ");
    DSerial.println(dist);
  } else {
    Serial.println(" out of range ");
    return 9999;
    DSerial.println("L4 Distance out of range");
  }

  return dist;
}


void bb() {
  delay(display_render_delay);

  if (DSerial.peek() == 'B') {
    DSerial.read();
    switch (DSerial.read()) {
      case '0':   //flush engines buffer
        MSerial.println("1 1");
        DSerial.println("L5B OOOO NECO NECO NECO");
        break;
      case '1':
        DSerial.println("L5B STOP OOOO NECO NECO");
        break;
      case '2':
        DSerial.println("L5B STOP NECO OOOO NECO");
        break;
      case '3':
        DSerial.println("L5B STOP NECO NECO OOOO");
        break;
    }
  } else {
    DSerial.println("L5B STOP NECO NECO NECO");
  }



}


int retrieveCanCount() {
  while (CSerial.available()) {
    CSerial.read();
  }
  CSerial.println("C");
  delay(10);
  if (CSerial.read() == 'C') {
    return CSerial.readString().toInt();
  } else {
    return -1;
  }
}

void dischargeCans() {
  static int canCount = retrieveCanCount();
  CSerial.print("S2");
  delay(500);
  CSerial.print("S2");
  delay(500);
  while (canCount > 0) {
    digitalWrite(LED2, HIGH);
    digitalWrite(LED1, LOW);
    int newCan = retrieveCanCount();
    printDisplayInfo();               //print battery voltage, motor, PC and system current onto display
    DSerial.print("L4Discharging cans ");
    DSerial.println(canCount);
    DSerial.println("L2");

    if (newCan != canCount) {
      canCount = newCan;
      MSerial.println("4 -80");
      DSerial.println("L2 4 -80 back up");
    }
  }
  digitalWrite(LED2, LOW);
}

void printDisplayInfo() {
  float battVoltage = ina3221.getBusVoltage_V(1);
  float motorCurrent = ina3221.getCurrent_mA(2);
  float PcCurrent = ina3221.getCurrent_mA(3);
  float systemCurrent = ina3221.getCurrent_mA(1);

  char foo0[5];
  delay(display_render_delay);
  DSerial.print("");
  DSerial.print("L0");

  dtostrf(battVoltage, 4, 2, foo0);
  DSerial.print(foo0);
  DSerial.print("V ");
  dtostrf(motorCurrent, 3, 0, foo0);
  DSerial.print(foo0);
  DSerial.print("/");
  dtostrf(PcCurrent, 3, 0, foo0);
  DSerial.print(foo0);
  DSerial.print("/");
  dtostrf(systemCurrent, 3, 0, foo0);
  DSerial.print(foo0);
  DSerial.print(" mA");
  DSerial.println();
  delay(display_render_delay + 500);
  if (PcCurrent > 50) {
    DSerial.println("P1");
  } else {
    DSerial.println("P0");
  }


}

void clearDisplay() {
  for (int i = 0; i < 6; i++) {
    delay(200);
    DSerial.print('L');
    DSerial.println(i);
  }
}
