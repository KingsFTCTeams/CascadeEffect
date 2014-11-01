#pragma systemFile            // this eliminates warning for "unreferenced" functions

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//
// Intialize the Crate Handling System
//
void InitCrateHandlingSystem();

///////////////////////////////////////////////////////////////////////////////////
//
// Public Crate Lift Interface
//
typedef enum
{
  CRATE_LIFT_TO_GRAB,
  CRATE_LIFT_TO_BALL,
  CRATE_LIFT_TO_CHIMNEY,
  CRATE_LIFT_TO_ROTATE,
  CRATE_LIFT_MANUAL_MOVEMENT,
  CRATE_LIFT_NO_MOVE

} TCrateLiftDestination;

void SetCrateLiftDestination(TCrateLiftDestination crateLiftDest);

///////////////////////////////////////////////////////////////////////////////////
//
// Public Crate Grab Interface
//
typedef enum
{
    CRATE_GRIP_HOLD,
    CRATE_GRIP_OPEN,

} TCrateGripPos;

void SetCrateGripPos(TCrateGripPos pos);

///////////////////////////////////////////////////////////////////////////////////
//
// Public Crate Rotation Interface
//
typedef enum
{
    CRATE_ROT_FWD,
    CRATE_ROT_REV,
    CRATE_ROT_FINE_ADJ_FWD,
    CRATE_ROT_FINE_ADJ_REV,
    CRATE_ROT_STOP,

} TCrateRotate;

void SetCrateRotationAction(TCrateRotate rot);

///////////////////////////////////////////////////////////////////////////////////
//
// Public Crate Claw Interface
//
typedef enum
{
  CLAW_DOWN,
  CLAW_UP,

} TCrateClawCmd;

void SetCrateClawPos(TCrateClawCmd cmd);

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//
// Internal stuff...
//
typedef enum
{
  CRATE_LIFT_AT_GRAB,
  CRATE_LIFT_AT_BALL,
  CRATE_LIFT_AT_CHIMNEY,
  CRATE_LIFT_AT_ROTATE,
  CRATE_LIFT_MOVING_UP,
  CRATE_LIFT_MOVING_DOWN,
  CRATE_LIFT_STOPPED,
  CRATE_LIFT_LOCKED

} TCrateLiftState;

typedef enum
{
  CRATE_LIFT_NOT_PRESET_HEIGHT = -1,
	CRATE_LIFT_GRAB_HEIGHT       = CRATE_LIFT_GRAB_ENCODER,
	CRATE_LIFT_BALL_HEIGHT       = CRATE_LIFT_BALL_ENCODER,
	CRATE_LIFT_ROTATE_HEIGHT     = CRATE_LIFT_ROTATE_ENCODER,
	CRATE_LIFT_CHIMNEY_HEIGHT    = CRATE_LIFT_CHIMNEY_ENCODER

} TCrateLiftHeight;

typedef enum
{
  BRIDGE_UP,
  BRIDGE_DOWN,
  BRIDGE_MOVING_UP,
  BRIDGE_MOVING_DOWN

} TBallDispenseState;


bool DispensingABall();
void DisplayCrateDiags();

TBallDispenseState    _tBallDispenseState;
TCrateLiftState       _tCrateLiftState;
TCrateLiftDestination _tCrateLiftDest;
bool                  _bCrateLiftMovingToTarget;
int                   _nCrateLiftManualPower;

bool _bDispensingABall = false;

#define DISPLAY_CRATE_TEST_DIAGS

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Crate handling control system initialization
//
void InitCrateHandlingSystem()
{
  _bDispensingABall = false;
  _tCrateLiftState = CRATE_LIFT_STOPPED;
  _tCrateLiftDest = CRATE_LIFT_NO_MOVE;
  _nCrateLiftManualPower = 0;
  _bCrateLiftMovingToTarget = false;
  servo[BallDispense] = BALL_DISPENSE_UP_POS; // send the bridge servo to up position
  nMotorEncoder[CrateLift] = 0; // assume the crate lift is in the down position
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Crate grip/grab system
//
//    Control state toggles so buttons don't have to be held down, just tapped
//
void SetCrateGripPos(TCrateGripPos pos)
{
  int servoPos = (pos == CRATE_GRIP_HOLD) ? CRATE_GRIP_CRATE_POS : CRATE_GRIP_OPEN_POS;

  servo[LeftCrateGrip]  = servoPos;
  servo[RightCrateGrip] = servoPos;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Crate rotation system
//
//    Button presses are ignored while rotating, i.e. until motors hit encoder target.
//
//    Pressing any Tophat orientation sets fine control mode (much smaller rotation)
//
//    Have tried motor sync but rotation backwards is very inconsistent and inaccurate
//    for the synched motor.
//
//    Having decent luck with using built-in PID and encoder targets.
//
void SetCrateRotationAction(TCrateRotate rot)
{
  static bool bCrateRotating = false;
  static int nNxtMotorPower = 0;

  // quick out if already rotating...check if we've reached the encoder target
  if (bCrateRotating == true)
  {
	  if (   nMotorRunState[LeftCrateRotate] == runStateIdle
	      || nMotorRunState[RightCrateRotate] == runStateIdle)
	  {
	    nNxtMotorPower = 0;
	    bCrateRotating = false;
	  }
    return;
  }

  // not rotating yet, get it going...
  if ( rot == CRATE_ROT_FWD )
  {
      nMotorEncoderTarget[LeftCrateRotate]  = CRATE_ROTATE_DEFAULT_TURN;
      nMotorEncoderTarget[RightCrateRotate] = CRATE_ROTATE_DEFAULT_TURN;
      nNxtMotorPower = CRATE_ROTATE_POWER;
      bCrateRotating = true;
  }
  else if ( rot == CRATE_ROT_REV )
  {
      nMotorEncoderTarget[LeftCrateRotate]  = -CRATE_ROTATE_DEFAULT_TURN;
      nMotorEncoderTarget[RightCrateRotate] = -CRATE_ROTATE_DEFAULT_TURN;
      nNxtMotorPower = -CRATE_ROTATE_POWER;
      bCrateRotating = true;
  }
  else if ( rot == CRATE_ROT_FINE_ADJ_FWD )
  {
      nMotorEncoderTarget[LeftCrateRotate]  = CRATE_ROTATE_SMALL_TURN;
      nMotorEncoderTarget[RightCrateRotate] = CRATE_ROTATE_SMALL_TURN;
      nNxtMotorPower = CRATE_ROTATE_POWER;
      bCrateRotating = true;
  }
  else if ( rot == CRATE_ROT_FINE_ADJ_REV )
  {
      nMotorEncoderTarget[LeftCrateRotate]  = -CRATE_ROTATE_SMALL_TURN;
      nMotorEncoderTarget[RightCrateRotate] = -CRATE_ROTATE_SMALL_TURN;
      nNxtMotorPower = CRATE_ROTATE_POWER;
      bCrateRotating = true;
  }

  motor[LeftCrateRotate]  = nNxtMotorPower;
  motor[RightCrateRotate] = nNxtMotorPower;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Crate lift system
//
//    Button presses are ignored while rotating, i.e. until motors hit encoder target.
//
//    Pressing any Tophat orientation sets fine control mode (much smaller rotation)
//
//    Have tried motor sync but rotation backwards is very inconsistent and inaccurate
//    for the synched motor.
//
//    Having decent luck with using built-in PID and encoder targets.
//
TCrateLiftState CurrCrateLiftState()
{
  return _tCrateLiftState;
}

/////////////////////////////////////////////////////////////////////

TCrateLiftDestination CurrCrateDest()
{
  return _tCrateLiftDest;
}

/////////////////////////////////////////////////////////////////////

void SetCrateLiftState(TCrateLiftState state)
{
  _tCrateLiftState = state;
}

/////////////////////////////////////////////////////////////////////

bool CrateLiftIsMoving()
{
  return ( _bCrateLiftMovingToTarget || _tCrateLiftState == CRATE_LIFT_MOVING_DOWN || _tCrateLiftState == CRATE_LIFT_MOVING_UP );
}

/////////////////////////////////////////////////////////////////////
int _crateLiftPower = 0;

void SetCrateLiftMotorPower(int power)
{
  _crateLiftPower = power;
  motor[CrateLift] = power;
}

/////////////////////////////////////////////////////////////////////

long GetEncoderTargetForCrateLiftDest(TCrateLiftDestination crateLiftDest)
{
  long tgt = CRATE_LIFT_GRAB_ENCODER;

  if ( crateLiftDest == CRATE_LIFT_TO_BALL )
    tgt = CRATE_LIFT_BALL_ENCODER;

  else if ( crateLiftDest == CRATE_LIFT_TO_ROTATE )
    tgt = CRATE_LIFT_ROTATE_ENCODER;

  else if ( crateLiftDest == CRATE_LIFT_TO_CHIMNEY )
    tgt = CRATE_LIFT_CHIMNEY_ENCODER;

  return tgt;
}

/////////////////////////////////////////////////////////////////////

void SetCrateLiftManualPower(int pwr)
{
  _nCrateLiftManualPower = pwr;
}

/////////////////////////////////////////////////////////////////////
/*
void MoveToCrateLiftDestination(TCrateLiftDestination crateLiftDest)
{
  // this function starts an encoder-controlled motor to start rotating towards
  // a target location.  Control is passed back to calling function so that
  // other operations can be performed while the shoulder is rotating.  It is up to the
  // calling function to turn off motor power

  // NOTE: Power should be passed as a positive value 0-100.
  //       The absolute value of iShoulderPower is multiplied by the direction of
  //       rotation (+/-) to determine sign of power applied to shoulder motor.
  //
  // Read current location
  // determine direction of rotation
  // determine  the encoder distance to travel from current encoder location
  // Turn on motor
  // wait for target to be reached
  // turn off motor

  static int i = 0;
  i += 1;
  nxtDisplayTextLine(1, "count = %d", i);

  long iLiftTarget  = GetEncoderTargetForCrateLiftDest(crateLiftDest);
  long iCurrLiftLoc = nMotorEncoder[CrateLift];
  int iRelativeMove = iLiftTarget - iCurrLiftLoc;

  // need to move?
  if (iRelativeMove == 0)
  {
    SetCrateLiftMotorPower(0);
    _tCrateLiftState = CRATE_LIFT_STOPPED;
  }
  else
  {
	  int iRotateDirection = (iRelativeMove) / abs(iRelativeMove);

	  // Set the target
	  nMotorEncoderTarget[CrateLift] = iRelativeMove;

	  // Start the motor
	  int pwr = iRotateDirection * abs(CRATE_LIFT_UP_POWER);
    SetCrateLiftMotorPower(pwr);

    _tCrateLiftState = ( pwr > 0 ) ? CRATE_LIFT_MOVING_UP : CRATE_LIFT_MOVING_DOWN;
    _tCrateLiftDest = crateLiftDest;
    _bCrateLiftMovingToTarget = true;
  }
}

/////////////////////////////////////////////////////////////////////

void SetCrateLiftDestination(TCrateLiftDestination crateLiftDest)
{
  // Is the crate lift not allowed to move?
  if ( _tCrateLiftState == CRATE_LIFT_LOCKED )
  {
    SetCrateLiftMotorPower(0);
  }

  // manually stopped?
  else if ( crateLiftDest == CRATE_LIFT_NO_MOVE )
  {
    //SetCrateLiftMotorPower(0);
    //_tCrateLiftState = CRATE_LIFT_STOPPED;
  }

  // moving the lift manually?
  else if ( crateLiftDest == CRATE_LIFT_MANUAL_MOVEMENT )
  {
    SetCrateLiftMotorPower(_nCrateLiftManualPower);

    if ( _nCrateLiftManualPower == 0 )
      _tCrateLiftState = CRATE_LIFT_STOPPED;
    else if ( _nCrateLiftManualPower > 0 )
      _tCrateLiftState = CRATE_LIFT_MOVING_UP;
    else
      _tCrateLiftState = CRATE_LIFT_MOVING_DOWN;
  }

  // need to move to a preset location...are we moving yet?
  else if ( CrateLiftIsMoving() == false )
  {
	  nxtDisplayTextLine(5, "");
    MoveToCrateLiftDestination(crateLiftDest);
    while ( nMotorRunState[CrateLift] != runStateIdle )
    {}
    SetCrateLiftManualPower(0);
    nxtDisplayTextLine(5, "HIT TGT");

    _bCrateLiftMovingToTarget = false;

    if ( CurrCrateDest() == CRATE_LIFT_TO_GRAB )
      _tCrateLiftState = CRATE_LIFT_AT_GRAB;
    else if ( CurrCrateDest() == CRATE_LIFT_TO_BALL )
      _tCrateLiftState = CRATE_LIFT_AT_BALL;
    else if ( CurrCrateDest() == CRATE_LIFT_TO_ROTATE )
      _tCrateLiftState = CRATE_LIFT_AT_ROTATE;
    else if ( CurrCrateDest() == CRATE_LIFT_TO_CHIMNEY )
      _tCrateLiftState = CRATE_LIFT_AT_CHIMNEY;
  }

  // need to move to a preset location and we're on our way...
  else
  {
	  if ( nMotorRunState[CrateLift] == runStateIdle )
	  {
	    SetCrateLiftMotorPower(0);
	    nxtDisplayTextLine(5, "HIT TGT");

	    _bCrateLiftMovingToTarget = false;

	    if ( CurrCrateDest() == CRATE_LIFT_TO_GRAB )
	      _tCrateLiftState = CRATE_LIFT_AT_GRAB;
	    else if ( CurrCrateDest() == CRATE_LIFT_TO_BALL )
	      _tCrateLiftState = CRATE_LIFT_AT_BALL;
	    else if ( CurrCrateDest() == CRATE_LIFT_TO_ROTATE )
	      _tCrateLiftState = CRATE_LIFT_AT_ROTATE;
	    else if ( CurrCrateDest() == CRATE_LIFT_TO_CHIMNEY )
	      _tCrateLiftState = CRATE_LIFT_AT_CHIMNEY;
	  }
  }

#ifdef DISPLAY_CRATE_TEST_DIAGS
  DisplayCrateDiags();
#endif
}
*/
/////////////////////////////////////////////////////////////////////

void SetCrateLiftDestination(TCrateLiftDestination crateLiftDest)
{
  int nCrateLiftPower = 0;
  long currLiftEncoder = nMotorEncoder[CrateLift];

  // Is the crate lift not allowed to move?
  if ( _tCrateLiftState == CRATE_LIFT_LOCKED )
  {
    nCrateLiftPower = 0;
  }

  // manual stopped
  else if ( crateLiftDest == CRATE_LIFT_NO_MOVE )
  {
    nCrateLiftPower = 0;
    _tCrateLiftState = CRATE_LIFT_STOPPED;
  }

  // moving the lift manually?
  else if ( crateLiftDest == CRATE_LIFT_MANUAL_MOVEMENT )
  {
    nCrateLiftPower = _nCrateLiftManualPower;
    if ( _nCrateLiftManualPower == 0 )
      _tCrateLiftState = CRATE_LIFT_STOPPED;
    else if ( _nCrateLiftManualPower > 0 )
      _tCrateLiftState = CRATE_LIFT_MOVING_UP;
    else
      _tCrateLiftState = CRATE_LIFT_MOVING_DOWN;
  }

  // go to crate grab position?
  else if ( crateLiftDest == CRATE_LIFT_TO_GRAB )
  {
    if ( WithinRange(CRATE_LIFT_GRAB_ENCODER, currLiftEncoder, CRATE_LIFT_ENCODER_SKID) )
    {
       nCrateLiftPower = 0;
      _tCrateLiftState = CRATE_LIFT_AT_GRAB;
    }
    else if ( currLiftEncoder < CRATE_LIFT_GRAB_ENCODER )
    {
       nCrateLiftPower = CRATE_LIFT_UP_POWER;
      _tCrateLiftState = CRATE_LIFT_MOVING_UP;
    }
    else
    {
       nCrateLiftPower = CRATE_LIFT_DWN_POWER;
      _tCrateLiftState = CRATE_LIFT_MOVING_DOWN;
    }
  }

  // go to ball load position?
  else if ( crateLiftDest == CRATE_LIFT_TO_BALL )
  {
    if ( WithinRange(CRATE_LIFT_BALL_ENCODER, currLiftEncoder, CRATE_LIFT_ENCODER_SKID) )
    {
       nCrateLiftPower = 0;
      _tCrateLiftState = CRATE_LIFT_AT_BALL;
    }
    else if ( currLiftEncoder < CRATE_LIFT_BALL_ENCODER )
    {
       nCrateLiftPower = CRATE_LIFT_UP_POWER;
      _tCrateLiftState = CRATE_LIFT_MOVING_UP;
    }
    else
    {
       nCrateLiftPower = CRATE_LIFT_DWN_POWER;
      _tCrateLiftState = CRATE_LIFT_MOVING_DOWN;
    }
  }

  // go to crate rotate position
  else if ( crateLiftDest == CRATE_LIFT_TO_ROTATE )
  {
    if ( WithinRange(CRATE_LIFT_ROTATE_ENCODER, currLiftEncoder, CRATE_LIFT_ENCODER_SKID) )
    {
       nCrateLiftPower = 0;
      _tCrateLiftState = CRATE_LIFT_AT_ROTATE;
    }
    else if ( currLiftEncoder < CRATE_LIFT_ROTATE_ENCODER )
    {
       nCrateLiftPower = CRATE_LIFT_UP_POWER;
      _tCrateLiftState = CRATE_LIFT_MOVING_UP;
    }
    else
    {
       nCrateLiftPower = CRATE_LIFT_DWN_POWER;
      _tCrateLiftState = CRATE_LIFT_MOVING_DOWN;
    }
  }

  // go to chimney load position
  else if ( crateLiftDest == CRATE_LIFT_TO_CHIMNEY )
  {
    if ( WithinRange(CRATE_LIFT_CHIMNEY_ENCODER, currLiftEncoder, CRATE_LIFT_ENCODER_SKID) )
    {
       nCrateLiftPower = 0;
      _tCrateLiftState = CRATE_LIFT_AT_CHIMNEY;
    }
    else if ( currLiftEncoder < CRATE_LIFT_CHIMNEY_ENCODER )
    {
       nCrateLiftPower = CRATE_LIFT_UP_POWER;
      _tCrateLiftState = CRATE_LIFT_MOVING_UP;
    }
    else
    {
       nCrateLiftPower = CRATE_LIFT_DWN_POWER;
      _tCrateLiftState = CRATE_LIFT_MOVING_DOWN;
    }
  }

  SetCrateLiftMotorPower(nCrateLiftPower);
  _tCrateLiftDest = crateLiftDest;

#ifdef DISPLAY_CRATE_TEST_DIAGS
  DisplayCrateDiags();
#endif
}

/////////////////////////////////////////////////////////////////////

#ifdef DISPLAY_CRATE_TEST_DIAGS

void DisplayCrateDiags()
{
  float fCurrEncoder = nMotorEncoder[CrateLift];

  if ( externalBatteryAvg < 0)
		nxtDisplayTextLine(0, "Ext Batt: OFF");       //External battery is off or not connected
	else
		nxtDisplayTextLine(0, "Ext Batt:%4.1f V", externalBatteryAvg / (float) 1000);

  string liftState = "xxx";
  if ( _tCrateLiftState == CRATE_LIFT_AT_GRAB )
    liftState = "AT GRAB";
  if ( _tCrateLiftState == CRATE_LIFT_AT_BALL )
    liftState = "AT BALL";
  else if ( _tCrateLiftState == CRATE_LIFT_AT_CHIMNEY )
    liftState = "AT CHIMNEY";
  else if ( _tCrateLiftState == CRATE_LIFT_AT_ROTATE )
    liftState = "AT ROTATE";
  else if ( _tCrateLiftState == CRATE_LIFT_MOVING_UP )
    liftState = "UP";
  else if ( _tCrateLiftState == CRATE_LIFT_MOVING_DOWN )
    liftState = "DWN";
  else if ( _tCrateLiftState == CRATE_LIFT_STOPPED )
    liftState = "STOPPED";
  else if ( _tCrateLiftState == CRATE_LIFT_LOCKED )
    liftState = "LOCKED";

  string liftDest = "zzz";
  if ( _tCrateLiftDest == CRATE_LIFT_TO_GRAB )
    liftDest = "TO GRAB";
  else if ( _tCrateLiftDest == CRATE_LIFT_TO_BALL )
    liftDest = "TO BALL";
  else if ( _tCrateLiftDest == CRATE_LIFT_TO_ROTATE )
    liftDest = "TO ROTATE";
  else if ( _tCrateLiftDest == CRATE_LIFT_TO_CHIMNEY )
    liftDest = "TO CHIMNEY";
  else if ( _tCrateLiftDest == CRATE_LIFT_MANUAL_MOVEMENT )
    liftDest = "MANUAL";
  else if ( _tCrateLiftDest == CRATE_LIFT_NO_MOVE )
    liftDest = "NO MOVE";

  string ballState = "yyy";
  if ( _tBallDispenseState == BRIDGE_UP )
    ballState = "IS UP";
  else if ( _tBallDispenseState == BRIDGE_DOWN )
    ballState = "IS DOWN";
  else if ( _tBallDispenseState == BRIDGE_MOVING_UP )
    ballState = "MOVE UP";
  else if ( _tBallDispenseState == BRIDGE_MOVING_DOWN )
    ballState = "MOVE DWN";

  string idle = (nMotorRunState[CrateLift] == runStateIdle) ? "yes" : "no";

  nxtDisplayTextLine(2, "Lift: %d %s", _crateLiftPower, liftState);
  nxtDisplayTextLine(3, "Dest: %s", liftDest);
  nxtDisplayTextLine(4, "Enc:  %5.0f", fCurrEncoder);
  //nxtDisplayTextLine(5, "Idle: %s", idle);

  nxtDisplayTextLine(6, "---------------");

  nxtDisplayTextLine(7, "Ball: %s", ballState);

}

#endif

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void SetBallDispense()
{
  long nStartTime;

  // Ignore multiple button presses/mashing...
  if ( joy2Btn(BTN_BALL_DISPENSE) && _bDispensingABall == false )
  {
    _bDispensingABall = true;
  }

  // early out if we're not dispensing a ball
  if ( _bDispensingABall == false )
  {
    return;
  }

  // Tell the crate lift to go to ball height and wait until it is there
  if ( CurrCrateLiftState() != CRATE_LIFT_AT_BALL && CurrCrateLiftState() != CRATE_LIFT_LOCKED )
  {
    if ( CrateLiftIsMoving() == false )
      SetCrateLiftDestination(CRATE_LIFT_TO_BALL);
    return;
  }

  // Crate lift is at ball height, now lock the crate lift in place so we can put the bridge down...
  SetCrateLiftState(CRATE_LIFT_LOCKED);

  // Tell the bridge to go down, and wait until it is there
  if ( _tBallDispenseState != BRIDGE_DOWN && _tBallDispenseState != BRIDGE_MOVING_UP )
  {
    // If the bridge isn't moving yet, get it moving...
    if ( _tBallDispenseState != BRIDGE_MOVING_DOWN )
    {
      nStartTime = nPgmTime;
      servo[BallDispense] = BALL_DISPENSE_DOWN_POS;
      _tBallDispenseState = BRIDGE_MOVING_DOWN;
    }

    // Wait until the bridge has had enough time to move down
    float t = nPgmTime - nStartTime;
    if ( (nPgmTime - nStartTime) < BRIDGE_MOVING_DOWN_MSEC )
    {
      return;
    }
	  _tBallDispenseState = BRIDGE_DOWN;
  }

  // Bridge is down, tell it to go back up, and wait until it is there
  if ( _tBallDispenseState != BRIDGE_UP )
  {
    // If the bridge isn't moving yet, get it moving...
    if ( _tBallDispenseState != BRIDGE_MOVING_UP )
    {
      nStartTime = nPgmTime;
      servo[BallDispense] = BALL_DISPENSE_UP_POS;
      _tBallDispenseState = BRIDGE_MOVING_UP;
    }

    // Wait until the bridge has had enough time to move up
    if ( (nPgmTime - nStartTime) < BRIDGE_MOVING_UP_MSEC )
    {
      return;
    }
	  _tBallDispenseState = BRIDGE_UP;
	}

  // Bridge is up, now release the crate lift so it can move again...
  _bDispensingABall = false;
  SetCrateLiftState(CRATE_LIFT_AT_BALL);
}

/////////////////////////////////////////////////////////////////////

bool DispensingABall()
{
  return _bDispensingABall;
}

/////////////////////////////////////////////////////////////////////

void SetCrateClawPos(TCrateClawCmd cmd)
{
    servo[CrateClaw] = (cmd == CLAW_DOWN) ? CLAW_DOWN_POS : CLAW_UP_POS;
}
