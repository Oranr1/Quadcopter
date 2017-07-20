#define SAFE

//-------Motor PWM Levels
#define MOTOR_ZERO_LEVEL  1000
#define MOTOR_MAX_LEVEL  2000

//-------RX Config----------
#define THROTTLE_RMIN  910
#define THROTTLE_SAFE_SHUTOFF 1120
#define THROTTLE_RMAX 2030
#define THROTTLE_RMID  1470

#define ROLL_RMIN 920
#define ROLL_RMAX 1960
#define ROLL_WMIN  -30
#define ROLL_WMAX  30

#define PITCH_RMIN 930
#define PITCH_RMAX 1990
#define PITCH_WMIN  -30
#define PITCH_WMAX  30

#define YAW_RMIN  900
#define YAW_RMAX 2010
#define YAW_WMIN  -30
#define YAW_WMAX  30

//-------Debug Config---------
#define DEBUG_OUTPUT
#define DEBUG_ANGLES
#define DEBUG_PID
#define DEBUG_RX
#define DEBUG_MOTORS
#define DEBUG_LOOP_TIME
//----------------------------

//-------RX PINS-------------
#define RX_PINS_OFFSET 6
#define PIN_RX_ROLL 6
#define PIN_RX_PITCH 9
#define PIN_RX_THROTTLE 8
#define PIN_RX_YAW 7

//------Timer-------------
#define anglesIntervalTime 12
#define reciverIntervalTime 3
#define keepAliveIntervalTime 50

