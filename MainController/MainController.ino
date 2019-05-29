#include <ros.h>
#include <std_msgs/String.h>
#include "Config.h"
#include <SDL_Arduino_INA3221.h>

void printDisplayInfo();



SDL_Arduino_INA3221 ina3221;
ros::NodeHandle  nh;    //creates handle for node

std_msgs::String str_msg;   //Create message to be send. Static alocation for safety.
ros::Publisher chatter("chatter", &str_msg);  //this is probably important too

char hello[20] = "Hello from Arduino!";
char time[20];
void setup() {
  
  nh.initNode();       //init node
  nh.advertise(chatter);   //init topic in node
  
  Serial1.begin(SERIAL1_BAUDRATE);             //motor controller
  Serial2.begin(SERIAL2_BAUDRATE);             //picker
  Serial3.begin(SERIAL3_BAUDRATE);             //display    
  ina3221.begin();
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

  printDisplayInfo();
  delay(1000);
}


void printDisplayInfo(){
   float battVoltage = ina3221.getBusVoltage_V(1);
   float motorCurrent = ina3221.getCurrent_mA(2);
   float PcCurrent = ina3221.getCurrent_mA(3);       
   float systemCurrent = ina3221.getCurrent_mA(1);
   char text[21];
   char foo0[5];
   dtostrf(battVoltage, 5,2, foo)
   char foo1[5];
   dtostrf(motorCurrent, 5,2, foo)   
   char foo2[5];
   dtostrf(PcCurrent, 5,2, foo)
   char foo3[5];
   dtostrf(systemCurrent, 5,2, foo)
   sprintf(text, "%s %s %s %s", foo0, foo1, foo2, foo3);
   Serial3.println(text);






}







