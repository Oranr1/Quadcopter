#ifndef TELEMETRY_SENDER_EXPORTS_H
#define TELEMETRY_SENDER_EXPORTS_H

#include "IMU.h"
#include <Pid/PID_exports.h>
#include <EEPROMManager/EEPROM_MANAGER_exports.h>

#include <RF24.h>
extern RF24 myRadio;

typedef enum {
  ANGLES,
  PID
} TELEMETRY_SENDER_id_t;

typedef union {
  IMU__3_axis_angles_t angles;
  PID_DATA_t pid;
} TELEMETRY_SENDER_payload_t;

typedef struct {
  TELEMETRY_SENDER_id_t id;
  TELEMETRY_SENDER_payload_t payload;
} TELEMETRY_SENDER_t;

void TELEMETRY_SENDER__send(TELEMETRY_SENDER_id_t id, void *payload);

#endif /* TELEMETRY_SENDER_EXPORTS.H */
