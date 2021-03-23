/**
 * Configuration file
 */

//Main loop
#define LOOP_TIME 20   // milliseconds

//Fail safe
#define FS_WS_THR 20  // 1 second = FS_WS_THR*LOOP_TIME

// Serial
#define SERIAL_BAUD 115200 

/*
 * I2C devices
 *  - PWM PCA9685
 *  - IMU MPU 9250
 */

#define PWM_CONTROLLER_ADDRESS 0x40
//#define PWM_CONTROLLER_TYPE    PCA9685
#define PWM_CONTROLLER_TYPE    ESP32PWM

#define IMU_ADDRESS            0x68
#define IMU_TYPE               MPU9250

// Robot config
#define LEG_NUM 4

// Size of leg parts in mm
#define LEG_SIZE_L1   0.0
#define LEG_SIZE_L2  51.9
#define LEG_SIZE_L3  50.0

// default angles for init state
#define LEG_ANGLE_ALPHA M_PI_2
#define LEG_ANGLE_BETA  M_PI_2
#define LEG_ANGLE_GAMMA M_PI_2

// angle limits
#define LEG_ANGLE_ALPHA_MIN 0
#define LEG_ANGLE_ALPHA_MAX M_PI

#define LEG_ANGLE_BETA_MIN  0
#define LEG_ANGLE_BETA_MAX  M_PI

#define LEG_ANGLE_GAMMA_MIN 0
#define LEG_ANGLE_GAMMA_MAX M_PI

#define LEG_TRIM_INC   0.002   // radian
#define LEG_TRIM_LIMIT  0.24   // See settingsUint8ToDouble()

// Servo config (set as max as possible for init, but use servoProfile)
#define SERVO_MIN 600
#define SERVO_MAX 2400
#define SERVO_FREQ 330

//Fail safe
#define FS_WS_THR 20  // 1 second = FS_WS_THR*LOOP_TIME
