#ifndef CNC_HANDLERS_EXPORTS
#define CNC_HANDLERS_EXPORTS

#include <EEPROMManager/EEPROM_MANAGER_exports.h>
#include <PID/PID_exports.h>

typedef enum {
  #define HANDLER(__cnc_id, __handler) \
    __cnc_id,
  #include <CNCHandler/CNC_HANDLER_handlers.hx>
  #undef HANDLER
} CNC_HANDLER__id_t;

typedef union {
  PID__set_cnc_t pid;
  uint32_t motor_speed;
} CNC_HANDLER_payload_t;

typedef struct {
  CNC_HANDLER__id_t id;
  CNC_HANDLER_payload_t payload;
} CNC_HANDLER_t;

void initialize_cncs();
void handle_cnc();
void cnc_handler__send_angles();
void cnc_handler__set_motors_speed();
void cnc_handler__get_pid();
void cnc_handler__set_pid();
void CNC_HANDLER__handle_cnc();

#endif
