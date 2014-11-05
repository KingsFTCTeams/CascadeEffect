/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Module: EncoderTask.h
//
// Description:
//        Handles all reading of Tetrix motor encoders
//          * Drive
//          * Arm Rotation
//          * Arm Elevation
//
// Usage:
//        1) Run the EncoderTask task
//        2) Call the appropriate function to read the desired encoder value
//        3) Call the appropriate function to reset the desirect encoder(s)
//
// Direct Motor Use:
//        none
//
// Direct Sensor Use:
//          * Drive encoders
//          * Arm encoders
//
// Timer Use:
//        none
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ENCODER_TASK_H
#define ENCODER_TASK_H

#pragma systemFile            // this eliminates warning for "unreferenced" functions

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Includes
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// #defines
//

// convert encoder counts to squares
#define EncoderTicks2Squares(ticks)  ((float)((ticks)/(ONE_SQUARE_ENCODER_COUNT)))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public Global Variables
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private Global Variables
//
long _nLeftDriveEncoderValue = 0;
long _nLastGoodLeftEncoderDelta = 0;
long _nRightDriveEncoderValue = 0;
long _nLastGoodRightEncoderDelta = 0;
bool _bResetDriveEncoders = false;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public Function Declarations
//
#ifdef USE_DRIVE_ENCODER_SPIKE_PROTECTION
  #define LeftDriveEncoderVal()   ( _nLeftDriveEncoderValue )
  #define RightDriveEncoderVal()  ( _nRightDriveEncoderValue )
#else
  #define LeftDriveEncoderVal()   LeftRawDriveEncoderVal
  #define RightDriveEncoderVal()  RightRawDriveEncoderVal
#endif

void FilterEncoderSpikes(tDriveSide nDriveSide, long &currValue, long &lastDelta);

/////////////////////////////////////////////////////////////////////////////////////////
//
// PRIVATE FUNCTION: initialization
//
void initEncoders()
{
	_nLeftDriveEncoderValue = 0;
	_nRightDriveEncoderValue = 0;
  _nLastGoodLeftEncoderDelta = 0;
  _nLastGoodRightEncoderDelta = 0;
  _bResetDriveEncoders = false;
}

#define _ResetDriveEncoders()         \
{                                     \
	LeftRawDriveEncoderVal = 0;         \
	RightRawDriveEncoderVal = 0;        \
	_nLeftDriveEncoderValue = 0;        \
	_nRightDriveEncoderValue = 0;       \
	_nLastGoodLeftEncoderDelta = 0;     \
	_nLastGoodRightEncoderDelta = 0;    \
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private Function Declarations
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Task Definitions
//
//    Keep all encoders values updated
//
task EncoderTask()
{
	initEncoders();

	while (true)
	{
    if ( _bResetDriveEncoders )
    {
      _ResetDriveEncoders();
		  _bResetDriveEncoders = false; // setting to false will let whoever asked for the reset know it is now done.
    }
		#ifdef USE_DRIVE_ENCODER_SPIKE_PROTECTION
		    FilterEncoderSpikes(LEFT_SIDE, _nLeftDriveEncoderValue, _nLastGoodLeftEncoderDelta);
		    FilterEncoderSpikes(RIGHT_SIDE, _nRightDriveEncoderValue, _nLastGoodRightEncoderDelta);
		#else
		    _nLeftDriveEncoderValue = LeftDriveEncoderVal();
		    _nRightDriveEncoderValue = RightDriveEncoderVal();
		#endif // USE_ENCODER_SPIKE_PROTECTION

    wait1Msec(ENCODER_TASK_SAMPLE_RATE);
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
#define TellEncoderTaskResetDriveEncoders()  \
{                                            \
  if ( _bResetDriveEncoders != true )        \
  {                                          \
	  _bResetDriveEncoders = true;             \
	  while ( _bResetDriveEncoders == true )   \
	  {}                                      \
	}                                          \
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
void FilterEncoderSpikes(tDriveSide nDriveSide, long &currValue, long &lastDelta)
{
  long newEncoderVal = (nDriveSide == LEFT_SIDE) ? LeftRawDriveEncoderVal : RightRawDriveEncoderVal;
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

#endif // ENCODER_TASK_H
