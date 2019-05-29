//
//                 CanCollector configuration file
//
// here you can set all possible things about collecting mechanism for cans


// ------------------------- Pinout -----------------------------

#define SUN_ENC_PIN A0
#define SUN_MOT_PIN1 3		//PWM
#define SUN_MOT_PIN2 5		//PWM
#define ROT_GEAR_START_PIN 12
#define ROT_GEAR_END_PIN 13
#define ROT_GEAR_MOT_PIN1 6	//PWM
#define ROT_GEAR_MOT_PIN2 11	//PWM
#define ELEVATOR_ENDSTOP_PIN 10
#define ELEVATOR_MOT_PIN1 8	//not PWM
#define ELEVATOR_MOT_PIN2 9	//not PWM
#define MAG_SERVO_PIN 7
#define CAN_CHECK_PIN A1
#define LED1 A2
#define LED2 A3

// ------------------------ Tresholds ---------------------------

#define SUN_GEAR_PWM 255
#define ROT_GEAR_PWM 190

#define PWM_INCREMENT 1

#define MAG_SERVO_GRAB_DEG 120
#define MAG_SERVO_RELEASE_DEG 140

#define ELEV_PUSH_TIME 300            // in ms
#define ELEV_PUSH_DIR true

#define SUN_ENC_TRES 970

// ---------------------- Communication -------------------------

#define SERIAL_BAUDRATE 9600
