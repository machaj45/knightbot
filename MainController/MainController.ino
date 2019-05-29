#include <ros.h>
#include <std_msgs/String.h>
#include "Config.h"
#include <SDL_Arduino_INA3221.h>

void printDisplayInfo();
void printState();
void clearDisplay();



SDL_Arduino_INA3221 ina3221;


ros::NodeHandle  nh;    //creates handle for node

std_msgs::String str_msg;   //Create message to be send. Static alocation for safety.
ros::Publisher chatter("chatter", &str_msg);  //this is probably important too

char hello[20] = "Hello from Arduino!";
char time[20];
void setup() {

  nh.initNode();       //init node
  nh.advertise(chatter);   //init topic in node
  //Serial1.begin(SERIAL1_BAUDRATE);             //motor controller
  //Serial2.begin(SERIAL2_BAUDRATE);             //picker
  pinMode(16, INPUT);
  pinMode(17, INPUT);

  Serial3.begin(SERIAL3_BAUDRATE);             //display
  ina3221.begin();
  clearDisplay();
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









void printDisplayInfo(){
   float battVoltage = ina3221.getBusVoltage_V(1);
   float motorCurrent = ina3221.getCurrent_mA(2);
   float PcCurrent = ina3221.getCurrent_mA(3);       
   float systemCurrent = ina3221.getCurrent_mA(1);
   
   char foo0[5];
   delay(display_render_delay);
  
   DSerial.print("L0");

   dtostrf(battVoltage, 4,2, foo0);
   DSerial.print(foo0);
   DSerial.print("V ");
   dtostrf(motorCurrent, 3,0, foo0);
   DSerial.print(foo0);
   DSerial.print("/");
   dtostrf(PcCurrent, 3,0, foo0);
   DSerial.print(foo0);  
   DSerial.print("/"); 
   dtostrf(systemCurrent, 3,0, foo0);
   DSerial.print(foo0);
   DSerial.print(" mA");
   DSerial.println('!');
   delay(display_render_delay+500);
   if(PcCurrent>50){
    DSerial.println("P1");
   }else{
    DSerial.println("P0");
   }
   

}

void clearDisplay() {
  delay(display_render_delay);
  DSerial.print("L0                     ");
  DSerial.println('!');
    delay(display_render_delay);
  DSerial.print("L1                     ");
  DSerial.println('!');
    delay(display_render_delay);
  DSerial.print("L2                     ");
  DSerial.println('!');
    delay(display_render_delay);
  DSerial.print("L3                     ");
  DSerial.println('!');
  DSerial.print("L4                     ");
  DSerial.println('!');
    delay(display_render_delay);
  DSerial.print("L5                     ");
  DSerial.println('!');
    delay(display_render_delay);

}

void printState() {
  delay(display_render_delay);
  DSerial.print("L1");
  delay(1);
  if (nh.connected()) {
    DSerial.print("ROS connected");
  } else {
    DSerial.print("   Waiting for ROS");
  }
  DSerial.print('!');

}
