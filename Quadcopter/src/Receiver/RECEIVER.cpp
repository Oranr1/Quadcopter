#include <Receiver/RECEIVER_exports.h>
#include <Utils/UTILS.h>
#include <string.h>
#include <arduino.h>

RECEIVER__state_t RECEIVER__state_g = {0, 0, 0, STOP_MOTOR_SPEED};

void RECEIVER__update(RECEIVER__state_t *state)
{
  memcpy(&RECEIVER__state_g, state, sizeof(RECEIVER__state_g));
}
