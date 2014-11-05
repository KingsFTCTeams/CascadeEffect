#ifndef TEAM_3717_H
#define TEAM_3717_H

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TEAM SPECIFIC SETTINGS ... THESE ARE THE ONLY SETTINGS THAT DIFFER BETWEEN 3717 AND 4590'S BOTS
//
#if TEAM_NUMBER == 3717

	// comment in/out the following individually to enable/disable the feature
  //
	#define ENABLE_NXT2EXCEL_DEBUGGING

	#define ENABLE_PID_DRIVE_CONTROL

	// Used in drive control to ramp power smoothly ... select one or none
	//#define USE_LINEAR_POWER_RAMP
	#define USE_COSINE_CURVE_POWER_RAMP

	// Turn control method setting ... select one and only one!
  #define ENABLE_GYRO_TURN_CONTROL
  //#define ENABLE_PID_TURN_CONTROL
  //#define ENABLE_ENCODER_TURN_CONTROL

#if defined(ENABLE_PID_TURN_CONTROL) || defined(ENABLE_GYRO_TURN_CONTROL)
  #define USING_GYRO
#endif

	//#define ENABLE_DRIVE_STALL_PROTECTION

  //#define READ_RAW_DRIVE_ENCODER_VALUES
  #define USE_DRIVE_ENCODER_SPIKE_PROTECTION

  // Alerts for various subsystems
  // ... unless the flag has a value such as ALERTS_DEFAULT_STATE_ON, just comment in or out to enable/disable
  #define ALERTS_DEFAULT_STATE_ON true // set to true or false
  #define ENABLE_COLLECTION_ALERTS
  #define ENABLE_PAYLOAD_ALERTS
  //#define ENABLE_SPECIAL_RESOURCE_ALERT

  // Minimum required battery voltages
  #define MIN_REQ_EXT_BATTERY_VOLTAGE 12800
  #define MIN_REQ_NXT_BATTERY_VOLTAGE 7000

  // Encoder task settings...
  #define ENCODER_TASK_SAMPLE_RATE 10

  // SMUX sensor ports...
  //#define USING_SMUX_LO
  //#define USING_SMUX_HI

  // lower smux
  #define ACCEL   0   // accelerometer
	#define USleft  0   // left ultra-sonic sensor
	#define MAG     0   // magnetic sensor
	#define USright 0   // right ultra-sonic sensor

  // upper smux
	#define IRleft  0   // left IR sensor
	#define GYRO    S4   // gyro sensor

	// Gyro sensor settings...
  //#define GYRO_LOC ON_SMUX  // indicates where the gyro is plugged in
  #define GYRO_LOC ON_BRICK // indicates where the gyro is plugged in

  // Sensor init and access macros
  // ...the read accelerometer function does not return a value but takes the storage variables by references
  // ...all the others return the sensor value
	#define SENSOR_TASK_SAMPLE_RATE 50


	//#define ReadAccel()   { HTACreadAllAxes(ACCEL, _nAccelSensorXValue, _nAccelSensorYValue,_nAccelSensorZValue); }
	#define ReadAccel()   { }

	//#define ReadUSright() USreadDist(USright)
	//#define ReadUSleft()  USreadDist(USleft)
	#define ReadUSright() 0
	#define ReadUSleft()  0

	//#define ReadIRleft()  HTIRS2readACDir(IRleft)
	//#define ReadIRright() HTIRS2readACDir(IRright)
	#define ReadIRleft()  0
	#define ReadIRright() 0

	//#define ReadMAG()     HTMAGreadVal(MAG)
  //#define ReadGYRO()    HTGYROreadRot(GYRO)
	#define ReadMAG()     0
  #define ReadGYRO()    SensorValue[GyroSensor]

	//#define InitSmuxLo()      { HTSMUXscanPorts(HTSmuxLo); wait1Msec(500); }
	//#define InitSmuxHi()      { HTSMUXscanPorts(HTSmuxHi); wait1Msec(500); }
	#define InitSmuxLo()      {  }
	#define InitSmuxHi()      {  }

	//#define InitSMUX()        { HTSMUXinit(); InitSmuxLo(); InitSmuxHi(); }
	#define InitSMUX()        {  }

	//#define InitMAG()         { HTMAGstartCal(MAG); }
	#define InitMAG()         {  }

  #define InitGYRO_BRICK()  { if (SensorType[GYRO] != sensorRawValue) { SetSensorType(GYRO, sensorRawValue); wait1Msec(100); } }
  #define InitGYRO_SMUX()   { }
  #define InitGYRO()        InitGYRO_BRICK()

	// Basic drive settings
	#define VOLTAGE_ADJUST .25
	#define BASE_VOLTAGE   12.7

	// Encoder spike protection settings...
	#define MAX_VALID_ENCODER_DELTA (1.30 * 35 * ENCODER_TASK_SAMPLE_RATE) // 152RPM -> 3.648 encoder ticks/ms with 30% buffer

	// Encoder controlled driving forward settings...
	#define ONE_WAVEY_SQUARE_ENCODER_COUNT 3400 // encoder ticks 1 corrogated square (starting from center of corrogated square)
	#define ONE_SQUARE_ENCODER_COUNT       (2550 * 1.65) // ...regular foam tile (was 2750)
	#define ENCODER_DRIVE_LEFT_PWR  75
	#define ENCODER_DRIVE_RIGHT_PWR 77

  // Encoder counts for turning (not used with PID which uses the gyro)
	#define ENCODER_CLOCKWISE90_COUNT        1800 // 1850
	#define ENCODER_COUNTERCLOCKWISE90_COUNT 1650
  #define ENCODER_TURN_SPEED 45
  #define ENCODER_TURN_MIN_PWR    25
  #define ENCODER_TURN_MAX_PWR    60

  // PID control settings...
	#define DRIVE_ENCODER_Kp 1.5  // PID drive control can use either the encoder or the gyro...
	#define DRIVE_ENCODER_Ki 0
	#define DRIVE_ENCODER_Kd 0

	#define DRIVE_GYRO_Kp    5.0  // PID drive control can use either the encoder or the gyro...
	#define DRIVE_GYRO_Ki    0
	#define DRIVE_GYRO_Kd    0

	#define DRIVE_PID_BASE_PWR 60 // base power in PID drive control
	#define DRIVE_PID_MIN_PWR  13 // minimum power in PID drive control
	#define DRIVE_PID_MIN_ADJ  1  // min power usable in PID drive control
	#define DRIVE_PID_MAX_ADJ  10 // max power usable in PID drive control
	#define DRIVE_PID_RAMP_DOWN_TICKS     100 // power ramp down start distance (in encoder ticks)

	// following control stall timeouts for PID drive control
	#define DRIVE_PID_TIME_SLOPE -100         // determine max time for 60% pwr (MSECS_ONE_SQUARE), calculate max time for 10% pwr
	#define DRIVE_PID_TIME_Y_INTERCEPT 7500   // this gives slope...now use y=mx+b and one of the pts to determine b.
                                            // example: 60% pwr, 1500ms (60, 1500) and 10% pwr, 9000ms (10, 9000)

	// following control power ramping when using cosine curve
	#define COS_PWR_RAMP_AMPLITUDE 4.00 // increasing makes ramp up and down steeper
	#define COS_PWR_RAMP_EXPONENT  0.90 // reducing slightly makes ramp up steeper but doesn't appreciably affect ramp down
	#define COS_PWR_RAMP_MIN_CLIP  0.40 // offset added to x^2 * cos() function ... establishes minimum power
	#define COS_PWR_RAMP_MAX_CLIP  1.00 // max power ... output is clipped to this

	#define TURN_Kp 1.0        // .6
	#define TURN_Ki .02        // .008
	#define TURN_Kd 20         // 0

	#define TURN_PID_MIN_PWR 0          // minimum power to use in PID controlled turn
	#define TURN_PID_MAX_PWR 75         // maximum power to use in PID controlled turn
	#define TURN_PID_ANGLE_TOLERANCE .5 // how close we need to be to call it good...

	// Gyro turn settings...
  #define GYRO_TURN_MIN_PWR    35   // used in gyro based gradient power turns...
  #define GYRO_TURN_MAX_PWR    60
  #define GYRO_CLOCKWISE90_TURN_GOAL         90
  #define GYRO_COUNTERCLOCKWISE90_TURN_GOAL -90

  // Stall protection settings...
	//  ... simple timeouts on basic movement actions such as drive X tiles or turn 90 degrees
	//  ... these are based on tested averages
	//  ... these provide a "safe" time buffer for each action by multiplying the tested average
	#define MSECS_ONE_SQUARE       1.85 * TESTED_MSECS_ONE_SQUARE
	#define MSECS_90_DEGREES       2.5 * TESTED_MSECS_90_DEGREES
	#define MSECS_1_DEGREE         (MSECS_90_DEGREES/90 + 1)

  #define TESTED_MSECS_ONE_SQUARE 810   // mSecs per square (2') seen on average during testing (autonomous driving straight)
  #define TESTED_MSECS_90_DEGREES 2000  // mSecs per 90 degree turn seen on average during testing (autonomous)
  #define DRIVE_RAMP_UP_DUR 250         // sufficient time to ensure drive system is rotating at normal speed
  #define MSECS_TO_HOLD_STALL 50        // number of mSecs to hold stall flag true
  #define EXPECTED_TICKS_SCALAR .75     // percentage of actual/tested ticks per sec...used to determine expected for "real" stall protection

  #define TESTED_SECS_ONE_SQUARE  ((float)TESTED_MSECS_ONE_SQUARE / 1000)

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // GAME SPECIFIC VALUES - these will change from year to year
  //

	// servo positions for crate flap servo
	#define FLAP_POS_IN  105
	#define FLAP_POS_OUT 222
	#define FLAP_MOVEMENT_DELAY 500

	// servo positions for back gate servo
	#define GATE_POS_DWN 94
	#define GATE_POS_UP  210
	#define GATE_MOVEMENT_DELAY 500

	// lift system parameters
	#define LIFT_RAISE_PWR -80
	#define LIFT_LOWER_PWR 80
	#define LIFT_COLLECTION_POS_HI 930
  #define LIFT_COLLECTION_POS_LOW 910
	#define LIFT_BOTTOM_POS     0

	// crate grabber system parameters ... servo positions for lift wrists
	#define RIGHT_WRIST_CRATE_WIDE_HOLD_POS 55
	#define LEFT_WRIST_CRATE_WIDE_HOLD_POS  210

	#define RIGHT_WRIST_OPEN_POS 110
	#define LEFT_WRIST_OPEN_POS  165

	#define RIGHT_WRIST_CLOSED_POS 0
	#define LEFT_WRIST_CLOSED_POS  255

	#define RIGHT_WRIST_CRATE_COLLECT_HOLD_POS 73
	#define LEFT_WRIST_CRATE_COLLECT_HOLD_POS  202

  #define RIGHT_WRIST_CRATE_GRAB_DOWN_POS 83
  #define LEFT_WRIST_CRATE_GRAB_DOWN_POS 192

	// Factor Level for drive system
  #define powerFactor 0.8


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#else

  xxx YOU MUST USE ONE OF THE TEAM #DEFINES IN THE Autonomous Template.h FILE BONEHEAD! xxx

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TEAM_3717_H
