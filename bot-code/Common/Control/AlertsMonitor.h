/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Module: AbortMonitor.h
//
// Description:
//        Detect when autonomous has been aborted and provide alert
//
// Usage:
//        1) Start the stall task
//
// Direct Motor Use:
//        none
//
// Direct Sensor Use:
//        none
//
// Timer Use:
//        none
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ABORT_MONITOR_H
#define ABORT_MONITOR_H

#pragma systemFile            // this eliminates warning for "unreferenced" functions

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef INDICATOR_H
#include "Indicator.h"
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// #defines
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public Global Variables
//
bool bLightToggle = false;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private Global Variables
//
const int nAlertMonitorDelay = 50;   // loop cycle time

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public Function Declarations
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private Function Declarations
//
void FlashLightsAlternating(int mSecInterval);
void FlashLightsTogether(int mSecInterval);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Task Definitions
//
task AlertsMonitorTask()
{
  bool bLightToggle = false;

  while ( true )
  {
    // if aborted flash the lights alternating left/right on a 1 second cycle.
	  if ( AutonomousIsAborted() )
	  {
	    FlashLightsAlternating(1000);
	  }

    // if have lost connection to the FCS, flast both lights on/off together on a 1/10 second cycle.
	  if ( !HaveFCSconnection() )
	  {
	    FlashLightsTogether(100);
	  }

    // if the drive system is stalled, turn off the lights until it comes out of stall
    else if ( DriveIsStalled() )
    {
      LightsOff();
    }

#ifdef ENABLE_COLLECTION_ALERTS
    // if the collection system has indicated a fault, flash lights alternating on 1 sec cycle
    else if ( CollectionFailure() )
    {
	    FlashLightsAlternating(1000);
    }
#endif

#ifdef ENABLE_PAYLOAD_ALERTS
    else if ( PayloadIsAtLimit1() || PayloadIsAtLimit2() )
    {
      LightsOn();
    }
#endif

#ifdef ENABLE_SPECIAL_RESOURCE_ALERT
    else if ( SpecialResourceDetected() )
    {
    }
#endif

    // otherwise, default state is determined by team config
    // ... make this more flexible we need to move the alert handlers to a team specific module similar to MotorControl.h
    else
    {
      if ( ALERTS_DEFAULT_STATE_ON )
      {
        LightsOn();
      }
      else
      {
        LightsOff();
      }
    }

    wait1Msec(nAlertMonitorDelay);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Function Definitions
//

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: check if the indicated drive side has been flagged as stalled
//

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: store the power currently being applied to the indicated drive side.
//

/////////////////////////////////////////////////////////////////////////////////////////
//
// PRIVATE FUNCTION: flash left and right lights in patterns
//
void FlashLightsAlternating(int mSecInterval)
{
  if ( bLightToggle )
  {
    LeftLightOn();
    RightLightOff();
  }
  else
  {
    LeftLightOff();
    RightLightOn();
  }
  bLightToggle = !bLightToggle;

  long nSleepTime = absValue(mSecInterval-nAlertMonitorDelay);
  long nStartTime = nPgmTime;
  while ( nSleepTime > 0 && (nPgmTime - nStartTime) < nSleepTime )
		wait1Msec(1);
}

void FlashLightsTogether(int mSecInterval)
{
  if ( bLightToggle )
  {
    LightsOn();
  }
  else
  {
    LightsOff();
  }
  bLightToggle = !bLightToggle;

  long nSleepTime = absValue(100 - nAlertMonitorDelay);
  long nStartTime = nPgmTime;
  while ( nSleepTime > 0 && (nPgmTime - nStartTime) < nSleepTime )
		wait1Msec(1);
}


#endif // ABORT_MONITOR_H
