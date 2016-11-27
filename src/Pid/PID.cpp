#include <arduino.h>
#include <string.h>
#include <EEPROMManager/EEPROM_MANAGER_exports.h>
#include <PID/PID_exports.h>

void PID__update_field(PID__field_id_t field_id, double value)
{
  PID_DATA_t pid;
  READ_EEPROM_FIELD(pid, pid_data);

  double *pid_data = (double *)&pid;
  memcpy(&pid_data[field_id], &value, sizeof(value));
  WRITE_EEPROM_FIELD(pid_data, pid);

  Serial.print("Updated pid field_id ");
  Serial.print(field_id);
  Serial.print(", value: ");
  Serial.println(value);
}
