#include <Servo.h>
#include "IMU.h"
#include "Wire.h"

#include <RF24_config.h>
#include <RF24.h>
#include <printf.h>
#include <nRF24L01.h>
#include <SPI.h>

#include <CNCHandler/CNC_HANDLER_exports.h>
#include <Utils/UTILS.h>

#define FRONT (2)
#define RIGHT (3)
#define REAR (4)
#define LEFT (5)

int pins[] = {RIGHT, REAR, LEFT, FRONT};
Servo motors[MOTORS_COUNT];
uint32_t motors_speed[MOTORS_COUNT] = {1000, 1000, 1000, 1000};
IMU imu(0, 0, -7);
unsigned long last_motors_update_time = 0;

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

  initialize_cncs();
  Serial.println("Waiting..");
}

void loop()
{
    imu.calculate_angle();
    handle_cnc();
    update_motors();
}

void update_motors()
{
  if (millis() - last_motors_update_time >= UPDATE_MOTORS_TIMEOUT) {
    for (uint8_t i = 0; i < MOTORS_COUNT; i++) {
      motors[i].writeMicroseconds(motors_speed[i]);
    }
    last_motors_update_time = millis();
  }
}
