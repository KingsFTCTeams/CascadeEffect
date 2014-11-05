/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Module: DriveStallProtection.h
//
// Description:
//        Provide the drive system stall protection:
//
//          1) "Poor man's" stall protection:
//              * simple timeouts of basic movement actions (drive straight X squares or turn 90 degrees)
//              * timeouts are based on tested averages
//              * triggering causes autonomous to abort completely
//
//          2) "Real" stall protection:
//              * based on an expected number of encoder ticks for the recent time slice
//              * compiled in/out with the ENABLE_DRIVE_STALL_PROTECTION #define label
//              * only used when a drive command asks for it
//              * stall protection waits for the drive system to ramp up (using CTstall)
//              * if stall is detected, hold the stall flag for a short period of time...it is up to the
//                rest of the drive system to decide what to do...normally this means not applying power
//                to the drive motors while the stall flag is held
//
// Usage:
//      "Poor man's" stall protection:
//        0) always enabled...
//      "Real" stall protection:
//        1) Compile with ENABLE_DRIVE_STALL_PROTECTION defined
//        2) Call InitDriveStallProtection()
//        3) Call BeginDriveStallProtection() to start using stall protection
//        4) Call EndDriveStallProtection() to stop using stall protection
//        5) Call DriveIsStalled() to determine if the drive system is stalled
//
// Direct Motor Use:
//        none
//
// Direct Sensor Use:
//        none
//
// Timer Use:
//        T2 -  times how long the drive command has been running
//        CTstall -  times how long it has been since power was applied to determine if drive system is ramped up
//        CT1 - times how long hold the stall flag true
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DRIVE_STALL_PROTECT_H
#define DRIVE_STALL_PROTECT_H

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
// For stall detection, minimum encoder ticks is derived as follows:
//   * We've empirically determined that in autonomous the bot averages 810mSecs per tile (to travel 2')
//   * 2' in 810mSecs = 1800 ticks/sec or 1.8 ticks/mSec
//   * Anything below 50% of expected turns for the interval is viewed as stalled
//
const float fExpectedTicksPerMsec = EXPECTED_TICKS_SCALAR * (1440 / TESTED_SECS_ONE_SQUARE) / 1000; // expected encoder ticks per msec (75% of normal)

bool bUsingDriveStallProtection = false;  // set to true to check for drive stall

bool bDriveIsStalled = false;             // true if either drive side is stalled

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public Function Declarations
//
void CheckIfDriveIsStalled(tDriveSide nDriveSide); // check if a stall condition exists and raise the stalled flag if it is

#define DriveIsStalled() ( bDriveIsStalled )

void BeginDriveStallProtection();
#define EndDriveStallProtection() { bUsingDriveStallProtection = false; }

#define InitDriveStallProtection()    \
{                                     \
  bUsingDriveStallProtection = false; \
  bDriveIsStalled = false;            \
}

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
// PUBLIC FUNCTION: begin stall protection
//
void BeginDriveStallProtection()
{
  bUsingDriveStallProtection = true;

#ifdef ENABLE_DRIVE_STALL_PROTECTION
  ClearCustomTimer(CTstall); // CTstall is used to determine if the drive system has ramped up to speed
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Determine if the drive system is stalled
//
//    Only check for stall if:
//      1) the drive system has asked for it,
//      2) applying power to the drive system,
//      3) the drive system has had time to ramp up to speed
//
//    Once stalled, hold the stall condition for a short period of time (such as 50 mSecs)
//
//    Uses timer T2 to time sampling intervals
//    Uses timer CTstall to time drive system ramp up (drive getting up to speed once power is applied)
//    Uses custom timer CT1 to time how long hold the stall flag true
//
void CheckIfDriveIsStalled(tDriveSide nDriveSide)
{
#ifndef ENABLE_DRIVE_STALL_PROTECTION

  bDriveIsStalled = false; // behave nicely if stall protection isn't being used

#else // ENABLE_DRIVE_STALL_PROTECTION is defined	/////////////////////////////////////////////////////////////

  // only check for stall if the drive system has asked for it and the drive system is powered and up to speed
  if ( !bUsingDriveStallProtection || CurrMotorPwr(nDriveSide) == 0 || CustomTimer(CTstall) < DRIVE_RAMP_UP_DUR )
    return;

  // Get current time (how long the drive command has been running) and current encoder reading
	long nCurrTime = time1[T2];                       // these were reset when
  long nCurrEncoder = CurrMotorEncoder(nDriveSide); // ...the drive command started

  // If currently stalled, hold stall for a "little while"
  if ( bDriveIsStalled && CustomTimer(CT1) > MSECS_TO_HOLD_STALL )
  {
    bDriveIsStalled = false;   // time is up, release stall condition so can be checked again
    ClearCustomTimer(CTstall); // restart the motor ramp up timer...the motors will be starting back up
  }

  // If not currently stalled and the drive system is up-to-speed, check if we're now stalled
  else if ( bDriveIsStalled == false )
  {
		static long nPrevTime = 0;
		static int  nPrevEncoder = 0;

		long nTimeDelta = CalcTimeDeltaWithRollOver(nCurrTime, nPrevTime);
		nPrevTime = nCurrTime;

		int nActualEncoderDelta = nCurrEncoder - nPrevEncoder;
		nPrevEncoder = nCurrEncoder;

		float fExpectedEncoderDelta = fExpectedTicksPerMsec * nTimeDelta;

		if ( nActualEncoderDelta < fExpectedEncoderDelta )
		{
		  bDriveIsStalled = true;
		  ClearCustomTimer(CT1);    // restart the "hold the stall flag" timer
		}
  }

#endif // ENABLE_DRIVE_STALL_PROTECTION ////////////////////////////////////////////////////////////////

}

#endif // DRIVE_STALL_PROTECT_H
