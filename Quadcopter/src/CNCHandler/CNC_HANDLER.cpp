#include <CNCHandler/CNC_HANDLER_exports.h>
#include <EEPROMManager/EEPROM_MANAGER_exports.h>
#include <TelemetrySender/TELEMETRY_SENDER_exports.h>
#include <PID/PID_exports.h>
#include <Receiver/RECEIVER_exports.h>
#include <RF24.h>
#include <Utils/UTILS.h>

RF24 myRadio (7, 8);
byte addresses[][6] = {"ORAN1", "SHAHA"};
int dataTransmitted;

CNC_HANDLER_t CNC_HANDLER__cnc_g;

void CNC_HANDLER__init()
{
  myRadio.begin();
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.enableAckPayload();
  myRadio.setAutoAck(true);
  myRadio.setRetries(5,5);
  myRadio.setCRCLength( RF24_CRC_16 ) ;

  myRadio.openWritingPipe(addresses[0]);
  myRadio.openReadingPipe(1, addresses[1]);
  myRadio.startListening();
  delay(1000);//
}

void CNC_HANDLER__handle_cnc()
{
  if (myRadio.available()) {
    myRadio.read(&CNC_HANDLER__cnc_g, sizeof(CNC_HANDLER__cnc_g));
    #define HANDLER(__cnc_id, __handler) \
      if (__cnc_id == CNC_HANDLER__cnc_g.id) {\
        __handler(); \
      }
    #include <CNCHandler/CNC_HANDLER_handlers.hx>
    #undef HANDLER
  }
}

void cnc_handler__set_motors_speed()
{
  Serial.println("Setting speed");
  for (int i = 0; i < MOTORS_COUNT; i++) {
    motors_speed[i] = CNC_HANDLER__cnc_g.payload.motor_speed;
  }
  Serial.println(CNC_HANDLER__cnc_g.payload.motor_speed);
}

void cnc_handler__send_angles()
{
  TELEMETRY_SENDER__send(ANGLES, &imu.angles);
  Serial.print(imu.angles.x);
  Serial.print(",");
  Serial.print(imu.angles.y);
  Serial.print(",");
  Serial.println(imu.angles.z);
}

void cnc_handler__get_pid()
{
  PID_DATA_t pid;
  READ_EEPROM_FIELD(pid, pid_data);
  Serial.print("P: ");
  Serial.print(pid.xyPID.p);
  Serial.print("I: ");
  Serial.print(pid.xyPID.i);
  Serial.print("D: ");
  Serial.print(pid.xyPID.d);
  Serial.print("P: ");
  Serial.print(pid.zPID.p);
  Serial.print("I: ");
  Serial.print(pid.zPID.i);
  Serial.print("D: ");
  Serial.println(pid.zPID.d);
  TELEMETRY_SENDER__send(PID, &pid);
}

void cnc_handler__set_pid()
{
  PID__update_field(CNC_HANDLER__cnc_g.payload.pid.pid_field_id,
                    CNC_HANDLER__cnc_g.payload.pid.value);
}

void cnc_handler__set_receiver_state()
{
  RECEIVER__update(&CNC_HANDLER__cnc_g.payload.receiver);
}

void cnc_handler__arm()
{

}

void cnc_handler__disarm()
{

}
