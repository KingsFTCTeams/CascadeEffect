#ifndef TEAM_5076_DRIVE_CONFIG_H
#define TEAM_5076_DRIVE_CONFIG_H

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TEAM SPECIFIC SETTINGS ... THESE ARE THE ONLY SETTINGS THAT DIFFER BETWEEN 3717 AND 4590'S BOTS
//
#if TEAM_NUMBER == 5076

  #define ONE_SQUARE_ENCODER_COUNT         (NUM_DRIVE_WHEEL_ENCODER_1_TILE-120)

  //-------------------------------------------------------------------------------------------
	// Some drivers like driving "backwards" to be "forwards"
	//
  #define DRIVE_TEAM_DIRECTION_PREFERENCE FORWARD

#define NaturalSpeed 0.5
#define BoostSpeed 1
#define ReducedSpeed 0.40
#define TurnSpeedPreset 40

  //-------------------------------------------------------------------------------------------
	// Method used to help the robot drive a straight line (i.e. not curve off to the side).
	// Both methods stop as soon as any drive encoder hits the distance goal.
	//
	// 		PID drive control - uses PID on either encoders or gyro to drive as straight as possible.
	//												PID_DRIVE_ERROR_ENCODER or PID_DRIVE_ERROR_GYRO
  //
	//		Encoder drive control - uses preset power (ENCODER_DRIVE_LEFT_PWR and ENCODER_DRIVE_RIGHT_PWR)
  //														to drive as straight as possible.
	//
	#define ENABLE_PID_DRIVE_CONTROL

	//-------------------------------------------------------------------------------------------
	// Used in PID drive control as the error source
	//
	//#define PID_DRIVE_ERROR_ENCODER
	#define PID_DRIVE_ERROR_GYRO

	// handles gyro facing fwd (-1) or bwd (1)
  #define PID_DRIVE_ERROR_GYRO_SIGN 1

  //-------------------------------------------------------------------------------------------
	// Used in drive control to ramp power smoothly ... select one or none
	//
	#define USE_COSINE_CURVE_POWER_RAMP

  //-------------------------------------------------------------------------------------------
	// Pivot turn control method setting ... select one and only one!
	//
  #define ENABLE_GYRO_GRADIENT_PWR_PIVOT_CONTROL
  //#define ENABLE_GYRO_PID_PIVOT_CONTROL
  //#define ENABLE_ENCODER_PIVOT_CONTROL

  // Drive system stall protection is not working...DO NOT ENABLE THIS YET!!!!
  //#define ENABLE_DRIVE_STALL_PROTECTION

  // Encoder spike protection filters random spikes in encoder reading
  #define USE_DRIVE_ENCODER_SPIKE_PROTECTION

	//-------------------------------------------------------------------------------------------
  // Basic drive settings
  //
	#define VOLTAGE_ADJUST .25
	#define BASE_VOLTAGE   12.7

	//-------------------------------------------------------------------------------------------
	// Teleop drive settings
	//
	#define DRIVE_HALF_SPEED_SCALE_FACTOR   (0.33)

	#define DRIVE_STRAIGHT_FWD_PWR   50
	#define DRIVE_STRAIGHT_REV_PWR  -50

  #define DRIVE_SPIN_PWR  100

  //-------------------------------------------------------------------------------------------
  // Encoder controlled driving forward settings...
	//
	#define DRIVE_GEAR_RATIO               (1.0/1.0) // motor:wheel
	#define DRIVE_WHEEL_DIAM               2.94
	#define DRIVE_WHEEL_CIRC               (DRIVE_WHEEL_DIAM * PI)
	#define BASE_ENCODER_TICK_1_REV        1440.0
	#define DRIVE_ENCODER_TICK_1_REV       (BASE_ENCODER_TICK_1_REV / DRIVE_GEAR_RATIO)
	#define TILE_LEN_IN_INCHES             24.0
	#define NUM_DRIVE_WHEEL_ROTS_1_TILE    (TILE_LEN_IN_INCHES / DRIVE_WHEEL_CIRC)
	#define NUM_DRIVE_WHEEL_ENCODER_1_TILE (NUM_DRIVE_WHEEL_ROTS_1_TILE * DRIVE_ENCODER_TICK_1_REV)

	// IN BOTH HEADER FILES FROM 4590 CUSTOMIZATION
  //#define ONE_SQUARE_ENCODER_COUNT       (NUM_DRIVE_WHEEL_ENCODER_1_TILE) // ...regular foam tile (was 2750)
	//#define ONE_SQUARE_ENCODER_COUNT         (NUM_DRIVE_WHEEL_ENCODER_1_TILE-120)
	#define ENCODER_DRIVE_LEFT_PWR  75
	#define ENCODER_DRIVE_RIGHT_PWR 77

	//-------------------------------------------------------------------------------------------
  // Encoder counts for encoder based turning which only supports 90 degree turns (not used
	// with PID which uses the gyro)
	//
	#define ENCODER_CLOCKWISE90_COUNT        1800 // 1850
	#define ENCODER_COUNTERCLOCKWISE90_COUNT 1650
  #define ENCODER_TURN_SPEED 45
  #define ENCODER_TURN_MIN_PWR    25
  #define ENCODER_TURN_MAX_PWR    60

  //-------------------------------------------------------------------------------------------
  // PID "drive in a straight line" control settings...
	//
	#define DRIVE_ENCODER_Kp .1  // (was 1.5) PID drive control can use either the encoder or the gyro...
	#define DRIVE_ENCODER_Ki 0	 // this set is used when driving straight using encoder difference as
	#define DRIVE_ENCODER_Kd 0	 // the error source

	#define DRIVE_GYRO_Kp    10.0  // PID drive control can use either the encoder or the gyro...
	#define DRIVE_GYRO_Ki    0		 // this set is used when driving straight using gyro angle as
	#define DRIVE_GYRO_Kd    0		 // the error source

	#define DRIVE_PID_BASE_PWR 50 // default base power in PID drive control used if no power provided

	#define DRIVE_PID_MIN_ADJ  1  // min power usable in PID drive control
	#define DRIVE_PID_MAX_ADJ  42 // max power usable in PID drive control

	//-------------------------------------------------------------------------------------------
  // following control stall timeouts for PID drive control
	//
	#define DRIVE_PID_TIME_SLOPE -100         // determine max time for 60% pwr (MSECS_ONE_SQUARE), calculate max time for 10% pwr
	#define DRIVE_PID_TIME_Y_INTERCEPT 7500   // this gives slope...now use y=mx+b and one of the pts to determine b.
                                            // example: 60% pwr, 1500ms (60, 1500) and 10% pwr, 9000ms (10, 9000)
  //-------------------------------------------------------------------------------------------
  // following control power ramping when using cosine curve
	//
	#define COS_PWR_RAMP_AMPLITUDE        70.00 // increasing makes ramp up and down steeper
	#define COS_PWR_RAMP_EXPONENT         6.00 // reducing slightly makes ramp up steeper but doesn't appreciably affect ramp down
	#define COS_PWR_RAMP_MIN_CLIP_START   0.50 // offset added to x^2 * cos() function ... establishes minimum power
	#define COS_PWR_RAMP_MIN_CLIP_END     0.10 // minimum power for the second half of driving straight
	#define COS_PWR_RAMP_MAX_CLIP         1.00 // max power ... output is clipped to this

  //-------------------------------------------------------------------------------------------
  // following control power ramping when using cosine curve
	//
	#define PIVOT_Kp 1.0        // .6
	#define PIVOT_Ki .02        // .008
	#define PIVOT_Kd 20         // 0

	#define PIVOT_PID_MIN_PWR 0          // minimum power to use in PID controlled turn
	#define PIVOT_PID_MAX_PWR 100         // maximum power to use in PID controlled turn
	#define PIVOT_PID_ANGLE_TOLERANCE .5 // how close we need to be to call it good...

  //-------------------------------------------------------------------------------------------
  // Gradient power ramping turn settings...
	//
  #define GRADIENT_TURN_MIN_PWR    50 // 30  // used in gyro based gradient power turns...
  #define GRADIENT_TURN_MAX_PWR    100 // 45

  //-------------------------------------------------------------------------------------------
	// Stall protection settings...
	//  ... simple timeouts on basic movement actions such as drive X tiles or turn 90 degrees
	//  ... these are based on tested averages
	//  ... these provide a "safe" time buffer for each action by multiplying the tested average
  //
	#define MSECS_ONE_SQUARE       1.85 * TESTED_MSECS_ONE_SQUARE
	#define MSECS_90_DEGREES       2.5 * TESTED_MSECS_90_DEGREES
	#define MSECS_1_DEGREE         (MSECS_90_DEGREES/90 + 1)

  #define TESTED_MSECS_ONE_SQUARE 810   // mSecs per square (2') seen on average during testing (autonomous driving straight)
  #define TESTED_MSECS_90_DEGREES 2000  // mSecs per 90 degree turn seen on average during testing (autonomous)
  #define DRIVE_RAMP_UP_DUR 250         // sufficient time to ensure drive system is rotating at normal speed
  #define MSECS_TO_HOLD_STALL 50        // number of mSecs to hold stall flag true
  #define EXPECTED_TICKS_SCALAR .75     // percentage of actual/tested ticks per sec...used to determine expected for "real" stall protection

  #define TESTED_SECS_ONE_SQUARE  ((float)TESTED_MSECS_ONE_SQUARE / 1000)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#else

  xxx YOU MUST USE ONE OF THE TEAM #DEFINES IN THE Autonomous Template.h FILE BONEHEAD! xxx

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TEAM_5076_DRIVE_CONFIG_H
