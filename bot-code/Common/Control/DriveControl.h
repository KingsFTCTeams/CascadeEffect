/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Module: DriveControl.h
//
// Description:
//        Drive system interface handling all logical drive commands and direct drive system motor control
//
// Usage:
//        1) Start the stall task
//        2) Call IsStalled() to determine if a drive side is stalled
//
// Direct Motor Use:
//        All direct drive motor code (including encoders) is in the team specific MotorControl.h module
//
// Direct Sensor Use:
//
// Timer Use:
//        none
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DRIVE_CONTROL_H
#define DRIVE_CONTROL_H

#pragma systemFile            // this eliminates warning for "unreferenced" functions

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "PowerRamping.h"

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
#define InitDriveSystem() { InitDriveStallProtection(); }

void PivotTurn(int nNumDegrees);     // turn the indicated number of degrees
void SwingTurn(float degrees, tTurnDir turnDir, tMovementDir moveDir);

void DriveStraight(float fDistInTiles); // drive straight doing our best to actually go in a straight line
void DriveStraight(float fDistInTiles, int nMotorPower);
void DriveStraight(float fDistInTiles, int nMotorPower, TPowerRampingType tPwrRampType);

void SetDrivePower(int nMotorPwr); // Set power to the entire drive system ... no attempt made to drive straight...
void SetDriveSidePower(tDriveSide nDriveSide, int nMotorPwr); // Set power to the indicated side of the drive system

long CurrMotorEncoder(tDriveSide nDriveSide);
void ResetDriveEncoders();

//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "DriveStallProtect.h"
#include "DriveControlPID.h"
#include "DriveControlEncoder.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private Function Declarations
//

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
// PUBLIC FUNCTION: Turn control ... turn indicated number of degrees
//
void PivotTurn(int nNumDegrees)
{
#if defined(ENABLE_GYRO_GRADIENT_PWR_PIVOT_CONTROL)
  PivotTurn_GradientPower(nNumDegrees);
#elif defined(ENABLE_GYRO_PID_PIVOT_CONTROL)
  PivotTurn_PID(nNumDegrees);
#else
  nxtDisplayTextLine(1, "CONFIG ERROR");
  nxtDisplayTextLine(3, "Turn(x) with");
  nxtDisplayTextLine(5, "Invalid turn method");
  nxtDisplayTextLine(6, "Check teamConfig.h");
  SetAbortAutonomous(true);
#endif
}

void SwingTurn(float degrees, tTurnDir turnDir, tMovementDir moveDir)
{
  SwingTurn_GradientPower(degrees, turnDir, moveDir);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: drive control ... move the indicated number of foam tiles
//
void DriveStraight(float fDistInTiles)
{
	DriveStraight(fDistInTiles, DRIVE_PID_BASE_PWR);
}

void DriveStraight(float fDistInTiles, int nMotorPower)
{
	DriveStraight(fDistInTiles, nMotorPower, COS_CURVE_PWR_RAMP);
}

void DriveStraight(float fDistInTiles, int nMotorPower, TPowerRampingType tPwrRampType)
{
  BeginDriveStallProtection();

	#ifdef ENABLE_PID_DRIVE_CONTROL
	  PIDdriveStraight(fDistInTiles, ONE_SQUARE_ENCODER_COUNT, nMotorPower, tPwrRampType);
	#else
	  EncoderDriveStraight(fDistInTiles, tPwrRampType);
	#endif

  EndDriveStallProtection();
}

void SetDrivePower(int nMotorPwr)
{
	SetDriveSidePower(RIGHT_SIDE, nMotorPwr); SetDriveSidePower(LEFT_SIDE, nMotorPwr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DIRECT MOTOR CONTROL ... These functions encapsulate the drive motors.
//                          NO OTHER CODE SHOULD DIRECTLY CHANGE MOTOR SETTING!!!
//

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Set motor power for indicated side (left or right) of the drive system
//
// Called ***ONLY*** by the main task
// ...NO OTHER TASK SHOULD EVER CALL THIS
// ...THIS IS THE ONLY FUNCTION THAT SHOULD EVER DIRECTLY APPLY POWER TO THE DRIVE MOTORS
//
void SetDriveSidePower(tDriveSide nDriveSide, int nMotorPwr)
{
	int pwr = nMotorPwr; // Assume we're not stalled or aborting

  CheckIfDriveIsStalled(nDriveSide);

  // adjust power based on current battery voltage
  float fVoltError = (externalBatteryAvg / 1000) - BASE_VOLTAGE;
  pwr -= fVoltError * VOLTAGE_ADJUST * pwr;

	if ( AutonomousIsAborted() || DriveIsStalled() )
	{
    pwr = 0;           // Stalled or aborting...remove power until told otherwise...
  }

  // ADD PWM HERE???

  ApplyMotorPower(nDriveSide, pwr);

}

long CurrMotorEncoder(tDriveSide nDriveSide)
{
	return ( (nDriveSide == LEFT_SIDE) ? LeftDriveEncoderVal() : RightDriveEncoderVal() );
}

void ResetDriveEncoders()
{
	TellEncoderTaskResetDriveEncoders();
}

#endif // DRIVE_CONTROL_H
