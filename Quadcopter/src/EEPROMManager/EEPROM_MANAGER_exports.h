#ifndef EEPROM_MANAGER_EXPORTS_H
#define EEPROM_MANAGER_EXPORTS_H

#include <EEPROM.h>
#include <Pid/PID_exports.h>

typedef struct {
  PID_t xyPID;
  PID_t zPID;
} PID_DATA_t;

typedef struct {
  PID_DATA_t pid_data;
} EEPROM_MANAGER__data_t;

#define READ_EEPROM_FIELD(__var, __field_name) \
  EEPROM.get(offsetof(EEPROM_MANAGER__data_t, __field_name), __var)

#define WRITE_EEPROM_FIELD(__field_name, __value) \
  EEPROM.put(offsetof(EEPROM_MANAGER__data_t, __field_name), __value)

#endif /* EEPROM_MANAGER_EXPORTS_H */
