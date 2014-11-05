#ifndef TEAM_5076_SENSOR_CONFIG_H
#define TEAM_5076_SENSOR_CONFIG_H

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TEAM SPECIFIC SETTINGS ... THESE ARE THE ONLY SETTINGS THAT DIFFER BETWEEN 3717 AND 4590'S BOTS
//
#if TEAM_NUMBER == 5076

  //-------------------------------------------------------------------------------------------
  // SMUX sensor ports...
  //
  //#define USING_SMUX_LO
  //#define USING_SMUX_HI

  //-------------------------------------------------------------------------------------------
  // Sensor usage
  //
  #define USING_GYRO
	//#define USING_IR1
	//#define USING_IR2
	//#define USING_ACCEL
	//#define USING_MAG
	//#define USING_US1
	//#define USING_US2

  //-------------------------------------------------------------------------------------------
	//
	// SENSOR CALIBRATION INFORMATION
	//
	//  These are the scale amounts for each gyro.  Each gyro performs slightly different but
	//  predictably.  Through observation (manual sensor calibration, the linear scaling factor
	//  is determined for each gyro and recorded here.
	//
  #define GYRO_ON_BRICK_SCALE (91/90.0)
  #define GYRO_ON_SMUX_SCALE (90/90.0)

  //-------------------------------------------------------------------------------------------
  //-------------------------------------------------------------------------------------------
	// Sensor init and access macros
	// ...the read accelerometer function does not return a value but takes the storage variables by references
	// ...all the others return the sensor value
  //
	#define SENSOR_TASK_SAMPLE_RATE 5

  //-------------------------------------------------------------------------------------------
	// Select one of the following as the integration method used to determine angular displacement
  //
  //#define GYRO_USE_DEBIASED_ROT_RATE
  //#define GYRO_USE_WEIGHTED_AVG_ROT_RATE
  #define GYRO_USE_TRAPEZOIDAL_APPROX

  //-------------------------------------------------------------------------------------------
  // Encoder task settings...
  //
  #define ENCODER_TASK_SAMPLE_RATE 10
  #define LEFT_ENCODER_MOTOR  DriveLeft
  #define RIGHT_ENCODER_MOTOR DriveRight

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#else

  xxx YOU MUST USE ONE OF THE TEAM #DEFINES IN THE Autonomous Template.h FILE BONEHEAD! xxx

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TEAM_5076_SENSOR_CONFIG_H
