//
//              MotorController configuration file
//
// here you can set all possible things about motors and movement
// note: If you want to change motor direction, just flip its pins

// ------------------------- Pinout -----------------------------

#define R_MOTOR_ENC_A 3
#define R_MOTOR_ENC_B 2
#define L_MOTOR_ENC_A 8
#define L_MOTOR_ENC_B 9

#define R_MOTOR_ENABLE 4
#define L_MOTOR_ENABLE 7

#define R_MOTOR_A 5                     // must be PWM pin!
#define R_MOTOR_B 6                     // must be PWM pin!
#define L_MOTOR_A 10                    // must be PWM pin!
#define L_MOTOR_B 11                    // must be PWM pin!

#define LED0 12
#define LED1 13

//------------------------ Dimensions ---------------------------

#define WHEEL_DIAMETER 80               // in mm
#define WHEEL_DISTANCE 240              // in mm

#define ENC_STEPS_PER_ROTATION 64*70   // PPR * gearbox ratio

// ------------------------- Speeds -----------------------------

#define DEFAULT_SPEED 200               // in mm/s
#define DEFAULT_ACC 100                 // in mm/s*s


// ---------------------- Communication -------------------------

#define SERIAL_BAUDRATE 115200

// commands:
#define STOP 1
#define FLOAT 2
#define FORWARD 3
#define FORWARD_DIST 4
#define TURN_CONTRA 5
#define TURN_WHEEL_F 6
#define TURN_WHEEL_B 7
#define SET_SPEED 8
#define SET_ACC 9
