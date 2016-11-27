#include <Servo.h>
#include "IMU.h"

#define MOTORS_COUNT (4)
extern uint32_t motors_speed[MOTORS_COUNT];
extern IMU imu;

#define UPDATE_MOTORS_TIMEOUT (5)
