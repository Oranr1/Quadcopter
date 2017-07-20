#include <Arduino.h>
#include <Configuration.h>
#include <PinChangeInt.h>


volatile int pwm_start_time[4];
uint8_t latest_interrupted_pin;
volatile int rx_values[4]; // ROLL, PITCH, THROTTLE, YAW
void Emergency(void);
void rising();
void falling();
void rx_initialize() {
  pinMode(PIN_RX_ROLL, INPUT); digitalWrite(PIN_RX_ROLL, HIGH);
  PCintPort::attachInterrupt(PIN_RX_ROLL, &rising, RISING);

  pinMode(PIN_RX_PITCH, INPUT); digitalWrite(PIN_RX_PITCH, HIGH);
  PCintPort::attachInterrupt(PIN_RX_PITCH, &rising, RISING);

  pinMode(PIN_RX_THROTTLE, INPUT); digitalWrite(PIN_RX_THROTTLE, HIGH);
  PCintPort::attachInterrupt(PIN_RX_THROTTLE, &rising, RISING);

  pinMode(PIN_RX_YAW, INPUT); digitalWrite(PIN_RX_YAW, HIGH);
  PCintPort::attachInterrupt(PIN_RX_YAW, &rising, RISING);
}

void getYawPitchRollThrottle(int *yaw, int *pitch, int *roll, int *throttle)
{
  *roll = rx_values[0];
  *yaw = rx_values[1];
  *throttle = rx_values[2];
  *pitch = rx_values[3];
}

int fail_counter = 0;
void rx_update(int *roll_rate, int *pitch_rate, int *yaw_rate, int *throttle) {
  // here we allow +- 20 for noise and sensitivity on the RX controls...
  // ROLL rx at mid level?
  int yaw, pitch, roll;

  getYawPitchRollThrottle(&yaw, &pitch, &roll, throttle);
  if(roll > THROTTLE_RMID - 20 && roll < THROTTLE_RMID + 20)
    *roll_rate = 0;
  else
    *roll_rate = map(roll,ROLL_RMIN,ROLL_RMAX,ROLL_WMIN,ROLL_WMAX);
  //PITCH rx at mid level?
  if(pitch > THROTTLE_RMID - 20 && pitch < THROTTLE_RMID + 20)
    *pitch_rate = 0;
  else
    *pitch_rate = map(pitch,PITCH_RMIN,PITCH_RMAX,PITCH_WMIN,PITCH_WMAX);
  //YAW rx at mid level?
  if(yaw > THROTTLE_RMID - 20 && yaw < THROTTLE_RMID + 20)
    *yaw_rate = 0;
  else
    *yaw_rate = map(yaw,YAW_RMIN,YAW_RMAX,YAW_WMIN,YAW_WMAX);

  *throttle=map(*throttle,THROTTLE_RMIN,THROTTLE_RMAX,MOTOR_ZERO_LEVEL,MOTOR_MAX_LEVEL);
  if (*throttle > MOTOR_MAX_LEVEL) {
    *throttle = MOTOR_MAX_LEVEL;
  }
  else if (*throttle < MOTOR_ZERO_LEVEL) {
    *throttle = MOTOR_ZERO_LEVEL;
  }

  if (*roll_rate < ROLL_WMIN ||
      *roll_rate > ROLL_WMAX ||
      *pitch_rate < PITCH_WMIN ||
      *pitch_rate > PITCH_WMAX ||
      *yaw_rate < YAW_WMIN ||
      *yaw_rate > YAW_WMAX) {
        fail_counter++;
    }
    else {
      fail_counter = 0;
    }

    if (fail_counter >= 1) {
        fail_counter = 0;
        Emergency();

    }
}

void rising()
{
  latest_interrupted_pin=PCintPort::arduinoPin;
  PCintPort::attachInterrupt(latest_interrupted_pin, &falling, FALLING);
  pwm_start_time[latest_interrupted_pin-RX_PINS_OFFSET] = micros();
}

void falling() {
  latest_interrupted_pin=PCintPort::arduinoPin;
  PCintPort::attachInterrupt(latest_interrupted_pin, &rising, RISING);
  rx_values[latest_interrupted_pin-RX_PINS_OFFSET] = micros()-pwm_start_time[latest_interrupted_pin-RX_PINS_OFFSET];
}
