#pragma systemFile            // this eliminates warning for "unreferenced" functions

///////////////////////////////////////////////////////////////////////////////////
//
// Public Ball Dispensing Interface
//
void DispenseABall();
bool CurrentlyDispensingABall();

task BallDispenserTask();

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//
// Internal predecs...
//
typedef enum
{
  BRIDGE_UP,
  BRIDGE_DOWN,
  BRIDGE_MOVING_UP,
  BRIDGE_MOVING_DOWN

} _TBallDispenseState;

void _InitBallDispenser();
void _ServiceBallDispenser();
void _SetBallDispensePos(int pos);

#ifdef DISPLAY_CRATE_TEST_DIAGS
  void _DisplayBallDiags();
#endif

_TBallDispenseState  _tBallDispenseState;
bool                _bCurrentlyDispensingABall = false;
bool                _bDispenseABall = false;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

task BallDispenserTask()
{
  _InitBallDispenser();

  while (true)
  {
    _ServiceBallDispenser();
    wait1Msec(5);
  }
}

/////////////////////////////////////////////////////////////////////

void DispenseABall()
{
  _bDispenseABall = true;
}

/////////////////////////////////////////////////////////////////////

bool CurrentlyDispensingABall()
{
  return _bCurrentlyDispensingABall;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void _InitBallDispenser()
{
  _bDispenseABall = false;
  _bCurrentlyDispensingABall = false;
  _SetBallDispensePos(BALL_DISPENSE_UP_POS); // send the bridge servo to up position
}

/////////////////////////////////////////////////////////////////////

void _ServiceBallDispenser()
{
  static long nStartTime;

#ifdef DISPLAY_CRATE_TEST_DIAGS
  _DisplayBallDiags();
#endif

  // Only dispense one ball at a time
  if ( _bDispenseABall && !_bCurrentlyDispensingABall )
  {
    _bCurrentlyDispensingABall = true;
  }

  // early out if we're not dispensing a ball
  if ( _bCurrentlyDispensingABall == false )
  {
    return;
  }

  // Tell the crate lift to go to ball height and wait until it is there
  if ( CurrCrateLiftState() != CRATE_LIFT_AT_BALL && CurrCrateLiftState() != CRATE_LIFT_LOCKED )
  {
    if ( _CrateLiftIsMoving() == false )
      SetCrateLiftDestination(CRATE_LIFT_TO_BALL);

    return;
  }

  // Crate lift is at ball height, now lock the crate lift in place so we can put the bridge down...
  _SetCrateLiftState(CRATE_LIFT_LOCKED);

  // Tell the bridge to go down, and wait until it is there
  if ( _tBallDispenseState != BRIDGE_DOWN && _tBallDispenseState != BRIDGE_MOVING_UP )
  {
    // If the bridge isn't moving yet, get it moving...
    if ( _tBallDispenseState != BRIDGE_MOVING_DOWN )
    {
      nStartTime = nPgmTime;
      _SetBallDispensePos(BALL_DISPENSE_DOWN_POS);
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
      _SetBallDispensePos(BALL_DISPENSE_UP_POS);
      _tBallDispenseState = BRIDGE_MOVING_UP;
    }

    // Wait until the bridge has had enough time to move up
    if ( (nPgmTime - nStartTime) < BRIDGE_MOVING_UP_MSEC )
    {
      return;
    }

    // Bridge is now back up and we're done dispensing the ball
	  _tBallDispenseState = BRIDGE_UP;
	  _bCurrentlyDispensingABall = false;
	  _bDispenseABall = false;

	  // ...release the crate lift so it can move again...
	  _SetCrateLiftState(CRATE_LIFT_AT_BALL);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

void _SetBallDispensePos(int pos)
{
  servo[BallDispense] = pos;
}

/////////////////////////////////////////////////////////////////////

#ifdef DISPLAY_CRATE_TEST_DIAGS

void _DisplayBallDiags()
{
  string ballState = "yyy";
  if ( _tBallDispenseState == BRIDGE_UP )
    ballState = "IS UP";
  else if ( _tBallDispenseState == BRIDGE_DOWN )
    ballState = "IS DOWN";
  else if ( _tBallDispenseState == BRIDGE_MOVING_UP )
    ballState = "MOVE UP";
  else if ( _tBallDispenseState == BRIDGE_MOVING_DOWN )
    ballState = "MOVE DWN";

  if ( externalBatteryAvg < 0)
		nxtDisplayTextLine(0, "Ext Batt: OFF");       //External battery is off or not connected
	else
		nxtDisplayTextLine(0, "Ext Batt:%4.1f V", externalBatteryAvg / (float) 1000);

  nxtDisplayTextLine(6, "---------------");
  nxtDisplayTextLine(7, "Ball: %s", ballState);

}

#endif
