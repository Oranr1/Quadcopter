#ifndef RECEIVER_EXPORTS_H
#define RECEIVER_EXPORTS_H

#include <stdint.h>

typedef struct {
  int8_t roll_rate;
  int8_t pitch_rate;
  int8_t yaw_rate;
  uint16_t throttle;
} RECEIVER__state_t;

extern RECEIVER__state_t RECEIVER__state_g;

void RECEIVER__update(RECEIVER__state_t *state);

#endif /* RECEIVER_EXPORTS_H */
