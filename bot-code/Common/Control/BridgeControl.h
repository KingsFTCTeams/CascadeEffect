/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Module: BridgeControl.h
//
// Description:
//        Bridge control interface - encapsulates all manipulation of the bridge and agitator
//
// Usage:
//        nothing special...
//
// Direct Motor Use:
//        RightBridge - bridge's right side elevation servo (srvo_S1_C3_1)
//        LeftBridge  - bridge's left side elevation servo  (srvo_S1_C3_2)
//        motorC      - baton agitator                      (NXT motor C)
//
// Direct Sensor Use:
//        none
//
// Timer Use:
//        T1 - used to time agitator when actually dispensing batons
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BRIDGE_CONTROL_H
#define BRIDGE_CONTROL_H

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
typedef enum
{
  APPROACH_HIGH,
  APPROACH_MID,
  APPROACH_LOW,

  DISPENSE_HIGH,
  DISPENSE_MID,
  DISPENSE_LOW,

  BRIDGE_UP

} tBridgePosition;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private Global Variables
//
bool High = false;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public Function Declarations
//
void SelectHeight();              // Asks user which height to dispense batons from - WHERE IS THIS USED?
void SetBridge(tBridgePosition position); // Set servo to preset positions ( up, high, med, low )

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
// PUBLIC FUNCTION: Move the bridge to the indicated setting (low, medium, high)
//
void SetBridge(tBridgePosition position)
{
  if ( AutonomousIsAborted() )
    return;

  int servoAngle = BRIDGE_UP_ANGLE; // BRIDGE_UP servo position

  if ( position == APPROACH_HIGH )           //
    servoAngle = BRIDGE_APPROACH_HIGH_ANGLE; //
                                             //
  else if ( position == APPROACH_MID )       //  Bridge "dispenser approach/alignment" level presets
    servoAngle = BRIDGE_APPROACH_MID_ANGLE;  //
                                             //
  else if ( position == APPROACH_LOW )       //
    servoAngle = BRIDGE_APPROACH_LOW_ANGLE;  //

  else if ( position == DISPENSE_HIGH )      //
    servoAngle = BRIDGE_DISPENSE_HIGH_ANGLE; //
                                             //
  else if ( position == DISPENSE_MID )       // Bridge "dispense batons" level presets
    servoAngle = BRIDGE_DISPENSE_MID_ANGLE;  //
                                             //
  else if ( position == DISPENSE_LOW )       //
    servoAngle = BRIDGE_DISPENSE_LOW_ANGLE;  //

  servo[RightBridge] = RIGHT_BRIDGE_UP_ANGLE_CALC(servoAngle);
  servo[LeftBridge]  = LEFT_BRIDGE_UP_ANGLE_CALC(servoAngle);

  wait1Msec(750);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Rotate the agitator one rotation
//
void RotateAgitatorOnce()
{
  if ( AutonomousIsAborted() )
  {
    motor[motorC] = 0;      // force the motors to stop
    return;
  }

	nMotorEncoder[motorC] = 0;

	motor[motorC] = 100;

	time1[T1] = 0;

	while ( nMotorEncoder[motorC] < AGITATOR_ROTATION_ENCODER_COUNT && time1[T1] < AGITATOR_ROTATION_MSECS )
	{
	}

	motor[motorC] = 0;

	wait1Msec(AGITATOR_DONE_DELAY_MSECS);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Ask the user which bridge height setting to use
//
void SelectHeight()
{
  nxtDisplayCenteredBigTextLine(2, "Select");
  nxtDisplayCenteredBigTextLine(4, "Height");

  wait1Msec(1500);

  eraseDisplay();

  nxtDisplayCenteredBigTextLine(3, "LOW?");

  while ( nNxtButtonPressed != 3 )
  {
    if ( nNxtButtonPressed == 1)
    {
      High = true;
    }
    else if ( nNxtButtonPressed == 2 )
    {
      High = false;
    }

    if ( High == true )
    {
      nxtDisplayCenteredBigTextLine(3, "HIGH?");
    }
    else
    {
      nxtDisplayCenteredBigTextLine(3, "LOW?");
    }

    wait1Msec(25);
    eraseDisplay();
  }

  eraseDisplay();

  nxtDisplayCenteredBigTextLine(3, "CONFIRMED");

  wait1Msec(1000);

  eraseDisplay();
}

#endif // BRIDGE_CONTROL_H
