#include <Arduino.h>


#include <PinChangeInt.h>
#include <SimpleTimer.h>
#include <RF24_config.h>
#include <RF24.h>
#include <printf.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <string.h>

#include "Configuration.h"


typedef struct {
  float X;
  float Y;
  float Z;
} angle_t;

typedef enum {
  GET_ANGLES,
  SET_MOTOR_SPEED,
  GET_PID_VALUES,
  SET_PID,
  RECIVER_STATE,
  KEEP_ALIVE
} cnc_id_t;

typedef enum {
  SET_XY_P,
  SET_XY_I,
  SET_XY_D,
  SET_Z_P,
  SET_Z_I,
  SET_Z_D
} pid_id_t;

typedef struct{
  int id;
  double fieldValue;
}pid_payload_t;

typedef struct{
  double Pxy;
  double Ixy;
  double Dxy;
  double Pz;
  double Iz;
  double Dz;
}current_pid_t;

typedef struct{
  int8_t roll_rate;
  int8_t pitch_rate;
  int8_t yaw_rate;
  uint16_t throttle;
}reciver_state_t;

typedef union {
  reciver_state_t reciverState;
  pid_payload_t pidPayload;
  uint32_t motorSpeed;
} cnc_payload_t;

typedef struct {
  cnc_id_t id;
  cnc_payload_t payload;
} CNC_t;

typedef enum {
  ANGLES,
  PIDVAL
} telemetry_id_t;

typedef union {
  angle_t angles;
  current_pid_t pid;
} telemetry_payload_t;

typedef struct {
  telemetry_id_t id;
  telemetry_payload_t payload;
} TELEMETRY_t;


RF24 myRadio (2, 3);
byte addresses[][6] = {"ORAN1", "SHAHA"};
char dataReceived;
cnc_id_t cnc_id = GET_ANGLES;
bool fetchAngles = false;
char actionToDo;
uint32_t motorSpeed;
bool arm = false;
int roll_rate = 0, pitch_rate = 0, yaw_rate = 0, throttle = 0;


CNC_t cnc;
pid_id_t pid;

angle_t  angles;
current_pid_t currentPid;
TELEMETRY_t telemetry;
SimpleTimer anglesTimer;
SimpleTimer reciverTimer;
SimpleTimer keepAlive;

void setup() {

  Serial.begin(115200);
  delay(1000);

  rx_initialize();
  myRadio.begin();
 // myRadio.setPALevel(RF24_PA_MIN);
  myRadio.setPALevel(RF24_PA_MAX);

  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.openWritingPipe( addresses[1]);
  myRadio.enableDynamicPayloads();
  myRadio.setAutoAck(true);
  myRadio.setRetries(5,5);
  myRadio.enableAckPayload();
  myRadio.setCRCLength(RF24_CRC_16);
  myRadio.startListening();
  anglesTimer.setInterval(anglesIntervalTime, GetAngle);
  reciverTimer.setInterval(reciverIntervalTime, SendReciverData);
  keepAlive.setInterval(keepAliveIntervalTime, SendKeepAlive);

  Serial.println ("Im");
}
void loop() {


rx_update(&roll_rate, &pitch_rate, &yaw_rate, &throttle);

keepAlive.run();

anglesTimer.run();

if(arm)
{
  reciverTimer.run();

}

InterpretRequest();
if(Serial.available())
 {
  actionToDo = Serial.read();

  if(actionToDo == 'm'){
     SetMotorsSpeed();
  }

  else if(actionToDo == 'e')
    {
      Emergency();
    }
  else if(actionToDo == 'a')
  {
    ReadByteFromMonitor();
    if(actionToDo == 's')
    {
      fetchAngles = true;
    }
    else if(actionToDo == 'e')
    {
      fetchAngles = false;
    }
  }
  else if(actionToDo == 'g')
  {
      ReadByteFromMonitor();
      if(actionToDo == 'p')
      {
         GetPid();
      }
  }
  else if(actionToDo == 's')
  {
    ReadByteFromMonitor();
    if(actionToDo == 'x')
    {
      ReadByteFromMonitor();
      if(actionToDo == 'p'){
        SetPID(SET_XY_P);
      }
      else if(actionToDo == 'i'){
        SetPID(SET_XY_I);
      }
      else if(actionToDo == 'd'){
        SetPID(SET_XY_D);
      }
    }
    else if(actionToDo == 'z')
    {
      ReadByteFromMonitor();
      if(actionToDo == 'p'){
        SetPID(SET_Z_P);
      }
      else if(actionToDo == 'i'){
        SetPID(SET_Z_I);
      }
      else if(actionToDo == 'd'){
        SetPID(SET_Z_D);
      }
    }
  }
  else if(actionToDo == 'r')
  {
    arm = true;
    Serial.println("Arm!!");
  }
 }
}

void SetMotorsSpeed()
{
    while(!Serial.available());
    cnc.payload.motorSpeed = Serial.parseInt();
    Serial.println(cnc.payload.motorSpeed );

    if(cnc.payload.motorSpeed > 2000){
    cnc.payload.motorSpeed  = 2000;
    }

    if(cnc.payload.motorSpeed < 1000){
    cnc.payload.motorSpeed  = 1000;
    }

    myRadio.stopListening();
    cnc.id = SET_MOTOR_SPEED;
    myRadio.write( &cnc, sizeof(cnc));
    myRadio.startListening();
}

void Emergency()
{
   if(arm)
   {
     myRadio.stopListening();
     cnc.payload.reciverState.roll_rate = 0;
     cnc.payload.reciverState.yaw_rate = 0;
     cnc.payload.reciverState.pitch_rate = 0;
     cnc.payload.reciverState.throttle = 1000;
     cnc.id =   RECIVER_STATE;
     myRadio.write( &cnc, sizeof(cnc) );
     myRadio.startListening();
     arm = false;
     Serial.println("DisArm!!");
   }
}

void GetAngle()
{
   if(fetchAngles)
   {
     myRadio.stopListening();
     cnc.id =   GET_ANGLES;
     myRadio.write( &cnc_id, sizeof(cnc_id) );
     myRadio.startListening();
   }
}

void ReadByteFromMonitor()
{
    while (Serial.available() <= 0);
    actionToDo = Serial.read();
}

void GetPid()
{
     myRadio.stopListening();
     cnc.id = GET_PID_VALUES;
     myRadio.write( &cnc, sizeof(cnc));
     myRadio.startListening();
}

void PrintAngles()
{
   Serial.print(telemetry.payload.angles.X);
   Serial.print(",");
   Serial.print(telemetry.payload.angles.Y);
   Serial.print(",");
   Serial.println(telemetry.payload.angles.Z);
}

void PrintPID()
{
    Serial.print("XY - P,I,D:  ");
    Serial.print(telemetry.payload.pid.Pxy);
    Serial.print(", ");
    Serial.print(telemetry.payload.pid.Ixy);
    Serial.print(", ");
    Serial.print(telemetry.payload.pid.Dxy);
    Serial.print("      Z - P,I,D:  ");
    Serial.print(telemetry.payload.pid.Pz);
    Serial.print(", ");
    Serial.print(telemetry.payload.pid.Iz);
    Serial.print(", ");
    Serial.println(telemetry.payload.pid.Dz);
}

void InterpretRequest()
{
   if(myRadio.available())
   {
    myRadio.read( &telemetry, sizeof(telemetry) );

    if(telemetry.id == ANGLES)
    {
      PrintAngles();
    }
    if(telemetry.id == PIDVAL)
    {
      PrintPID();
    }
  }
}

void SetPID(int fieldId)
{
   while(!Serial.available());
   cnc.payload.pidPayload.fieldValue = (double)Serial.parseFloat();
   Serial.println("new PID values:");

   cnc.payload.pidPayload.id = fieldId;
   cnc.id = SET_PID;
   myRadio.stopListening();
   myRadio.write( &cnc, sizeof(cnc));
   myRadio.startListening();

   GetPid();
}

void SendReciverData()
{
     myRadio.stopListening();
     if(throttle < 1100)
     {
       throttle = 1000;
     }
     cnc.payload.reciverState.roll_rate = roll_rate * 2;
     cnc.payload.reciverState.yaw_rate = yaw_rate * 2;
     cnc.payload.reciverState.pitch_rate = pitch_rate * 2;
     cnc.payload.reciverState.throttle = throttle;
     cnc.id =   RECIVER_STATE;
     myRadio.write( &cnc, sizeof(cnc) );
     myRadio.startListening();


     Serial.print("ROLL: ");
     Serial.print(roll_rate);
     Serial.print(", PITCH: ");
     Serial.print(pitch_rate);
     Serial.print(", YAW: ");
     Serial.print(yaw_rate);
     Serial.print(", THROTTLE: ");
     Serial.println(throttle);
}

void SendKeepAlive()
{
  cnc.id = KEEP_ALIVE;
  myRadio.stopListening();
  myRadio.write( &cnc, sizeof(cnc) );
  myRadio.startListening();

}
