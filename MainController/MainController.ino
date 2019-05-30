#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int16MultiArray.h>
#include "Config.h"
#include <SDL_Arduino_INA3221.h>
#include "lib.h"

void printDisplayInfo();
void printState();
void clearDisplay();
void waitForInit();
void initStuff();
int retrieveCanCount();


SDL_Arduino_INA3221 ina3221;


ros::NodeHandle  nh;    //creates handle for node

//----------------Chatter message----------------//
std_msgs::String str_msg;   //Create message to be send. Static alocation for safety.
ros::Publisher chatter("chatter", &str_msg);  //this is probably important too

//----------------CanCount message---------------//
std_msgs::Int16 cans;   //Create message to be send. Static alocation for safety.
ros::Publisher CanCount("CanCount", &cans);  //this is probably important too

//----------------Motor controller msg----------//
void enginesCb( const std_msgs::Int16MultiArray& engine_msg){
   int command = engine_msg.data[0];
   int parameter = engine_msg.data[1];
   MSerial.print(command);
   MSerial.print(" ");
   MSerial.println(parameter);
   
   DSerial.print("L2 ");
   DSerial.print(command);
   DSerial.print(" ");
   DSerial.println(parameter);
   } 
ros::Subscriber<std_msgs::Int16MultiArray> motorSubscriber("motor_control", &enginesCb );
                         
/*void servo_cb(td_msgs::UInt16& cmd_msg){
     servo.write(cmd_msg.data); //set servo angle, should be from 0-180  
     digitalWrite(13, HIGH-digitalRead(13));  //toggle led  
}                        */
   
   
//ros::Subscriber<std_msgs::UInt16> sub("servo", servo_cb);


                         
char hello[20] = "Hello from Arduino!";
char time[20];

void setup() {
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  MSerial.begin(SERIAL1_BAUDRATE);             //motor controller
  MSerial.setTimeout(5);
  //CSerial.begin(SERIAL2_BAUDRATE);             //picker
  //CSerial.setTimeout(5);
  DSerial.begin(SERIAL3_BAUDRATE);             //display
  DSerial.setTimeout(5);  
  
  pinMode(16, INPUT);
  pinMode(17, INPUT);


  //waitForInit();  
  MSerial.print('M');
  MSerial.print('C');
  MSerial.print('D'); 
  while(MSerial.peek()=='M'){MSerial.read();}
  while(CSerial.peek()=='C'){CSerial.read();}
  while(DSerial.peek()=='D'){DSerial.read();}  
  
  
  
  ina3221.begin();
  clearDisplay();
  
  
  
  
   //init ROS
  
  nh.initNode();       //init node
  nh.advertise(chatter);   //init topic in node
  nh.advertise(CanCount);
  nh.subscribe(motorSubscriber);
 // nh.subscribe(sub);
 
}


void loop(){
  initStuff();
  str_msg.data = hello;             //save data into prealocated msg
  chatter.publish( &str_msg );      //publish msg

  String foo = String(millis());    //save time to string
  foo.toCharArray(time, 20);        //coppy string into char array
  str_msg.data = time;              //save data into prealocated msg
  chatter.publish( &str_msg );      //publish msg
 
  cans.data=retrieveCanCount();
  CanCount.publish( &cans );
  DSerial.print("L4Cans in storage ");
  DSerial.println(cans.data);
 
 
 
  nh.spinOnce();
  printDisplayInfo();               //print battery voltage, motor, PC and system current onto display
  printState();
  digitalWrite(LED1, HIGH);
  delay(500);
  digitalWrite(LED1, LOW);
  delay(500);
}

int retrieveCanCount(){
    CSerial.println("C");
    delay(10);
    if(CSerial.read()=='C'){
      return CSerial.readString().toInt();
    } else {
      return -1;
    }
}

void initStuff(){
 /* if(MSerial.peek()=='M'){
    MSerial.write('M');
    MSerial.read();  
  }
  if(DSerial.peek()=='D'){
    DSerial.write('D');
    DSerial.read();  
  }
  if(CSerial.peek()=='C'){
    CSerial.write('C');
    CSerial.read();  
  }   */
  DSerial.write('D');
  MSerial.write('M');
  
}

void waitForInit() {
  
  
  MSerial.print('M');
  CSerial.print('C');
  DSerial.print('D'); 
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
        DSerial.println("L2  Motors          OK");
      }
    }
    if (!devOK[1]) {
      if (CSerial.read() == 'C'){   //must be added U option - cube collector
        devOK[1] = true;
        DSerial.println("L3  Collector    OK");
      }
    }
    if (!devOK[2]) {
      if (MSerial.read() == 'D'){   // well, cant print anything on display befor its initialisation
        devOK[2] = true;
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
    delay(display_render_delay);
    DSerial.print('L');
    DSerial.println(i);
  }
}

void printState() {
  delay(display_render_delay);
  DSerial.println("L0   (K)NightBot 0.1");
  DSerial.print("L3");
  delay(1);
  if (nh.connected()) {
    DSerial.print("ROS connected ");
  } else {
    DSerial.print("Waiting for ROS ");
  }
  DSerial.print(millis());
  DSerial.println();

}
