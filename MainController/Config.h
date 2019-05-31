//
//       Main controller (Arduino Mega) configuration file
//



// ------------------------- Pinout -----------------------------

#define LED1 23
#define LED2 22
#define LED3 12
#define LED4 13

#OPTO0 A0
#OPTO1 A1
#OPTO2 A2
#OPTO3 A3
#OPTO4 A4
#OPTO5 A5
#OPTO6 A6
#OPTO7 A7


// ---------------------- Communication -------------------------


#define SERIAL1_BAUDRATE 115200		//motor controller
#define SERIAL2_BAUDRATE 9600		//collector
#define SERIAL3_BAUDRATE 9600		//display
#define DSerial Serial3
#define MSerial Serial1
#define CSerial Serial2
#define display_render_delay 0
#define canCallSign 'C'
#define displayCallSign 'D'
#define motorCallSIgn 'M'
