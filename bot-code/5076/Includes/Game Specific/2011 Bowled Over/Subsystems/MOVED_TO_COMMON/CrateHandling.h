#pragma systemFile            // this eliminates warning for "unreferenced" functions

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//
// Intialize the Crate Handling System (excluding crate rotation and ball dispensing)
//    -Crate lifter
//    -Crate gripper
//    -Crate claw
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

void SetCrateLiftDestination(TCrateLiftDestination crateLiftDest);
void SetCrateLiftManualPower(int pwr);
TCrateLiftState CurrCrateLiftState();

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
  CRATE_LIFT_NOT_PRESET_HEIGHT = -1,
	CRATE_LIFT_GRAB_HEIGHT       = CRATE_LIFT_GRAB_ENCODER,
	CRATE_LIFT_BALL_HEIGHT       = CRATE_LIFT_BALL_ENCODER,
	CRATE_LIFT_ROTATE_HEIGHT     = CRATE_LIFT_ROTATE_ENCODER,
	CRATE_LIFT_CHIMNEY_HEIGHT    = CRATE_LIFT_CHIMNEY_ENCODER

} TCrateLiftHeight;

void _ResetCrateLiftEncoder();

#define DISPLAY_CRATE_TEST_DIAGS

#ifdef DISPLAY_CRATE_TEST_DIAGS
  void _DisplayCrateDiags();
#endif

TCrateLiftState       _tCrateLiftState;
TCrateLiftDestination _tCrateLiftDest;
bool                  _bCrateLiftMovingToTarget;
int                   _nCrateLiftManualPower;

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Crate handling control system initialization
//
void InitCrateHandlingSystem()
{
  _tCrateLiftState = CRATE_LIFT_STOPPED;
  _tCrateLiftDest = CRATE_LIFT_NO_MOVE;
  _nCrateLiftManualPower = 0;
  _bCrateLiftMovingToTarget = false;
  _ResetCrateLiftEncoder();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// Crate grip/grab system
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
/////////////////////////////////////////////////////////////////////////////////////////
//
// Crate lift system
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

TCrateLiftDestination _CurrCrateLiftDest()
{
  return _tCrateLiftDest;
}

void _SetCrateLiftState(TCrateLiftState state)
{
  _tCrateLiftState = state;
}

bool _CrateLiftIsMoving()
{
  return ( _bCrateLiftMovingToTarget || _tCrateLiftState == CRATE_LIFT_MOVING_DOWN || _tCrateLiftState == CRATE_LIFT_MOVING_UP );
}

void SetCrateLiftManualPower(int pwr)
{
  _nCrateLiftManualPower = pwr;
}

/////////////////////////////////////////////////////////////////////
int _crateLiftPower = 0;

void _SetCrateLiftMotorPower(int power)
{
  _crateLiftPower = power;
  motor[CrateLift] = power;
}

void _ResetCrateLiftEncoder()
{
  nMotorEncoder[CrateLift] = 0; // assume the crate lift is in the down position
}

long _CurrCrateLiftEncoder()
{
  return nMotorEncoder[CrateLift];
}

void _SetCrateLiftEncoderTarget(long tgt)
{
	  nMotorEncoderTarget[CrateLift] = tgt;
}

TNxtRunState _CrateLiftMotorRunState()
{
  return nMotorRunState[CrateLift];
}

/////////////////////////////////////////////////////////////////////

long _GetEncoderTargetForCrateLiftDest(TCrateLiftDestination crateLiftDest)
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

int _DetermineLiftPowerForDest(long nEncoderTarget, TCrateLiftState tTargetState)
{
  int pwr = 0;
  long nCurrLiftEncoder = _CurrCrateLiftEncoder();

  // Are we at the target?
  if ( WithinRange(nEncoderTarget, nCurrLiftEncoder, CRATE_LIFT_ENCODER_SKID) )
  {
     pwr = 0;
    _tCrateLiftState = tTargetState;
  }
  // below and need to move up?
  else if ( nCurrLiftEncoder < nEncoderTarget )
  {
     pwr = CRATE_LIFT_UP_POWER;
    _tCrateLiftState = CRATE_LIFT_MOVING_UP;
  }
  // above and need to move down.
  else
  {
     pwr = CRATE_LIFT_DWN_POWER;
    _tCrateLiftState = CRATE_LIFT_MOVING_DOWN;
  }

  return pwr;
}

/////////////////////////////////////////////////////////////////////

void SetCrateLiftDestination(TCrateLiftDestination crateLiftDest)
{
  int nCrateLiftPower = 0;

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
    nCrateLiftPower = _DetermineLiftPowerForDest(CRATE_LIFT_GRAB_ENCODER, CRATE_LIFT_AT_GRAB);
  }

  // go to ball load position?
  else if ( crateLiftDest == CRATE_LIFT_TO_BALL )
  {
    nCrateLiftPower = _DetermineLiftPowerForDest(CRATE_LIFT_BALL_ENCODER, CRATE_LIFT_AT_BALL);
  }

  // go to crate rotate position
  else if ( crateLiftDest == CRATE_LIFT_TO_ROTATE )
  {
    nCrateLiftPower = _DetermineLiftPowerForDest(CRATE_LIFT_ROTATE_ENCODER, CRATE_LIFT_AT_ROTATE);
  }

  // go to chimney load position
  else if ( crateLiftDest == CRATE_LIFT_TO_CHIMNEY )
  {
    nCrateLiftPower = _DetermineLiftPowerForDest(CRATE_LIFT_CHIMNEY_ENCODER, CRATE_LIFT_AT_CHIMNEY);
  }

  _SetCrateLiftMotorPower(nCrateLiftPower);
  _tCrateLiftDest = crateLiftDest;

#ifdef DISPLAY_CRATE_TEST_DIAGS
  _DisplayCrateDiags();
#endif
}

/////////////////////////////////////////////////////////////////////

#ifdef DISPLAY_CRATE_TEST_DIAGS

void _DisplayCrateDiags()
{
  float fCurrEncoder = _CurrCrateLiftEncoder();

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

  if ( externalBatteryAvg < 0)
		nxtDisplayTextLine(0, "Ext Batt: OFF");       //External battery is off or not connected
	else
		nxtDisplayTextLine(0, "Ext Batt:%4.1f V", externalBatteryAvg / (float) 1000);

  nxtDisplayTextLine(2, "Lift: %d %s", _crateLiftPower, liftState);
  nxtDisplayTextLine(3, "Dest: %s", liftDest);
  nxtDisplayTextLine(4, "Enc:  %5.0f", fCurrEncoder);
}

#endif

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// Crate claw system
//

void SetCrateClawPos(TCrateClawCmd cmd)
{
    servo[CrateClaw] = (cmd == CLAW_DOWN) ? CLAW_DOWN_POS : CLAW_UP_POS;
}
