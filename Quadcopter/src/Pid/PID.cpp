#include <arduino.h>
#include <string.h>
#include <EEPROMManager/EEPROM_MANAGER_exports.h>
#include <Receiver/RECEIVER_exports.h>
#include <PID/PID_exports.h>
#include <PID/PID_internal.h>
#include <PID_v1.h>
#include <Utils/UTILS.h>

double angleY = 0, angleX = 0, angleZ = 0;
PID__vector_t PID__powers_g = {0};
PID__vector_t PID__balanced_angles_g = {0};
uint16_t pid__previous_throttle = 0;

PID yPID (&angleY, &PID__powers_g.y, &PID__balanced_angles_g.y, 0, 0, 0, DIRECT);
PID xPID (&angleX, &PID__powers_g.x, &PID__balanced_angles_g.x, 0, 0, 0, DIRECT);
PID zPID (&angleZ, &PID__powers_g.z, &PID__balanced_angles_g.z, 0, 0, 0, DIRECT);

void PID__init(void)
{
  PID_DATA_t pid;
  READ_EEPROM_FIELD(pid, pid_data);
  yPID.SetTunings(pid.xyPID.p, pid.xyPID.i, pid.xyPID.d);
  xPID.SetTunings(pid.xyPID.p, pid.xyPID.i, pid.xyPID.d);
  zPID.SetTunings(pid.zPID.p, pid.zPID.i, pid.zPID.d);

	yPID.SetOutputLimits(-PID__MAX_POWER, PID__MAX_POWER);
	yPID.SetMode(AUTOMATIC);

	xPID.SetOutputLimits(-PID__MAX_POWER, PID__MAX_POWER);
	xPID.SetMode(AUTOMATIC);

	zPID.SetOutputLimits(-PID__MAX_POWER, PID__MAX_POWER);
	zPID.SetMode(AUTOMATIC);
}

void PID__compute(void)
{
  PID__balanced_angles_g.x = RECEIVER__state_g.pitch_rate / 2;
  PID__balanced_angles_g.y = RECEIVER__state_g.roll_rate / 2;
  PID__balanced_angles_g.z -= RECEIVER__state_g.yaw_rate / 100.0;
  /*Serial.print("angle X: ");
    Serial.print(PID__balanced_angles_g.x);
    Serial.print(", angle y: ");
    Serial.print(PID__balanced_angles_g.y);
    Serial.print(", angle z: ");
    Serial.println(PID__balanced_angles_g.z);
  Serial.print("X: ");
  Serial.println(PID__balanced_angles_g.x);
  Serial.print("Y: ");
  Serial.println(PID__balanced_angles_g.y);*/
  angleX = imu.angles.x;
  angleY = imu.angles.y;
  angleZ = imu.angles.z;
  bool use_integral = true;
  if (RECEIVER__state_g.throttle < 1100) {
    use_integral = false;
  }
  if ((pid__previous_throttle > 1100) && (RECEIVER__state_g.throttle < 1100)) {
    imu.angles.z = 0;
    PID__balanced_angles_g.z = 0;
  }
  yPID.Compute(use_integral);
  xPID.Compute(use_integral);
  zPID.Compute(use_integral);

  pid__previous_throttle = RECEIVER__state_g.throttle;
}

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

  PID__init();
}
