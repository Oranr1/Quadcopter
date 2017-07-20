#ifndef UTILS_H
#define UTILS_H

#include <Servo.h>
#include "IMU.h"

#define MOTORS_COUNT (4)
extern uint32_t motors_speed[MOTORS_COUNT];
extern IMU imu;

#define UPDATE_MOTORS_TIMEOUT (5)
#define STOP_MOTOR_SPEED (1000)
#define MAX_MOTOR_SPEED (2000)

#define KEEP_ALIVE_TIMEOUT (500)

void main__keep_alive(void);

#endif
