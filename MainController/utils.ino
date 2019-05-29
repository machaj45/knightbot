





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
   delay(display_render_delay);
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
