//
//              MotorController configuration file
//
// here you can set all possible things about motors and movement


// ------------------------- Pinout -----------------------------

#define R_MOTOR_ENC_A 2
#define R_MOTOR_ENC_B 3
#define L_MOTOR_ENC_A 8
#define L_MOTOR_ENC_B 9

#define R_MOTOR_A 5                     // must be PWM pin!
#define R_MOTOR_B 6                     // must be PWM pin!
#define L_MOTOR_A 10                    // must be PWM pin!
#define L_MOTOR_B 11                    // must be PWM pin!


//------------------------ Dimensions ---------------------------

#define WHEEL_DIAMETER 80               // in mm
#define WHEEL_DISTANCE 240              // in mm

#define ENC_STEPS_PER_ROTATION 64*70   // PPR * gearbox ratio

// ------------------------- Speeds -----------------------------

#define DEFAULT_SPEED 200               // in mm/s
#define DEFAULT_ACC 100                 // in mm/s*s


// ---------------------- Communication -------------------------

#define SERIAL_BAUDRATE 115200
