 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Module: DriveControlPID.h
//
// Description:
//        Drive system interface handling all PID controlled drive commands
//
// Usage:
//
// Direct Motor Use:
//
// Direct Sensor Use:
//        Gyro for PID control
//
// Timer Use:
//        T2 - driving straight/turning timeouts (poor man's stall protection)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DRIVE_CONTROL_PID_H
#define DRIVE_CONTROL_PID_H

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
void PIDdriveStraight(float fSquares, int nSquareSize, int nCmdMaxPwr, TPowerRampingType tPwrRampType);

void PivotTurn_PID(int nNumDegrees);
void PivotTurn_GradientPower(float degrees);

void SwingTurn_GradientPower(float degrees, tTurnDir turnDir, tMovementDir moveDir);

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
// PRIVATE FUNCTION: PID drive control ... move the indicated number of sized sections
//
void PIDdriveStraight(float fSquares, int nSquareSize, int nCmdMaxPwr, TPowerRampingType tPwrRampType)
{
  if ( AutonomousIsAborted() )
    return;

  //const int   nRunTimeLimit = max(2000, round(MSECS_ONE_SQUARE * abs(fSquares)));
  long   nRunTimeLimit = max2(2000, round( abs(fSquares) * (-DRIVE_PID_TIME_SLOPE * nCmdMaxPwr + DRIVE_PID_TIME_Y_INTERCEPT)));
	nRunTimeLimit = 300000;
  const float fGoalDistance = fSquares * nSquareSize;

  const float fMinAdjust = DRIVE_PID_MIN_ADJ;
  const float fMaxAdjust = nCmdMaxPwr; // DRIVE_PID_MAX_ADJ;

  const int   nMaxPower = (fSquares >= 0) ? nCmdMaxPwr : -nCmdMaxPwr;

#ifdef PID_DRIVE_ERROR_ENCODER
poop
  //const float Kp = DRIVE_ENCODER_Kp;
  float Kp = DRIVE_ENCODER_Kp;

#elif defined(PID_DRIVE_ERROR_GYRO)
  //const float Kp = DRIVE_GYRO_Kp;
  float Kp = DRIVE_GYRO_Kp;

#else
  *** PID CONFIGURATION ERROR IN TEAMCONFIG.H ***
#endif

  //const float Ki = DRIVE_ENCODER_Ki;
  //const float Kd = DRIVE_ENCODER_Kd;
  float Ki = DRIVE_ENCODER_Ki; // 3.54 won't compile with the gain variables as consts
  float Kd = DRIVE_ENCODER_Kd;

  int   nPwr;
  float fPwrRampScale = 1.0;

  float fError = 0.0;
  float fIntegral = 0.0;
  float fDerivative = 0.0;

  static float fPrevError = 0.0;

  float fAdjust = 0;

  eraseDisplay();

  ResetDriveEncoders();

  float fLeftEncoder = CurrMotorEncoder(LEFT_SIDE);
  float fRightEncoder = CurrMotorEncoder(RIGHT_SIDE);

  ClearTimer(T2);

  ResetGyro();

  while ( abs(fLeftEncoder) < abs(fGoalDistance) || abs(fRightEncoder) < abs(fGoalDistance) )
  {
    if ( time1[T2] > nRunTimeLimit )
    {
      SetAbortAutonomous(true);
      SetDrivePower(0);
      break;
    }

#ifdef PID_DRIVE_ERROR_ENCODER
poop
    fError = fLeftEncoder - fRightEncoder;
#elif defined(PID_DRIVE_ERROR_GYRO)
    fError = Gyro.fAngle * PID_DRIVE_ERROR_GYRO_SIGN;
#else
  *** PID CONFIGURATION ERROR IN TEAMCONFIG.H ***
#endif

    fIntegral += fError;
    fDerivative = fError - fPrevError;
    fPrevError = fError;

    fAdjust = (fError*Kp) + (fIntegral*Ki) + (fDerivative*Kd);
    fAdjust = limit2Range(fAdjust, fMinAdjust, fMaxAdjust);

    // Allow power to scale up/down in a smooth gradient so we're not immediately applying max power.
    // Goal is to avoid spinning out when starting or skidding when stopping.
	  if ( tPwrRampType == COS_CURVE_PWR_RAMP )
	  {
	    fPwrRampScale = CosPowerCurveForRampedPower(fGoalDistance, fLeftEncoder, fRightEncoder);
	  }
	  else
	  {
	    fPwrRampScale = 1.0; // just simply apply the max power with no ramping...
	  }

    nPwr = nMaxPower * fPwrRampScale;
    fAdjust *= fPwrRampScale;

    SetDriveSidePower(RIGHT_SIDE, nPwr + fAdjust);
    SetDriveSidePower(LEFT_SIDE,  nPwr - fAdjust);

    wait1Msec(5);


	  nxtDisplayTextLine(0, "pwr:   %d",    nPwr);
	  nxtDisplayTextLine(1, "goal:  %3.2f", fGoalDistance);
	  nxtDisplayTextLine(2, "error: %3.2f", fError);
		nxtDisplayTextLine(3, "adj:   %3.2f", fAdjust);
		nxtDisplayTextLine(4, "left:  %6.0f", fLeftEncoder);
		nxtDisplayTextLine(5, "right: %6.0f", fRightEncoder);
		// the power ramp functions will add debug information on lines 5-7

    fLeftEncoder  = CurrMotorEncoder(LEFT_SIDE);
    fRightEncoder = CurrMotorEncoder(RIGHT_SIDE);

    // compensate for a single dead encoder by ignoring it and only using the other
    if ( fLeftEncoder == 0 && fRightEncoder != 0 )
      fLeftEncoder = fRightEncoder;
    else if ( fRightEncoder == 0 && fLeftEncoder != 0 )
      fRightEncoder = fLeftEncoder;
  }

  SetDrivePower(0);

  wait1Msec(250);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
//
// PRIVATE FUNCTION: PID controlled pivot turn
//
void PivotTurn_PID(int nNumDegrees)
{
  if ( AutonomousIsAborted() )
    return;

  const float Kp = PIVOT_Kp;
  const float Ki = PIVOT_Ki;
  const float Kd = PIVOT_Kd;

  const int   nTurnTimeLimit = MSECS_1_DEGREE * abs(nNumDegrees);

  const float fGoalAngle = nNumDegrees;
  const int   nMaxTurnPower = PIVOT_PID_MAX_PWR;

  const int   nMinTurnPower = PIVOT_PID_MIN_PWR;
  const float fTurnAngleTolerance = PIVOT_PID_ANGLE_TOLERANCE;

  float fError = 0.0;
  float fIntegral = 0.0;
  float fDerivative = 0.0;

  int    nTurn = 0;
  float  fCurrAngle = 0.0;
  static float fPrevError = 0.0;

  bool   bAtGoalAngle = false;
  bool   bInTolerance = false;

  ResetGyro(); // All of our turns are relative heading turns

  ClearTimer(T2);

  while (bAtGoalAngle == false )
  {
    if ( time1[T2] > nTurnTimeLimit )
    {
	    //SetAbortAutonomous(true);
      SetDrivePower(0);
      break;
    }

    if ( bInTolerance )
    {
      nTurn = 0;             // When we've been in tolerance long enough, then
      bAtGoalAngle = true;   // stop the motors and set the "we're done" flag
    }

    else                     // Not ready yet, keep adjusting
    {
	    fCurrAngle = Gyro.fAngle;
	    fError = fGoalAngle - fCurrAngle;

	    bInTolerance = (abs(fError) < fTurnAngleTolerance) || (abs(fCurrAngle) > abs(fGoalAngle));

	    fIntegral += fError;
	    fDerivative = fError - fPrevError;
      fPrevError = fError;

      nTurn = (int)(fError*Kp) + (int)(fIntegral*Ki) + (int)(fDerivative*Kd);
      nTurn = limit2Range(nTurn, nMinTurnPower, nMaxTurnPower);

	    //if ( bInTolerance )
	      //nTurn = 0;
    }
    SetDriveSidePower(RIGHT_SIDE, -nTurn); // drive the motors...
    SetDriveSidePower(LEFT_SIDE,   nTurn);

		nxtDisplayTextLine(4, "pwr:   %d", nTurn);
		nxtDisplayTextLine(5, "angle: %3.2f", fCurrAngle);
		nxtDisplayTextLine(6, "delay: %ld", Gyro.lTimeDelta);

    wait1Msec(Gyro.lTimeDelta+3); // cause the loop to run a tad slower than the gyro update task loop
  }


#ifdef ENABLE_NXT2EXCEL_DEBUGGING
  {
	  const int nSkidWaitTimeMsec = 1000;
	  wait1Msec(nSkidWaitTimeMsec);       // allow the bot time to skid to a stop...
	  float fSkidAngle = Gyro.fAngle;     // and record the final angle
		nxtDisplayTextLine(7, "skid: %3.2f", fSkidAngle);

	  NXT2excelAddFloat(fCurrAngle);
	  NXT2excelAddFloat(fSkidAngle);
	  NXT2excelAddLong(nAvgBatteryLevel);
	  NXT2excelAddLong(externalBatteryAvg);
	  NXT2excelAddInt(nMaxTurnPower);
	  NXT2excelAddInt(Gyro.nGyroLoc);
	  NXT2excelAddInt(nMinTurnPower);
	  NXT2excelAddFloat(fTurnAngleTolerance);
	  NXT2excelSendMessage();
  }
#endif

  wait1Msec(250);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// PRIVATE FUNCTION: Gyro controlled 90 degree clockwise turn using gradient power control
//                   *VERY* primative P control...gradient power control is from max to min
//
void PivotTurn_GradientPower(float degrees)
{
  const int PwrGradient = GRADIENT_TURN_MAX_PWR - GRADIENT_TURN_MIN_PWR;
  const float goal = degrees;

  float error = 0.0;
  int pwr = 0;
  int maxPwr = 0;

  if ( AutonomousIsAborted() )
    return;

  int nTurnTimeLimit = MSECS_90_DEGREES;  // should we scale this based on number of degrees? Okay unless over 90...

  eraseDisplay();

  ResetGyro();

  ClearCustomTimer(CT4);
  ClearTimer(T2);

  float currValue = Gyro.fAngle;

  while (abs(currValue) < abs(goal))
  {
    if ( time1[T2] > nTurnTimeLimit )
    {
      SetAbortAutonomous(true);
      break;
    }

    error = 1 - currValue/goal;
    pwr   = GRADIENT_TURN_MIN_PWR + ( error * PwrGradient );
    maxPwr = max2(pwr, maxPwr);

    if ( goal < 0 )
    {
      pwr *= -1;
    }

    SetDriveSidePower(LEFT_SIDE,   pwr);
    SetDriveSidePower(RIGHT_SIDE,  -pwr);

    currValue = Gyro.fAngle;
	}

  SetDrivePower(0);

#ifdef ENABLE_NXT2EXCEL_DEBUGGING
  {
	  int mSecs = CustomTimer(CT4);
	  wait1Msec(1000);
	  float skidValue = Gyro.fAngle;
	  float weightedAvgValue = Gyro.fAngleByWeightedAvg;
	  float trapApproxValue = Gyro.fAngleByTrapezoidalApprox;
	  int battry = externalBatteryAvg;

	  nxtDisplayTextLine(0, "Goal %3.2f", goal);
	  nxtDisplayTextLine(1, "bttry: %d", battry);
	  nxtDisplayTextLine(2, "mSecs: %d", mSecs);
	  nxtDisplayTextLine(3, "Angle: %3.2f", currValue);
	  nxtDisplayTextLine(4, "Skid:  %3.2f", skidValue);
	  nxtDisplayTextLine(5, "WtAvg: %3.2f", weightedAvgValue);
	  nxtDisplayTextLine(6, "Trap:  %3.2f", trapApproxValue);
	  nxtDisplayTextLine(7, "maxPwr: %d", maxPwr);
  }
#endif

  wait1Msec(250);
}


void SwingTurn_GradientPower(float degrees, tTurnDir turnDir, tMovementDir moveDir)
{
    const int PwrGradient = GRADIENT_TURN_MAX_PWR - GRADIENT_TURN_MIN_PWR;
    float goal = degrees;
    int pwrSign = 1;

    if ( turnDir == TURN_LEFT && moveDir == FORWARD )
    {
      goal *= -1;
    }
    if ( turnDir == TURN_LEFT && moveDir == BACKWARD )
    {
      pwrSign = -1;
    }
    if ( turnDir == TURN_RIGHT && moveDir == FORWARD )
    {
    }
    if ( turnDir == TURN_RIGHT && moveDir == BACKWARD )
    {
      goal *= -1;
      pwrSign = -1;
    }

	  float error = 0.0;
	  int pwr = 0;
	  int maxPwr = 100;

	  int leftPwr = 0;
	  int rightPwr = 0;

	  if ( AutonomousIsAborted() )
	    return;

	  int nTurnTimeLimit = MSECS_90_DEGREES;  // should we scale this based on number of degrees? Okay unless over 90...

	  eraseDisplay();

	  ResetGyro();

	  ClearCustomTimer(CT4);
	  ClearTimer(T2);

	  float currValue = Gyro.fAngle;

	  while (abs(currValue) < abs(goal))
	  {
	    if ( time1[T2] > nTurnTimeLimit )
	    {
	      SetAbortAutonomous(true);
	      break;
	    }

	    error = 1 - currValue/goal;
	    pwr   = GRADIENT_TURN_MIN_PWR + ( error * PwrGradient );
	    maxPwr = max2(pwr, maxPwr);

      if ( turnDir == TURN_RIGHT )
      {
        leftPwr = pwr * pwrSign;
        rightPwr = 0;
      }
      else
      {
        leftPwr = 0;
        rightPwr = pwr * pwrSign;
      }

	    SetDriveSidePower(LEFT_SIDE,   leftPwr);
	    SetDriveSidePower(RIGHT_SIDE,  rightPwr);

	    currValue = Gyro.fAngle;
	  }

  SetDrivePower(0);

#ifdef ENABLE_NXT2EXCEL_DEBUGGING
  {
	  int mSecs = CustomTimer(CT4);
	  wait1Msec(1000);
	  float skidValue = Gyro.fAngle;
	  float weightedAvgValue = Gyro.fAngleByWeightedAvg;
	  float trapApproxValue = Gyro.fAngleByTrapezoidalApprox;
	  int battry = externalBatteryAvg;

	  nxtDisplayTextLine(0, "Goal %3.2f", goal);
	  nxtDisplayTextLine(1, "bttry: %d", battry);
	  nxtDisplayTextLine(2, "mSecs: %d", mSecs);
	  nxtDisplayTextLine(3, "Angle: %3.2f", currValue);
	  nxtDisplayTextLine(4, "Skid:  %3.2f", skidValue);
	  nxtDisplayTextLine(5, "WtAvg: %3.2f", weightedAvgValue);
	  nxtDisplayTextLine(6, "Trap:  %3.2f", trapApproxValue);
	  nxtDisplayTextLine(7, "maxPwr: %d", maxPwr);
  }
#endif
}

#endif // DRIVE_CONTROL_PID_H
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Module: DriveControlPID.h
//
// Description:
//        Drive system interface handling all PID controlled drive commands
//
// Usage:
//
// Direct Motor Use:
//
// Direct Sensor Use:
//        Gyro for PID control
//
// Timer Use:
//        T2 - driving straight/turning timeouts (poor man's stall protection)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DRIVE_CONTROL_PID_H
#define DRIVE_CONTROL_PID_H

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
void PIDdriveStraight(float fSquares, int nSquareSize, int nCmdMaxPwr, TPowerRampingType tPwrRampType);

void PivotTurn_PID(int nNumDegrees);
void PivotTurn_GradientPower(float degrees);

void SwingTurn_GradientPower(float degrees, tTurnDir turnDir, tMovementDir moveDir);

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
// PRIVATE FUNCTION: PID drive control ... move the indicated number of sized sections
//
void PIDdriveStraight(float fSquares, int nSquareSize, int nCmdMaxPwr, TPowerRampingType tPwrRampType)
{
  if ( AutonomousIsAborted() )
    return;

  //const int   nRunTimeLimit = max(2000, round(MSECS_ONE_SQUARE * abs(fSquares)));
  long   nRunTimeLimit = max2(2000, round( abs(fSquares) * (-DRIVE_PID_TIME_SLOPE * nCmdMaxPwr + DRIVE_PID_TIME_Y_INTERCEPT)));
	nRunTimeLimit = 300000;
  const float fGoalDistance = fSquares * nSquareSize;

  const float fMinAdjust = DRIVE_PID_MIN_ADJ;
  const float fMaxAdjust = nCmdMaxPwr; // DRIVE_PID_MAX_ADJ;

  const int   nMaxPower = (fSquares >= 0) ? nCmdMaxPwr : -nCmdMaxPwr;

#ifdef PID_DRIVE_ERROR_ENCODER
poop
  //const float Kp = DRIVE_ENCODER_Kp;
  float Kp = DRIVE_ENCODER_Kp;

#elif defined(PID_DRIVE_ERROR_GYRO)
  //const float Kp = DRIVE_GYRO_Kp;
  float Kp = DRIVE_GYRO_Kp;

#else
  *** PID CONFIGURATION ERROR IN TEAMCONFIG.H ***
#endif

  const float Ki = DRIVE_ENCODER_Ki;
  const float Ki = DRIVE_ENCODER_Ki;
  const float Kd = DRIVE_ENCODER_Kd;

  int   nPwr;
  float fPwrRampScale = 1.0;

  float fError = 0.0;
  float fIntegral = 0.0;
  float fDerivative = 0.0;

  static float fPrevError = 0.0;

  float fAdjust = 0;

  eraseDisplay();

  ResetDriveEncoders();

  float fLeftEncoder = CurrMotorEncoder(LEFT_SIDE);
  float fRightEncoder = CurrMotorEncoder(RIGHT_SIDE);

  ClearTimer(T2);

  ResetGyro();

  while ( abs(fLeftEncoder) < abs(fGoalDistance) || abs(fRightEncoder) < abs(fGoalDistance) )
  {
    if ( time1[T2] > nRunTimeLimit )
    {
      SetAbortAutonomous(true);
      SetDrivePower(0);
      break;
    }

#ifdef PID_DRIVE_ERROR_ENCODER
poop
    fError = fLeftEncoder - fRightEncoder;
#elif defined(PID_DRIVE_ERROR_GYRO)
    fError = Gyro.fAngle * PID_DRIVE_ERROR_GYRO_SIGN;
#else
  *** PID CONFIGURATION ERROR IN TEAMCONFIG.H ***
#endif

    fIntegral += fError;
    fDerivative = fError - fPrevError;
    fPrevError = fError;

    fAdjust = (fError*Kp) + (fIntegral*Ki) + (fDerivative*Kd);
    fAdjust = limit2Range(fAdjust, fMinAdjust, fMaxAdjust);

    // Allow power to scale up/down in a smooth gradient so we're not immediately applying max power.
    // Goal is to avoid spinning out when starting or skidding when stopping.
	  if ( tPwrRampType == COS_CURVE_PWR_RAMP )
	  {
	    fPwrRampScale = CosPowerCurveForRampedPower(fGoalDistance, fLeftEncoder, fRightEncoder);
	  }
	  else
	  {
	    fPwrRampScale = 1.0; // just simply apply the max power with no ramping...
	  }

    nPwr = nMaxPower * fPwrRampScale;
    fAdjust *= fPwrRampScale;

    SetDriveSidePower(RIGHT_SIDE, nPwr + fAdjust);
    SetDriveSidePower(LEFT_SIDE,  nPwr - fAdjust);

    wait1Msec(5);


	  nxtDisplayTextLine(0, "pwr:   %d",    nPwr);
	  nxtDisplayTextLine(1, "goal:  %3.2f", fGoalDistance);
	  nxtDisplayTextLine(2, "error: %3.2f", fError);
		nxtDisplayTextLine(3, "adj:   %3.2f", fAdjust);
		nxtDisplayTextLine(4, "left:  %6.0f", fLeftEncoder);
		nxtDisplayTextLine(5, "right: %6.0f", fRightEncoder);
		// the power ramp functions will add debug information on lines 5-7

    fLeftEncoder  = CurrMotorEncoder(LEFT_SIDE);
    fRightEncoder = CurrMotorEncoder(RIGHT_SIDE);

    // compensate for a single dead encoder by ignoring it and only using the other
    if ( fLeftEncoder == 0 && fRightEncoder != 0 )
      fLeftEncoder = fRightEncoder;
    else if ( fRightEncoder == 0 && fLeftEncoder != 0 )
      fRightEncoder = fLeftEncoder;
  }

  SetDrivePower(0);

  wait1Msec(250);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
//
// PRIVATE FUNCTION: PID controlled pivot turn
//
void PivotTurn_PID(int nNumDegrees)
{
  if ( AutonomousIsAborted() )
    return;

  const float Kp = PIVOT_Kp;
  const float Ki = PIVOT_Ki;
  const float Kd = PIVOT_Kd;

  const int   nTurnTimeLimit = MSECS_1_DEGREE * abs(nNumDegrees);

  const float fGoalAngle = nNumDegrees;
  const int   nMaxTurnPower = PIVOT_PID_MAX_PWR;

  const int   nMinTurnPower = PIVOT_PID_MIN_PWR;
  const float fTurnAngleTolerance = PIVOT_PID_ANGLE_TOLERANCE;

  float fError = 0.0;
  float fIntegral = 0.0;
  float fDerivative = 0.0;

  int    nTurn = 0;
  float  fCurrAngle = 0.0;
  static float fPrevError = 0.0;

  bool   bAtGoalAngle = false;
  bool   bInTolerance = false;

  ResetGyro(); // All of our turns are relative heading turns

  ClearTimer(T2);

  while (bAtGoalAngle == false )
  {
    if ( time1[T2] > nTurnTimeLimit )
    {
	    //SetAbortAutonomous(true);
      SetDrivePower(0);
      break;
    }

    if ( bInTolerance )
    {
      nTurn = 0;             // When we've been in tolerance long enough, then
      bAtGoalAngle = true;   // stop the motors and set the "we're done" flag
    }

    else                     // Not ready yet, keep adjusting
    {
	    fCurrAngle = Gyro.fAngle;
	    fError = fGoalAngle - fCurrAngle;

	    bInTolerance = (abs(fError) < fTurnAngleTolerance) || (abs(fCurrAngle) > abs(fGoalAngle));

	    fIntegral += fError;
	    fDerivative = fError - fPrevError;
      fPrevError = fError;

      nTurn = (int)(fError*Kp) + (int)(fIntegral*Ki) + (int)(fDerivative*Kd);
      nTurn = limit2Range(nTurn, nMinTurnPower, nMaxTurnPower);

	    //if ( bInTolerance )
	      //nTurn = 0;
    }
    SetDriveSidePower(RIGHT_SIDE, -nTurn); // drive the motors...
    SetDriveSidePower(LEFT_SIDE,   nTurn);

		nxtDisplayTextLine(4, "pwr:   %d", nTurn);
		nxtDisplayTextLine(5, "angle: %3.2f", fCurrAngle);
		nxtDisplayTextLine(6, "delay: %ld", Gyro.lTimeDelta);

    wait1Msec(Gyro.lTimeDelta+3); // cause the loop to run a tad slower than the gyro update task loop
  }


#ifdef ENABLE_NXT2EXCEL_DEBUGGING
  {
	  const int nSkidWaitTimeMsec = 1000;
	  wait1Msec(nSkidWaitTimeMsec);       // allow the bot time to skid to a stop...
	  float fSkidAngle = Gyro.fAngle;     // and record the final angle
		nxtDisplayTextLine(7, "skid: %3.2f", fSkidAngle);

	  NXT2excelAddFloat(fCurrAngle);
	  NXT2excelAddFloat(fSkidAngle);
	  NXT2excelAddLong(nAvgBatteryLevel);
	  NXT2excelAddLong(externalBatteryAvg);
	  NXT2excelAddInt(nMaxTurnPower);
	  NXT2excelAddInt(Gyro.nGyroLoc);
	  NXT2excelAddInt(nMinTurnPower);
	  NXT2excelAddFloat(fTurnAngleTolerance);
	  NXT2excelSendMessage();
  }
#endif

  wait1Msec(250);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// PRIVATE FUNCTION: Gyro controlled 90 degree clockwise turn using gradient power control
//                   *VERY* primative P control...gradient power control is from max to min
//
void PivotTurn_GradientPower(float degrees)
{
  const int PwrGradient = GRADIENT_TURN_MAX_PWR - GRADIENT_TURN_MIN_PWR;
  const float goal = degrees;

  float error = 0.0;
  int pwr = 0;
  int maxPwr = 0;

  if ( AutonomousIsAborted() )
    return;

  int nTurnTimeLimit = MSECS_90_DEGREES;  // should we scale this based on number of degrees? Okay unless over 90...

  eraseDisplay();

  ResetGyro();

  ClearCustomTimer(CT4);
  ClearTimer(T2);

  float currValue = Gyro.fAngle;

  while (abs(currValue) < abs(goal))
  {
    if ( time1[T2] > nTurnTimeLimit )
    {
      SetAbortAutonomous(true);
      break;
    }

    error = 1 - currValue/goal;
    pwr   = GRADIENT_TURN_MIN_PWR + ( error * PwrGradient );
    maxPwr = max2(pwr, maxPwr);

    if ( goal < 0 )
    {
      pwr *= -1;
    }

    SetDriveSidePower(LEFT_SIDE,   pwr);
    SetDriveSidePower(RIGHT_SIDE,  -pwr);

    currValue = Gyro.fAngle;
	}

  SetDrivePower(0);

#ifdef ENABLE_NXT2EXCEL_DEBUGGING
  {
	  int mSecs = CustomTimer(CT4);
	  wait1Msec(1000);
	  float skidValue = Gyro.fAngle;
	  float weightedAvgValue = Gyro.fAngleByWeightedAvg;
	  float trapApproxValue = Gyro.fAngleByTrapezoidalApprox;
	  int battry = externalBatteryAvg;

	  nxtDisplayTextLine(0, "Goal %3.2f", goal);
	  nxtDisplayTextLine(1, "bttry: %d", battry);
	  nxtDisplayTextLine(2, "mSecs: %d", mSecs);
	  nxtDisplayTextLine(3, "Angle: %3.2f", currValue);
	  nxtDisplayTextLine(4, "Skid:  %3.2f", skidValue);
	  nxtDisplayTextLine(5, "WtAvg: %3.2f", weightedAvgValue);
	  nxtDisplayTextLine(6, "Trap:  %3.2f", trapApproxValue);
	  nxtDisplayTextLine(7, "maxPwr: %d", maxPwr);
  }
#endif

  wait1Msec(250);
}


void SwingTurn_GradientPower(float degrees, tTurnDir turnDir, tMovementDir moveDir)
{
    const int PwrGradient = GRADIENT_TURN_MAX_PWR - GRADIENT_TURN_MIN_PWR;
    float goal = degrees;
    int pwrSign = 1;

    if ( turnDir == TURN_LEFT && moveDir == FORWARD )
    {
      goal *= -1;
    }
    if ( turnDir == TURN_LEFT && moveDir == BACKWARD )
    {
      pwrSign = -1;
    }
    if ( turnDir == TURN_RIGHT && moveDir == FORWARD )
    {
    }
    if ( turnDir == TURN_RIGHT && moveDir == BACKWARD )
    {
      goal *= -1;
      pwrSign = -1;
    }

	  float error = 0.0;
	  int pwr = 0;
	  int maxPwr = 100;

	  int leftPwr = 0;
	  int rightPwr = 0;

	  if ( AutonomousIsAborted() )
	    return;

	  int nTurnTimeLimit = MSECS_90_DEGREES;  // should we scale this based on number of degrees? Okay unless over 90...

	  eraseDisplay();

	  ResetGyro();

	  ClearCustomTimer(CT4);
	  ClearTimer(T2);

	  float currValue = Gyro.fAngle;

	  while (abs(currValue) < abs(goal))
	  {
	    if ( time1[T2] > nTurnTimeLimit )
	    {
	      SetAbortAutonomous(true);
	      break;
	    }

	    error = 1 - currValue/goal;
	    pwr   = GRADIENT_TURN_MIN_PWR + ( error * PwrGradient );
	    maxPwr = max2(pwr, maxPwr);

      if ( turnDir == TURN_RIGHT )
      {
        leftPwr = pwr * pwrSign;
        rightPwr = 0;
      }
      else
      {
        leftPwr = 0;
        rightPwr = pwr * pwrSign;
      }

	    SetDriveSidePower(LEFT_SIDE,   leftPwr);
	    SetDriveSidePower(RIGHT_SIDE,  rightPwr);

	    currValue = Gyro.fAngle;
	  }

  SetDrivePower(0);

#ifdef ENABLE_NXT2EXCEL_DEBUGGING
  {
	  int mSecs = CustomTimer(CT4);
	  wait1Msec(1000);
	  float skidValue = Gyro.fAngle;
	  float weightedAvgValue = Gyro.fAngleByWeightedAvg;
	  float trapApproxValue = Gyro.fAngleByTrapezoidalApprox;
	  int battry = externalBatteryAvg;

	  nxtDisplayTextLine(0, "Goal %3.2f", goal);
	  nxtDisplayTextLine(1, "bttry: %d", battry);
	  nxtDisplayTextLine(2, "mSecs: %d", mSecs);
	  nxtDisplayTextLine(3, "Angle: %3.2f", currValue);
	  nxtDisplayTextLine(4, "Skid:  %3.2f", skidValue);
	  nxtDisplayTextLine(5, "WtAvg: %3.2f", weightedAvgValue);
	  nxtDisplayTextLine(6, "Trap:  %3.2f", trapApproxValue);
	  nxtDisplayTextLine(7, "maxPwr: %d", maxPwr);
  }
#endif
}

#endif // DRIVE_CONTROL_PID_H
