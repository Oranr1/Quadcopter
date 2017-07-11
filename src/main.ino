#include <Servo.h>
#include "IMU.h"
#include "Wire.h"

#include <RF24_config.h>
#include <RF24.h>
#include <printf.h>
#include <nRF24L01.h>
#include <SPI.h>

#include <CNCHandler/CNC_HANDLER_exports.h>
#include <Pid/PID_exports.h>
#include <Utils/UTILS.h>

#define FRONT (2)
#define RIGHT (3)
#define REAR (4)
#define LEFT (5)

#define RIGHT_INDEX (0)
#define REAR_INDEX (1)
#define LEFT_INDEX (2)
#define FRONT_INDEX (3)

extern double powerY;
extern double powerX;
extern double powerZ;

int pins[] = {RIGHT, REAR, LEFT, FRONT};
Servo motors[MOTORS_COUNT];
uint32_t motors_speed[MOTORS_COUNT] = {1000, 1000, 1000, 1000};
IMU imu(0, 0, -7);
unsigned long last_motors_update_time = 0;
unsigned long keep_alive_last_received_time = 0;

void setup()
{
  Serial.begin(115200);
  //Serial.println("Statring");
  for (int i = 0; i < MOTORS_COUNT; i++) {
    motors[i].attach(pins[i]);
    motors[i].writeMicroseconds(motors_speed[i]);
  }
  //Serial.println("Waiting for arm");
  delay(3000);
  last_motors_update_time = millis();

  Wire.begin();
  imu.initialize();

  CNC_HANDLER__init();
  PID__init();
  Serial.println("Waiting..");
}

void main__keep_alive()
{
  keep_alive_last_received_time = millis();
}

void loop()
{
    imu.calculate_angle();
    CNC_HANDLER__handle_cnc();
    PID__compute();
    if (millis() - keep_alive_last_received_time > KEEP_ALIVE_TIMEOUT) {
      RECEIVER__state_g.throttle = 1000;
    }
    update_motors();
}

int map_speed(int speed)
{
  if (RECEIVER__state_g.throttle < 1100) {
    return STOP_MOTOR_SPEED;
  }

  if (speed < STOP_MOTOR_SPEED) {
    speed = STOP_MOTOR_SPEED;
  }
  else if (speed > MAX_MOTOR_SPEED) {
    speed = MAX_MOTOR_SPEED;
  }

  return speed;
}

void update_motors()
{
  if (millis() - last_motors_update_time >= UPDATE_MOTORS_TIMEOUT) {
    motors[FRONT_INDEX].writeMicroseconds(map_speed(
      RECEIVER__state_g.throttle + PID__powers_g.x + PID__powers_g.z));
    motors[REAR_INDEX].writeMicroseconds(map_speed(
      RECEIVER__state_g.throttle - PID__powers_g.x + PID__powers_g.z));
    motors[LEFT_INDEX].writeMicroseconds(map_speed(
      RECEIVER__state_g.throttle + PID__powers_g.y - PID__powers_g.z));
    motors[RIGHT_INDEX].writeMicroseconds(map_speed(
      RECEIVER__state_g.throttle - PID__powers_g.y - PID__powers_g.z));
    last_motors_update_time = millis();
  }
}
