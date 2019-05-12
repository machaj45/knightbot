//
//                 CanCollector configuration file
//
// here you can set all possible things about collecting mechanism for cans


// ------------------------- Pinout -----------------------------

#define SUN_ENC_PIN 0
#define SUN_MOT_PIN1 0
#define SUN_MOT_PIN2 0
#define ROT_GEAR_START_PIN 0
#define ROT_GEAR_END_PIN 0
#define ROT_GEAR_MOT_PIN1 0
#define ROT_GEAR_MOT_PIN2 0
#define ELEVATOR_START_PIN 0
#define ELEVATOR_END_PIN 0
#define ELEVATOR_MOT_PIN1 0
#define ELEVATOR_MOT_PIN2 0
#define MAG_SERVO_PIN 0
#define CAN_CHECK_PIN 0

// ------------------------ Tresholds ---------------------------

#define SUN_GEAR_PWM 255
#define ROT_GEAR_PWM 255
#define ELEVATOR_PWM 255

#define PWM_INCREMENT 1

#define MAG_SERVO_GRAB_DEG 0
#define MAG_SERVO_RELEASE_DEG 255

#define SUN_ENC_TRES 512

// ---------------------- Communication -------------------------

#define SERIAL_BAUDRATE 115200
