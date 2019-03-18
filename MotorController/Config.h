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

#define ENC_STEPS_PER_ROTATION 64*130   // PPR * gearbox ratio        TODO 130?

// ------------------------- Speeds -----------------------------

#define DEFAULT_SPEED 150               // todo units, probably just PWM value (0-255)
#define DEFAULT_ACC 10                  // same here


// ---------------------- Communication -------------------------

#define SERIAL_BAUDRATE 115200
