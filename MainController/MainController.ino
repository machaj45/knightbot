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
  Serial1.begin(SERIAL1_BAUDRATE);             //motor controller
  Serial1.setTimeout(5);
  Serial2.begin(SERIAL2_BAUDRATE);             //picker
  Serial2.setTimeout(5);
  //pinMode(16, INPUT);
  //pinMode(17, INPUT);

  Serial3.begin(SERIAL3_BAUDRATE);             //display
  Serial3.setTimeout(5);
  ina3221.begin();
  clearDisplay();

  //waitForInit();
  nh.initNode();       //init node
  nh.advertise(chatter);   //init topic in node
 
}


void loop()
{


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
  clearDisplay();
  DSerial.println("L0   (K)NightBot 0.1!");
  delay(display_render_delay);
  printDisplayInfo();
  while (true) {
    if (!devOK[0]) {
      if (MSerial.read() == 'M')
        devOK[0] = true;
      DSerial.println("L2      Motors OK      !");
      delay(display_render_delay);
    }
    if (!devOK[1]) {
      if (MSerial.read() == 'C')
        devOK[1] = true;
      DSerial.println("L3   Can colector OK   !");
      delay(display_render_delay);
    }
    if (!devOK[2]) {
      if (MSerial.read() == 'D')
        devOK[2] = true;
      DSerial.println("L4      Display OK     !");
      delay(display_render_delay);
    }
    if (devOK[0] && devOK[1] && devOK[2]) {
      break;
    }
  }
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
  DSerial.println('!');
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
    DSerial.println('!');
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
  DSerial.print('!');

}
