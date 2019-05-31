#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/Empty.h>
#include "Config.h"
#include <SDL_Arduino_INA3221.h>
#include "lib.h"

void printDisplayInfo();
void printState();
void clearDisplay();
void waitForInit();
void initStuff();
int retrieveCanCount();
void bb();
void dischargeCans();


SDL_Arduino_INA3221 ina3221;


ros::NodeHandle  nh;    //creates handle for node

//----------------Chatter message----------------//
std_msgs::String str_msg;   //Create message to be send. Static alocation for safety.
ros::Publisher chatter("chatter", &str_msg);  //this is probably important too

//----------------CanCount message---------------//
std_msgs::Int16 cans;   //Create message to be send. Static alocation for safety.
ros::Publisher CanCount("CanCount", &cans);  //this is probably important too

//----------------Motor controller msg----------//
void enginesCb(std_msgs::Int16MultiArray& engine_msg){
   int command = engine_msg.data[0];
   int parameter = engine_msg.data[1];
   delay(100);
   MSerial.print(command);
   MSerial.print(" ");
   MSerial.println(parameter);
   //delay(50);
   DSerial.print("L2 ");
   DSerial.print(command);
   DSerial.print(" ");
   DSerial.println(parameter);
   } 
ros::Subscriber<std_msgs::Int16MultiArray> motorSubscriber("/master_node/main_loader/motor_control", &enginesCb );

//----------Start can unloading process--------//
void unloadCb(std_msgs::Empty& empty_msg){
   
   
   
   
   } 
ros::Subscriber<std_msgs::Empty> unloadSubscriber("/master_node/main_loader/unload_cans", &unloadCb );
                         
char hello[20] = "Hello from Arduino!";
char time[20];

void setup() {
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  MSerial.begin(SERIAL1_BAUDRATE);             //motor controller
  MSerial.setTimeout(5);
  CSerial.begin(SERIAL2_BAUDRATE);             //picker
  CSerial.setTimeout(5);
  DSerial.begin(SERIAL3_BAUDRATE);             //display
  DSerial.setTimeout(1);  
  
  //pinMode(16, INPUT);
  //pinMode(17, INPUT);


  //waitForInit();  
  //MSerial.print('M');
  //MSerial.print('C');
  //MSerial.print('D'); 
  while(MSerial.peek()=='M'){MSerial.read();}
  while(CSerial.peek()=='C'){CSerial.read();}
  //while(DSerial.peek()=='D'){DSerial.read();}  
  
  
  
  ina3221.begin();
  //clearDisplay();
  digitalWrite(LED2, HIGH);
  MSerial.println("4 100");
  delay(1000);
  CSerial.print("S1");
  digitalWrite(LED2, LOW);
  
  
   //init ROS
  
  nh.initNode();       //init node
  nh.advertise(chatter);   //init topic in node
  nh.advertise(CanCount);
  nh.subscribe(motorSubscriber);
  nh.subscribe(unloadSubscriber);
  
 
}
void moveEngines(int command, int parameter){
   MSerial.print(command);
   MSerial.print(" ");
   MSerial.println(parameter);
   MSerial.println("4 100");
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

void loop(){
  //initStuff();
  /*str_msg.data = hello;           //save data into prealocated msg
  chatter.publish( &str_msg );      //publish msg

  String foo = String(millis());    //save time to string
  foo.toCharArray(time, 20);        //coppy string into char array
  str_msg.data = time;              //save data into prealocated msg
  chatter.publish( &str_msg );      //publish msg              */
 
  cans.data=retrieveCanCount();
  CanCount.publish( &cans );
  DSerial.print("L4Cans in storage ");
  DSerial.println(retrieveCanCount());
  DSerial.println("L2");
 
 
  //clearDisplay();
  //DSerial.print("DDDDDDDDDDDDDDDDDDDDDDDDDDDd");
  printDisplayInfo();               //print battery voltage, motor, PC and system current onto display
  printState();                     //print ROS connection state and arduino time
  //bb();     
  digitalWrite(LED1, !digitalRead(LED1));  // toggle state
  //DSerial.println("L5 STOP NECO NECO NECO");
  nh.spinOnce();  
  
  if(retrieveCanCount()==8){
    dischargeCans();
  }
  
  /*delay(5000);
  moveEngines(4, 50);
  delay(5000);
  moveEngines(4, -50);   */
}

void bb(){
  delay(display_render_delay);
  
  if(DSerial.peek()=='B'){
    DSerial.read();
    switch(DSerial.read()){
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


int retrieveCanCount(){
    while(CSerial.available()){CSerial.read();}
    CSerial.println("C");
    delay(10);
    if(CSerial.read()=='C'){
      return CSerial.readString().toInt();
    } else {
      return -1;
    }
}

void dischargeCans(){
   static int canCount=retrieveCanCount();
   CSerial.print("S2");
   while(canCount>0){
     digitalWrite(LED2, HIGH); 
     digitalWrite(LED1, LOW);  
     int newCan=retrieveCanCount();
      printDisplayInfo();               //print battery voltage, motor, PC and system current onto display
      printState();                     //print ROS connection state and arduino time   
      DSerial.print("L4Discharging cans ");
      DSerial.println(canCount);
      DSerial.println("L2");
      
     if(newCan!=canCount){
       canCount=newCan;
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
    delay(200);
    DSerial.print('L');
    DSerial.println(i);
  }
}

void printState() {
  delay(display_render_delay);
  DSerial.println("L0   (K)NightBot 0.1");
  DSerial.print("L3");
  delay(display_render_delay);
  delay(1);
  if (nh.connected()) {
    DSerial.print("ROS connected ");
  } else {
    DSerial.print("ROS waiting ");
  }
  DSerial.print((int)(millis()/1000));
  DSerial.println();

}
