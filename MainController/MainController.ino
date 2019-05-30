#include <ros.h>
#include <std_msgs/String.h>
#include "Config.h"
#include <SDL_Arduino_INA3221.h>
#include "lib.h"

void printDisplayInfo();
void printState();
void clearDisplay();
void waitForInit();


SDL_Arduino_INA3221 ina3221;


ros::NodeHandle  nh;    //creates handle for node

std_msgs::String str_msg;   //Create message to be send. Static alocation for safety.
ros::Publisher chatter("chatter", &str_msg);  //this is probably important too

char hello[20] = "Hello from Arduino!";
char time[20];

void setup() {
  
  nh.initNode();       //init node
  nh.advertise(chatter);   //init topic in node
  MSerial.begin(SERIAL1_BAUDRATE);             //motor controller
  MSerial.setTimeout(5);
  CSerial.begin(SERIAL2_BAUDRATE);             //picker
  CSerial.setTimeout(5);
  //pinMode(16, INPUT);
  //pinMode(17, INPUT);

  DSerial.begin(SERIAL3_BAUDRATE);             //display
  DSerial.setTimeout(5);
  waitForInit();
  
  ina3221.begin();
  clearDisplay();

  //waitForInit();
  nh.initNode();       //init node
  nh.advertise(chatter);   //init topic in node
 
}


void loop(){

  str_msg.data = hello;             //save data into prealocated msg
  chatter.publish( &str_msg );      //publish msg
  nh.spinOnce();                    //no clue

  String foo = String(millis());    //save time to string
  foo.toCharArray(time, 20);        //coppy string into char array
  str_msg.data = time;              //save data into prealocated msg
  chatter.publish( &str_msg );      //publish msg
  nh.spinOnce();

  printDisplayInfo();               //print battery voltage, motor, PC and system current onto display
  printState();

  delay(1000);
}


void waitForInit() {
  bool devOK[] = {false, false, false};
  //clearDisplay();
  DSerial.println("L0   (K)NightBot 0.1");
  DSerial.println("L2  Motors");
  DSerial.println("L3  CanCollector");
  DSerial.println("L4  Display");
  while (true) {          //TODO resend unit letters, e.g. M,C,U,D to acknowledge
    if (!devOK[0]) {
      if(MSerial.read() == 'M'){
        devOK[0] = true;
        MSerial.print('M');
        DSerial.println("L2  Motors          OK");
      }
    }
    if (!devOK[1]) {
      if (CSerial.read() == 'C'){   //must be added U option - cube collector
        devOK[1] = true;
        MSerial.print('C');
        DSerial.println("L3  Collector    OK");
      }
    }
    if (!devOK[2]) {
      if (MSerial.read() == 'D'){   // well, cant print anything on display befor its initialisation
        devOK[2] = true;
        MSerial.print('D');
        DSerial.println("L4  Display         OK");
      }
    }
    if (devOK[0] && devOK[1] && devOK[2]) {
      break;
    }
  }
  while(MSerial.available()){MSerial.read();}
  while(CSerial.available()){CSerial.read();}
  while(DSerial.available()){DSerial.read();}
  delay(500);
  
}

void printDisplayInfo() {
  float battVoltage = ina3221.getBusVoltage_V(1);
  float motorCurrent = ina3221.getCurrent_mA(2);
  float PcCurrent = ina3221.getCurrent_mA(3);
  float systemCurrent = ina3221.getCurrent_mA(1);

  char foo0[5];
  delay(display_render_delay);

  DSerial.print("L1");

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
    delay(display_render_delay);
    DSerial.write('L');
    DSerial.write(i);
    DSerial.print("                     ");
    DSerial.println();
  }
}

void printState() {
  delay(display_render_delay);
  DSerial.print("L3");
  delay(1);
  if (nh.connected()) {
    DSerial.print("ROS connected");
  } else {
    DSerial.print("   Waiting for ROS");
  }
  DSerial.println();

}
