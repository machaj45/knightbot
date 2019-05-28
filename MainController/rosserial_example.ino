/*
 * rosserial Publisher Example
 * Prints "hello world! and arduino time(millis) into topic 'chatter' 
 */

#include <ros.h>
#include <std_msgs/String.h>

ros::NodeHandle  nh;    //creates handle for node

std_msgs::String str_msg;   //Create message to be send. Static alocation for safety.
ros::Publisher chatter("chatter", &str_msg);  //this is probably important too

char hello[20] = "Hello from Arduino!";
char time[20];




void setup()
{
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

  
  delay(1000);
}
