/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Module: DriveControlEncoder.h
//
// Description:
//        Drive system interface handling all encoder controlled drive commands
//
// Usage:
//
// Direct Motor Use:
//
// Direct Sensor Use:
//
// Timer Use:
//        T2 - driving straight/turning timeouts (poor man's stall protection)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DRIVE_CONTROL_ENCODER_H
#define DRIVE_CONTROL_ENCODER_H

#pragma systemFile            // this eliminates warning for "unreferenced" functions

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Includes
//

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
// Private Global Variables
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public Function Declarations
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private Function Declarations
//

void EncoderTurnClockwise90();
void EncoderTurnCounterClockwise90();

void EncoderDriveStraight( float fSquares );

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Task Definitions
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Function Definitions
//

/////////////////////////////////////////////////////////////////////////////////////////
//
// PRIVATE FUNCTION: Encoder controlled 90 degree clockwise turn
//
void EncoderTurnClockwise90()
{
  const int PwrGradient = ENCODER_TURN_MAX_PWR - ENCODER_TURN_MIN_PWR;
  int goal= ENCODER_CLOCKWISE90_COUNT;

  float error = 0.0;
  int pwr = 0;
  int maxPwr = 0;

  if ( AutonomousIsAborted() )
    return;

  int nTurnTimeLimit = MSECS_90_DEGREES;
  eraseDisplay();

  ResetDriveEncoders();

  ClearCustomTimer(CT4);
  ClearTimer(T2);

  int currValue = CurrMotorEncoder(LEFT_SIDE);

  while (currValue < goal)
  {
    if ( time1[T2] > nTurnTimeLimit )
    {
      SetAbortAutonomous(true);
      break;
    }

    error = 1 - currValue/goal;
    pwr   = ENCODER_TURN_MIN_PWR + ( error * PwrGradient );
    maxPwr = max2(pwr, maxPwr);

    SetDriveSidePower(LEFT_SIDE,   pwr);
    SetDriveSidePower(RIGHT_SIDE,  -pwr);

    currValue = CurrMotorEncoder(LEFT_SIDE);
	}

  SetDrivePower(0);

  int mSecs = CustomTimer(CT4);
  wait1Msec(1000);
  int skidValue = CurrMotorEncoder(LEFT_SIDE);
  int battry = externalBatteryAvg;

  nxtDisplayTextLine(0, "ENCODER CCW TURN");
  nxtDisplayTextLine(1, "bttry: %d", battry);
  nxtDisplayTextLine(3, "mSecs: %d", mSecs);
  nxtDisplayTextLine(4, "Angle: %3.2f", currValue);
  nxtDisplayTextLine(5, "Skid:  %3.2f", skidValue);
  nxtDisplayTextLine(7, "maxPwr: %d", maxPwr);

  wait1Msec(250);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// PRIVATE FUNCTION: Encoder controlled 90 degree counter-clockwise turn
//
void EncoderTurnCounterClockwise90()
{
  const int PwrGradient = ENCODER_TURN_MAX_PWR - ENCODER_TURN_MIN_PWR;
  int goal= ENCODER_CLOCKWISE90_COUNT;

  int error = 0;
  int pwr = 0;
  int maxPwr = 0;

  if ( AutonomousIsAborted() )
    return;

  int nTurnTimeLimit = MSECS_90_DEGREES;

  eraseDisplay();

  ResetDriveEncoders();

  ClearCustomTimer(CT4);
  ClearTimer(T2);

  int currValue = CurrMotorEncoder(RIGHT_SIDE);
  while (currValue < ENCODER_COUNTERCLOCKWISE90_COUNT)
  {
    if ( time1[T2] > nTurnTimeLimit )
    {
      SetAbortAutonomous(true);
      break;
    }

    error = 1 - currValue/ENCODER_COUNTERCLOCKWISE90_COUNT;
    pwr = ENCODER_TURN_MIN_PWR + ( error * PwrGradient );
    maxPwr = max2(pwr, maxPwr);

    SetDriveSidePower(LEFT_SIDE, -pwr);
    SetDriveSidePower(RIGHT_SIDE, pwr);

    currValue = CurrMotorEncoder(RIGHT_SIDE);
  }

  SetDrivePower(0);

  int mSecs = CustomTimer(CT4);
  wait1Msec(1000);
  int skidValue = CurrMotorEncoder(RIGHT_SIDE);
  int battry = externalBatteryAvg;

  nxtDisplayTextLine(0, "ENCODER CCW TURN");
  nxtDisplayTextLine(1, "bttry: %d", battry);
  nxtDisplayTextLine(3, "mSecs: %d", mSecs);
  nxtDisplayTextLine(4, "Angle: %3.2f", currValue);
  nxtDisplayTextLine(5, "Skid:  %3.2f", skidValue);
  nxtDisplayTextLine(7, "maxPwr: %d", maxPwr);

  wait1Msec(250);
}

#ifndef ENABLE_PID_DRIVE_CONTROL

/////////////////////////////////////////////////////////////////////////////////////////
//
// PRIVATE FUNCTION: Encoder drive control ... move the indicated number of foam tiles
//
void EncoderDriveStraight(float fSquares, TPowerRampingType pwrRmpType)
{
  if ( AutonomousIsAborted() )
    return;

  const int nRunTimeLimit = (int)(MSECS_ONE_SQUARE * fSquares);

  const float fGoalDistance = (ONE_SQUARE_ENCODER_COUNT * fSquares) - MOMENTUM_FACTOR_TICKS;

  float fPwrRampScale = 1.0;

  ResetDriveEncoders();

  float fLeftEncoder = CurrMotorEncoder(LEFT_SIDE);
  float fRightEncoder = CurrMotorEncoder(RIGHT_SIDE);

  ClearTimer(T2);

  // Drive until both motors have reached their target distance
  if (fSquares >= 0)
  {
    while ( fLeftEncoder < fGoalDistance && fRightEncoder < fGoalDistance)
    {
	    if ( time1[T2] > nRunTimeLimit )
	    {
        SetAbortAutonomous(true);
	      break;
	    }

	    fPwrRampScale = 1.0;

	    // Allow power to scale up/down in a smooth gradient so we're not immediately applying max power.
	    // Goal is to avoid spinning out when starting or skidding when stopping.
	    if(pwrRmpType != NO_PWR_RAMP) {
				#ifdef USE_COSINE_CURVE_POWER_RAMP
				    fPwrRampScale = CosPowerCurveForRampedPower(fGoalDistance, fLeftEncoder, fRightEncoder);
				#elif defined(USE_LINEAR_POWER_RAMP)
				    fPwrRampScale = LinearPowerCurveForRampedPower(fGoalDistance, fLeftEncoder, fRightEncoder);
				#endif
			}

	    SetDriveSidePower(LEFT_SIDE,  ENCODER_DRIVE_LEFT_PWR * fPwrRampScale);
	    SetDriveSidePower(RIGHT_SIDE, ENCODER_DRIVE_RIGHT_PWR * fPwrRampScale);

		  fLeftEncoder = CurrMotorEncoder(LEFT_SIDE);
		  fRightEncoder = CurrMotorEncoder(RIGHT_SIDE);
    }
  }
  else if (fSquares < 0) // accepts negative square values to move backwards
  {
    while ( fLeftEncoder > fGoalDistance && fRightEncoder > fGoalDistance)
    {
	    if ( time1[T2] > nRunTimeLimit )
	    {
        SetAbortAutonomous(true);
	      break;
	    }

			#ifdef USE_COSINE_CURVE_POWER_RAMP
			    fPwrRampScale = CosPowerCurveForRampedPower(fGoalDistance, fLeftEncoder, fRightEncoder);
			#elif defined(USE_LINEAR_POWER_RAMP)
					poop
			    fPwrRampScale = LinearPowerCurveForRampedPower(fGoalDistance, fLeftEncoder, fRightEncoder);
			#else
					ppp
			    fPwrRampScale = 1.0; // just simply apply the max power with no ramping...
			#endif

			writeDebugStreamLine("pwr: %6.0f", ENCODER_DRIVE_LEFT_PWR * fPwrRampScale);

	    SetDriveSidePower(LEFT_SIDE,  -ENCODER_DRIVE_LEFT_PWR * fPwrRampScale);
	    SetDriveSidePower(RIGHT_SIDE, -ENCODER_DRIVE_RIGHT_PWR * fPwrRampScale);

		  fLeftEncoder = CurrMotorEncoder(LEFT_SIDE);
		  fRightEncoder = CurrMotorEncoder(RIGHT_SIDE);
    }
  }

	SetDrivePower(0);

	  nxtDisplayTextLine(1, "goal:  %3.2f", fGoalDistance);
		nxtDisplayTextLine(4, "left:  %6.0f", fLeftEncoder);
		nxtDisplayTextLine(5, "right: %6.0f", fRightEncoder);

  wait1Msec(250);
}

#endif // ENABLE_PID_DRIVE_CONTROL not defined

#endif // DRIVE_CONTROL_ENCODER_H
