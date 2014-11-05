/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Module: Dispensing.h
//
// Description:
//        Encapsulates all operations associated with dispensing batons
//
// Usage:
//        nothing special
//
// Direct Motor Use:
//
// Direct Sensor Use:
//        IR seeker (to home in on the center dispensor)
//        US sensor (to position the bot the proper distance from the wall when at a dispensor)
//
// Timer Use:
//        T1 - used to timeout attempts to obtain the proper distance from the wall when preparing to dispense
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DISPENSING_H
#define DISPENSING_H

#pragma systemFile            // this eliminates warning for "unreferenced" functions

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Includes
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// #defines
//
typedef enum
{
  LOW_DISPENSER,
  MID_DISPENSER,
  HIGH_DISPENSER

} tDispenserID;
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
void ScoreBatons(tArmRotation nSide);
void DispenseXBatons( int batons );
void TrackIR();
void AlignWithDispenserID(tDispenserID DispenserID);

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
// PUBLIC FUNCTION: Slide the bot 1 inch to either side to better align with the dispenser
//
//      45-45-90 triangle is 1-1-sqrt(2) for leg-leg-hypotenuse
//      leg: 1/24 = 0.04166 of a full square for the 45-45-90 leg
//      hyp: sqrt(2)/24 = 0.0589 of a full square for the 45-45-90 hypotenuse
//
#define SHIFT_HYP 0.0589
#define SHIFT_LEG 0.04166

#define ShiftLeft()                   \
{                                     \
  SetDriveSidePower(RIGHT_SIDE, 75);  \
  wait1Msec(1000);                    \
  SetDrivePower(0);                   \
  SetDriveSidePower(LEFT_SIDE, 75);   \
  wait1Msec(800);                     \
  SetDrivePower(0);                   \
  DriveXSquares(-.25);                \
}

#define ShiftRight()                  \
{                                     \
  SetDriveSidePower(LEFT_SIDE, 75);   \
  wait1Msec(1000);                    \
  SetDrivePower(0);                   \
  SetDriveSidePower(RIGHT_SIDE, 75);  \
  wait1Msec(800);                     \
  SetDrivePower(0);                   \
  DriveXSquares(-.25);                \
}


#define AdjustForIR()                 \
{                                     \
  if ( error > 0 )                    \
  {                                   \
    ShiftRight();                     \
  }                                   \
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Score batons into the rolling goal on the indicated side of the bot.
//                  When tiling the scoop, the rolling goal will be forward of the arm
//                  so tilt the scoop in the opposite direction the arm turned.
//
void ScoreBatons(tArmRotation nSide)
{
  SetArmElevation(RAISED);

  RotateArm(nSide);

  tScoopPosition nRotateDir = (nSide == LEFT_GOAL) ? TILT_RIGHT : TILT_LEFT;
  RotateScoop(nRotateDir); // tilt toward front of bot

  SetArmElevation(LOWERED);

  SetScoringShields(OPENED);

  wait1Msec(1000); // allow time for the batons to fall out

  SetArmElevation(RAISED);

  SetScoringShields(CLOSED);

  RotateScoop(LEVEL);

  RotateArm(CENTERED);

  RotateArm(CENTERED);

  RotateArm(CENTERED);

  SetArmElevation(LOWERED);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Dispense the indicated number of batons
//
void DispenseXBatons( int batons )
{
  for ( int i = 0; i < batons && !DetectedMagnet() ; i++ )
  {
    RotateAgitatorOnce();
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Locate/home in on the center dispenser using the IR beacon
//
void TrackIR()
{
  int IR1_Val = IR1SensorVal();
  int IR2_Val = IR2SensorVal();   // Update IR values
  int IRSum = IR1_Val + IR2_Val;
  int error = IRSum - TRACK_IR_TARGET;
  int lPwr, rPwr;

  //AdjustForIR();

	ClearTimer(T1);                    // Reset operation expiration timer

  while ( USsensorVal() > TRACK_IR_MINIMUM_DISTANCE )
  {
    IR1_Val = IR1SensorVal();
    IR2_Val = IR2SensorVal();   // Update IR values
    IRSum = IR1_Val + IR2_Val;
    error = IRSum - TRACK_IR_TARGET;

    // assume going straight, and change to turn if necessary
    lPwr = rPwr = TRACK_IR_SPEED;

    // possible improvement: different turn if 1 off, 2, off, etc ... simple proportional
    if ( error > 1 )
    {
      lPwr =  TRACK_IR_TWO_OFF_LEFT_TURN_SPEED;
      rPwr = -TRACK_IR_TWO_OFF_RIGHT_TURN_SPEED;
    }
    else if ( error == 1 )
    {
      lPwr =  TRACK_IR_ONE_OFF_LEFT_TURN_SPEED;
      rPwr = 0;//-TRACK_IR_ONE_OFF_RIGHT_TURN_SPEED;
    }
    else if ( error == -1 )
    {
      lPwr = 0;//-TRACK_IR_ONE_OFF_LEFT_TURN_SPEED;
      rPwr =  TRACK_IR_ONE_OFF_RIGHT_TURN_SPEED;
    }
    else if ( error < -1 )
    {
      lPwr = -TRACK_IR_TWO_OFF_LEFT_TURN_SPEED;
      rPwr =  TRACK_IR_TWO_OFF_RIGHT_TURN_SPEED;
    }
    else
    {
      PlaySound(soundBlip);
    }

    SetDriveSidePower(LEFT_SIDE, lPwr);
    SetDriveSidePower(RIGHT_SIDE, rPwr);

    // "Stutter" in to give finer approach control
    if ( TRACK_IR_STUTTER_PWR_MSECS > 0 && lPwr != rPwr )
    {
	    wait1Msec(TRACK_IR_STUTTER_PWR_MSECS);
	    SetDrivePower(0);
	    wait1Msec(TRACK_IR_STUTTER_WAIT_MSECS);
	  }

    if ( time1[T1] > TRACK_IR_TIMEOUT )
    {
      SetAbortAutonomous(true);
      break;
    }

  }

  PlaySound(soundBeepBeep);

  SetDrivePower(0);

  wait1Msec(250);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Move the the proper distance from the dispenser so can dispense
//
#define AlignWithDispenser() { AlignWithDispenserID(MID_DISPENSER); }

void AlignWithDispenserID(tDispenserID DispenserID)
{
	  float pwr = 0;
	  int nTargetDistance = ALIGN_DISPENSER_MID_TGT_DISTANCE;
	  int nStutterDistance = nTargetDistance + ALIGN_DISPENSER_STUTTER_DIST;

	  int error = ALIGN_DISPENSER_TOLERANCE + 1;
	  bool bAtGoal = false;

	  bDisplayDiagnostics = false;
	  eraseDisplay();

	  if ( DispenserID == LOW_DISPENSER )
	  {
	    nTargetDistance = ALIGN_DISPENSER_LOW_TGT_DISTANCE;
	  }
	  else if ( DispenserID == HIGH_DISPENSER )
	  {
	    nTargetDistance = ALIGN_DISPENSER_HIGH_TGT_DISTANCE;
	  }

	  ClearTimer(T1);                    // Reset operation expiration timer

	  // Approach loop ... just get into stutter range
	  while ( time1[T1] < ALIGN_DISPENSER_TIMEOUT && USsensorVal() > nStutterDistance )
	  {
	    SetDrivePower(ALIGN_DISPENSER_SPEED);
	  }
	  SetDrivePower(0);

	  // Stutter loop
	  while ( time1[T1] < ALIGN_DISPENSER_TIMEOUT )
	  {
	    error = USsensorVal() - nTargetDistance;

	    nxtDisplayBigTextLine(3, "%d", USsensorVal());

	    if ( bAtGoal && time1[T2] > ALIGN_DISPENSER_AT_GOAL_TIME )
	    {
	      PlaySound(soundBeepBeep);
	      break;
	    }

	    if ( !bAtGoal )
	      ClearTimer(T2);

	    // Not at goal yet or long enough, check if we are now
	    bAtGoal = ( abs(error) <= ALIGN_DISPENSER_TOLERANCE );
	    if ( bAtGoal )
	    {
	      pwr = 0;
	    }
	    else if ( error > 0 )
	    {
	      pwr = ALIGN_DISPENSER_SPEED;
	    }

	    else if ( error < 0 )
	    {
	      pwr = -ALIGN_DISPENSER_SPEED;
	    }

	    SetDrivePower(pwr);

	    // "Stutter" in to give finer approach control
	    if ( ALIGN_DISPENSER_STUTTER_PWR_MSECS > 0 )
	    {
		    wait1Msec(ALIGN_DISPENSER_STUTTER_PWR_MSECS);
		    SetDrivePower(0);
		    wait1Msec(ALIGN_DISPENSER_STUTTER_WAIT_MSECS);
		  }

	  }

    SetDrivePower(0);

	  wait1Msec(250);
}
/*
#define AlignWithDispenser() \
{ \
  _AlignWithDispenser(); \
  _AlignWithDispenser(); \
  _AlignWithDispenser(); \
}

void _AlignWithDispenser()  // Use US sensor to park proper distance from dispenser
{
	  time1[T1] = 0;                    // Reset operation expiration timer

	  if ( USsensorVal() > US_DISPENSER_INIT_TGT_DISTANCE  && USsensorVal() != 255 )    //** Determine which direction is
	  {                                              // needed to allign (forward/back)
	    SetDrivePower(AlignWithDispenser_SPEED);

	    while ( USsensorVal() > US_DISPENSER_INIT_TGT_DISTANCE || time1[T1] > 3000 )
	    {
	      wait1Msec(75);
	      //** Wait for US to reach target value, or expiration timer expires
	    }
	  }
	  else if ( USsensorVal() < US_DISPENSER_INIT_TGT_DISTANCE && USsensorVal() != 255 )
	  {
	    SetDrivePower(-AlignWithDispenser_SPEED);

	    while ( USsensorVal() < US_DISPENSER_INIT_TGT_DISTANCE || time1[T1] > 3000 )
	    {
	      wait1Msec(75);
	      // See ** above
	    }
	  }

	  SetDrivePower(0);

	  wait1Msec(250);
}
*/


#endif // DISPENSING_H
