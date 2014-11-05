/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Module: SensorValuesTask.h
//
// Description:
//        Handles all reading of sensors
//          * Accelerometer
//          * IR1 sensor
//          * IR2 sensor
//          * Ultrasonic sensor
//          * Magnetic sensor
//          * Drive encoders
//          * Arm encoders
//          * Gyro
//
// Usage:
//        1) Run the SensorValuesTask task
//        2) Call the appropriate function to read the desired sensor value
//           ... gyro state is maintained in gyro.h
//
// Direct Motor Use:
//        none
//
// Direct Sensor Use:
//          * Accelerometer
//          * IR1 sensor
//          * IR2 sensor
//          * Ultrasonic sensor
//          * Magnetic sensor
//          * Drive encoders
//          * Arm encoders
//          * Gyro
//
// Timer Use:
//        none
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H

#pragma systemFile            // this eliminates warning for "unreferenced" functions

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "..\Common\Drivers\HTMAG-driver.h"            //
#include "..\Common\Drivers\HTIRS2-AC-ONLY-driver.h"	 // Hi-Technic Sensor Drivers
#include "..\Common\Drivers\LEGOUS-driver.h"		       //
#include "..\Common\Drivers\HTAC-ALL_ONLY-driver.h"		 //
#include "..\Common\Drivers\HTGYRO-driver.h"		       //
#include "Gyro.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// #defines
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public Global Variables
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private Global Variables ... gyro state is maintained in gyro.h
//
int _nAccelSensorXValue;
int _nAccelSensorYValue;
int _nAccelSensorZValue;
int _nIR1SensorValue;
int _nIR2SensorValue;
int _nUSrightSensorValue;
int _nUSleftSensorValue;
int _nMagnetSensorValue;

long _nLeftDriveEncoderValue;
long _nLastGoodLeftEncoderDelta;
long _nRightDriveEncoderValue;
long _nLastGoodRightEncoderDelta;
bool _bResetDriveEncoders;

long _nArmRotEncoderValue;
long _nLastGoodArmRotEncoderDelta;
bool _bResetArmRotEncoder;

long _nArmElevEncoderValue;
long _nLastGoodArmElevEncoderDelta;
bool _bResetArmElevEncoder;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public Function Declarations ... gyro state is maintained in gyro.h
//
#define AccelSensorXVal()	      ( _nAccelSensorXValue )
#define AccelSensorYVal() 	    ( _nAccelSensorYValue )
#define AccelSensorZVal()	      ( _nAccelSensorZValue )
#define IR1SensorVal()	        ( _nIR1SensorValue )
#define IR2SensorVal()	        ( _nIR2SensorValue )
#define USleftSensorVal()	      ( _nUSleftSensorValue )
#define USrightSensorVal()	    ( _nUSrightSensorValue )
#define USsensorVal()	          ( (_nUSrightSensorValue + _nUSleftSensorValue)/2 )
#define MagneticSensorVal() 	  ( _nMagnetSensorValue )

#ifdef READ_RAW_DRIVE_ENCODER_VALUES
  #define LeftDriveEncoderVal()   ( nMotorEncoder[Left_Drive] )
  #define RightDriveEncoderVal()  ( nMotorEncoder[Right_Drive] )
#else
  #define LeftDriveEncoderVal()   ( _nLeftDriveEncoderValue )
  #define RightDriveEncoderVal()  ( _nRightDriveEncoderValue )
#endif

#ifdef READ_RAW_ARM_ENCODER_VALUES
  #define ArmRotEncoderVal()      ( nMotorEncoder[motorF] )
  #define ArmElevEncoderVal()     ( nMotorEncoder[motorG] )
#else
  #define ArmRotEncoderVal()      ( _nArmRotEncoderValue )
  #define ArmElevEncoderVal()     ( _nArmElevEncoderValue )
#endif

void FilterEncoderSpikes(int motorID, long &currValue, long &lastDelta);

/////////////////////////////////////////////////////////////////////////////////////////
//
// PRIVATE FUNCTION: initialization
//
#define initSensorValues()            \
{                                     \
	_nAccelSensorXValue = 0;            \
	_nAccelSensorYValue = 0;            \
	_nAccelSensorZValue = 0;            \
	_nIR1SensorValue = 0;               \
	_nIR2SensorValue = 0;               \
	_nUSleftSensorValue = 0;            \
	_nUSrightSensorValue = 0;           \
	_nMagnetSensorValue = 0;            \
	_nLeftDriveEncoderValue = 0;        \
	_nRightDriveEncoderValue = 0;       \
  _nLastGoodLeftEncoderDelta = 0;     \
  _nLastGoodRightEncoderDelta = 0;    \
  _bResetDriveEncoders = false;       \
  _nArmRotEncoderValue = 0;           \
  _nLastGoodArmRotEncoderDelta = 0;   \
  _bResetArmRotEncoder = false;       \
  _nArmElevEncoderValue = 0;          \
  _nLastGoodArmElevEncoderDelta = 0;  \
  _bResetArmElevEncoder = false;      \
  InitSMUX();                         \
  InitMAG();                          \
}

#define _ResetDriveEncoders()         \
{                                     \
	nMotorEncoder[Left_Drive] = 0;      \
	nMotorEncoder[Right_Drive] = 0;     \
	_nLeftDriveEncoderValue = 0;        \
	_nRightDriveEncoderValue = 0;       \
	_nLastGoodLeftEncoderDelta = 0;     \
	_nLastGoodRightEncoderDelta = 0;    \
}

#define _ResetArmRotEncoder()         \
{                                     \
	nMotorEncoder[motorF] = 0;          \
	_nArmRotEncoderValue = 0;           \
	_nLastGoodArmRotEncoderDelta = 0;   \
}

#define _ResetArmElevEncoder()        \
{                                     \
	nMotorEncoder[motorG] = 0;          \
	_nArmElevEncoderValue = 0;          \
	_nLastGoodArmElevEncoderDelta = 0;  \
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private Function Declarations
//


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Task Definitions
//
//    Keep all sensor values updated for sensors that are connected to the SMUX
//
task SensorValuesTask()
{
	initSensorValues();

	int bReadHalfSampleRateSensors = true;

	while (true)
	{
	  // Run these every other cycle...
    if ( bReadHalfSampleRateSensors )
    {
    }

    _nUSrightSensorValue = ReadUSright();
    _nUSleftSensorValue = ReadUSleft();

#ifdef ENABLE_PID_TURN_CONTROL
    UpdateGyroInfo();
#endif

	  ReadAccel();
    _nIR1SensorValue = ReadIR1();
    _nIR2SensorValue = ReadIR2();
    _nMagnetSensorValue = ReadMAG();

#ifdef USE_DRIVE_ENCODER_SPIKE_PROTECTION
    if ( _bResetDriveEncoders )
    {
      _ResetDriveEncoders();
		  _bResetDriveEncoders = false; // setting to false will let whoever asked for the reset know it is now done.
    }
    FilterEncoderSpikes(Left_Drive, _nLeftDriveEncoderValue, _nLastGoodLeftEncoderDelta);
    FilterEncoderSpikes(Right_Drive, _nRightDriveEncoderValue, _nLastGoodRightEncoderDelta);
#else
    _nLeftDriveEncoderValue = nMotorEncoder[Left_Drive];
    _nRightDriveEncoderValue = nMotorEncoder[Right_Drive];
#endif // USE_ENCODER_SPIKE_PROTECTION

#ifdef USE_ARM_ENCODER_SPIKE_PROTECTION
    if ( _bResetArmRotEncoder )
    {
      _ResetArmRotEncoder();
		  _bResetArmRotEncoder = false; // setting to false will let whoever asked for the reset know it is now done.
    }
    FilterEncoderSpikes(motorF, _nArmRotEncoderValue, _nLastGoodArmRotEncoderDelta);

    if ( _bResetArmElevEncoder )
    {
      _ResetArmElevEncoder();
		  _bResetArmElevEncoder = false; // setting to false will let whoever asked for the reset know it is now done.
    }
    FilterEncoderSpikes(motorG, _nArmElevEncoderValue, _nLastGoodArmElevEncoderDelta);
#else
    _nArmRotEncoderValue = nMotorEncoder[motorF];
    _nArmElevEncoderValue = nMotorEncoder[motorG];
#endif // USE_ARM_ENCODER_SPIKE_PROTECTION

    bReadHalfSampleRateSensors = !bReadHalfSampleRateSensors; // run these every other cycle

		wait1Msec(SENSOR_TASK_SAMPLE_RATE);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Function Definitions
//

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: TellSensorTaskResetDriveEncoders
//
//    Since the sensor task owns the encoders, it needs to reset them so that any associated
//    state variables can also be reset.
//
//    Will block until the sensor task indicates it is done with the reset ... it will do so by clearning the flag
//
#define TellSensorTaskResetDriveEncoders()  \
{                                           \
  if ( _bResetDriveEncoders != true )       \
  {                                         \
	  _bResetDriveEncoders = true;            \
	  while ( _bResetDriveEncoders == true )  \
	    ;                                     \
	}                                         \
}

#define TellSensorTaskResetArmRotEncoder()  \
{                                           \
  if ( _bResetArmRotEncoder != true )       \
  {                                         \
	  _bResetArmRotEncoder = true;            \
	  while ( _bResetArmRotEncoder == true )  \
	    ;                                     \
	}                                         \
}

#define TellSensorTaskResetArmElevEncoder() \
{                                           \
  if ( _bResetArmElevEncoder != true )      \
  {                                         \
	  _bResetArmElevEncoder = true;           \
	  while ( _bResetArmElevEncoder == true ) \
	    ;                                     \
	}                                         \
}


/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: FilterEncoderSpikes
//
//  Ignore spikes in encoder readings ... per RobotC forum posts this seems to happen with
//  heavily loaded IC2 buses...4 encoders on 1 brick port with a SMUX and full load of sensors...
//  If we see a spike, interpolate encoder reading based on the last valid encoder delta assuming
//  the rotational rate has remained the same.
//
void FilterEncoderSpikes(int motorID, long &currValue, long &lastDelta)
{
  long newEncoderVal = nMotorEncoder[motorID];
  long encoderDelta = newEncoderVal - currValue;
  if ( absValue(encoderDelta) > MAX_VALID_ENCODER_DELTA  )
  {                                                         // looks like a spike...
    currValue += lastDelta;  // ...assume rotating at the prior rate
    //nxtDisplayTextLine(0, "Lf newD:  %ld", encoderDelta);
    //nxtDisplayTextLine(1, "Lf newE:  %ld", newEncoderVal);
  }                                                         // ...and update the interpolated encoder reading
  else
  {
  	currValue = newEncoderVal;   // doesn't look like a spike, use it
    lastDelta = encoderDelta; // and remember this delta as our current rotational rate
  }
  //nxtDisplayTextLine(2, "Lf lastE: %ld", currValue);
  //nxtDisplayTextLine(3, "Lf lastD: %ld", lastDelta);
}



/////////////////////////////////////////////////////////////////////////////////////////
//
// PRIVATE FUNCTION: <description>
//
/*
    // Ignore spikes in encoder readings ... per RobotC forum posts this seems to happen with
    // heavily loaded IC2 buses...4 encoders on 1 brick port with a SMUX and full load of sensors...
    // If we see a spike, interpolate encoder reading based on the last valid encoder delta assuming
    // the rotational rate has remained the same.
    newEncoderVal = nMotorEncoder[Left_Drive];
    encoderDelta = newEncoderVal - _nLeftDriveEncoderValue;
    if ( absValue(encoderDelta) > MAX_VALID_ENCODER_DELTA  )
    {                                                         // looks like a spike...
      _nLeftDriveEncoderValue += _nLastGoodLeftEncoderDelta;  // ...assume rotating at the prior rate
      //nxtDisplayTextLine(0, "Lf newD:  %ld", encoderDelta);
      //nxtDisplayTextLine(1, "Lf newE:  %ld", newEncoderVal);
    }                                                         // ...and update the interpolated encoder reading
    else
    {
    	_nLeftDriveEncoderValue = newEncoderVal;   // doesn't look like a spike, use it
      _nLastGoodLeftEncoderDelta = encoderDelta; // and remember this delta as our current rotational rate
    }
    //nxtDisplayTextLine(2, "Lf lastE: %ld", _nLeftDriveEncoderValue);
    //nxtDisplayTextLine(3, "Lf lastD: %ld", _nLastGoodLeftEncoderDelta);

    newEncoderVal = nMotorEncoder[Right_Drive];
    encoderDelta = newEncoderVal - _nRightDriveEncoderValue;
    if ( absValue(encoderDelta) > MAX_VALID_ENCODER_DELTA  )
    {                                                           // looks like a spike...
      _nRightDriveEncoderValue += _nLastGoodRightEncoderDelta;  // ...assume rotating at the prior rate
      //nxtDisplayTextLine(4, "Rt delta: %ld", encoderDelta);
      //nxtDisplayTextLine(5, "Rt encod: %ld", newEncoderVal);
    }                                                           // ...and update the interpolated encoder reading
    else
    {
    	_nRightDriveEncoderValue = newEncoderVal;   // doesn't look like a spike, use it
      _nLastGoodRightEncoderDelta = encoderDelta; // and remember this delta as our current rotational rate
    }
    //nxtDisplayTextLine(6, "Rt lastE: %ld", _nRightDriveEncoderValue);
    //nxtDisplayTextLine(7, "Rt lastD: %ld", _nLastGoodRightEncoderDelta);
*/
#endif // SENSOR_TASK_H
