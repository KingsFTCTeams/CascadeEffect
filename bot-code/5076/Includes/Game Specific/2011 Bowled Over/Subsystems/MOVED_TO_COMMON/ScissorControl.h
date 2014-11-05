#pragma systemFile            // this eliminates warning for "unreferenced" functions

///////////////////////////////////////////////////////////////////////////////////
//
// Public Crate Lift Interface
//
void InitLiftSystem();
void MoveScissor(int pwr);

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//
// Internal stuff...
//
typedef enum
{
  LIFT_DOWN_DIR,
  LIFT_UP_DIR,
  LIFT_STOPPED,
  LIFT_AT_UPPER_LIMIT,
  LIFT_AT_LOWER_LIMIT,

} LIFT_STATE;

int StopAtLimits(int cmdPower);
void SetLiftPower(int cmdPower);
void ResetLiftEncoders();
long LeftScissorEncoder();
long RightScissorEncoder();
long MaxEncoder();
long MinEncoder();


long nLiftRunTime = 0;
long nLiftStartTime = 0;
long nStartEncoderTickCount = 0;
long nEndEncoderTickCount = 0;
long nMaxLiftEncoderValue;
LIFT_STATE tLiftState = LIFT_STOPPED;

//#define DISPLAY_SCISSOR_TEST_DIAGS

#ifdef DISPLAY_SCISSOR_TEST_DIAGS
void DisplayScissorDiags(int cmdPower);
#endif

/////////////////////////////////////////////////////////////////////

void InitLiftSystem()
{
  nMaxLiftEncoderValue = SCISSOR_FULL_UP;
  ResetLiftEncoders();
  nLiftRunTime = 0;
  nLiftStartTime = 0;
  nStartEncoderTickCount = 0;
  nEndEncoderTickCount = 0;
  tLiftState = LIFT_STOPPED;
}

/////////////////////////////////////////////////////////////////////

bool LiftAtLimit()
{
  return (tLiftState == LIFT_AT_UPPER_LIMIT || tLiftState == LIFT_AT_LOWER_LIMIT);
}

/////////////////////////////////////////////////////////////////////

void MoveScissor(int pwr)
{
  if ( (pwr > 0 && tLiftState == LIFT_AT_UPPER_LIMIT) || (pwr < 0 && tLiftState == LIFT_AT_LOWER_LIMIT) )
  {
    pwr = 0;
  }
  else if ( pwr > 0 && tLiftState != LIFT_AT_UPPER_LIMIT )
  {
    if ( tLiftState != LIFT_UP_DIR ) // just hit the button
    {
      nLiftStartTime = nPgmTime;
      nStartEncoderTickCount = MaxEncoder();
      tLiftState = LIFT_UP_DIR;
    }
  }
  else if ( pwr < 0 && tLiftState != LIFT_AT_LOWER_LIMIT  )
  {
    if ( tLiftState != LIFT_DOWN_DIR ) // just hit the button
    {
      nLiftStartTime = nPgmTime;
      nStartEncoderTickCount = MinEncoder();
      tLiftState = LIFT_DOWN_DIR;
    }
  }
  else if ( pwr == 0 )
  {
    if ( tLiftState != LIFT_STOPPED ) // just released the button
    {
      if ( LiftAtLimit() )
        nLiftRunTime = nPgmTime - nLiftStartTime;
      nEndEncoderTickCount = (tLiftState == LIFT_UP_DIR ? MaxEncoder() : MinEncoder());
      tLiftState = LIFT_STOPPED;
    }
  }

  pwr = StopAtLimits(pwr);
  SetLiftPower(pwr);

#ifdef DISPLAY_SCISSOR_TEST_DIAGS
  DisplayScissorDiags(pwr);
#endif
}

/////////////////////////////////////////////////////////////////////

int StopAtLimits(int cmdPower)
{
  // Quick out if not trying to move
  if ( cmdPower == 0 )
    return 0;

#ifdef DISPLAY_SCISSOR_TEST_DIAGS
  // If running in manual mode there are no stops
  //if ( menuOption == MANUAL )
  //  return cmdPower;
#endif

  // Determine if going up and at/beyond goal
  if ( tLiftState == LIFT_UP_DIR && MaxEncoder() > nMaxLiftEncoderValue )
  {
    cmdPower = 0;
    SetLiftPower(0);
    nLiftRunTime = nPgmTime - nLiftStartTime;
    nEndEncoderTickCount = MaxEncoder();
    tLiftState = LIFT_AT_UPPER_LIMIT;
  }

  // Determine if going down and at/below goal
  else if ( tLiftState == LIFT_DOWN_DIR && MinEncoder() < SCISSOR_FULL_DOWN )
  {
    cmdPower = 0;
    SetLiftPower(0);
    nLiftRunTime = nPgmTime - nLiftStartTime;
    nEndEncoderTickCount = MinEncoder();
    tLiftState = LIFT_AT_LOWER_LIMIT;
  }

  return cmdPower;
}

/////////////////////////////////////////////////////////////////////

long MaxEncoder()
{
  long LeftScissorEncoderVal = LeftScissorEncoder();
  long RightScissorEncoderVal = RightScissorEncoder();

  return ( LeftScissorEncoderVal > RightScissorEncoderVal ? LeftScissorEncoderVal : RightScissorEncoderVal );
}

long MinEncoder()
{
  long LeftScissorEncoderVal = LeftScissorEncoder();
  long RightScissorEncoderVal = RightScissorEncoder();

  return ( LeftScissorEncoderVal < RightScissorEncoderVal ? LeftScissorEncoderVal : RightScissorEncoderVal );
}

/////////////////////////////////////////////////////////////////////

#ifdef DISPLAY_SCISSOR_TEST_DIAGS

void DisplayScissorDiags(int cmdPower)
{
  float fLeftEncoder = LeftScissorEncoder();
  float fRightEncoder = RightScissorEncoder();

  float fMaxEncoder = MaxEncoder();
  float fMinEncoder = MinEncoder();
  float fCurrEncoder = (tLiftState == LIFT_DOWN_DIR) ? fMinEncoder : fMaxEncoder;

  // ... motor RPM display
  double rpm = 0;
  double rots = 0;
  double msecs = ( tLiftState == LIFT_STOPPED || tLiftState == LIFT_AT_UPPER_LIMIT || tLiftState == LIFT_AT_LOWER_LIMIT ) ? nLiftRunTime : (nPgmTime - nLiftStartTime);
  double secs =  msecs / 1000.0;
  if ( secs )
  {
	  rots = abs(fCurrEncoder - nStartEncoderTickCount) / 1440.0;
    rpm = rots / secs * 60.0;
  }

  if ( externalBatteryAvg < 0)
		nxtDisplayTextLine(0, "Ext Batt: OFF");       //External battery is off or not connected
	else
		nxtDisplayTextLine(0, "Ext Batt:%4.1f V", externalBatteryAvg / (float) 1000);

  string state = "STOP";
  if ( tLiftState == LIFT_AT_UPPER_LIMIT )
    state = "@ UP MAX";
  if ( tLiftState == LIFT_AT_LOWER_LIMIT )
    state = "@ DWN MAX";
  else if ( tLiftState == LIFT_DOWN_DIR )
    state = "DOWN";
  else if ( tLiftState == LIFT_UP_DIR )
    state = "UP";
/*
  string mode = "MANUAL MODE";
  if ( menuOption == LAB )
    mode = "LAB BENCH";
  else if ( menuOption == FULL )
    mode = "FULL HEIGHT";

  nxtDisplayTextLine(1, "***%s***", mode);
*/
  nxtDisplayTextLine(2, "Pwr:  %d %s", cmdPower, state);

  nxtDisplayTextLine(3, "Secs: %3.1f", secs);
  nxtDisplayTextLine(4, "Rots: %3.1f", rots);
  nxtDisplayTextLine(5, "RPM:  %3.1f", rpm);
  nxtDisplayTextLine(6, "---------------");
  nxtDisplayTextLine(7, "Enc: %5.0f/%5.0f", fLeftEncoder, fRightEncoder);
}

#endif

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void SetLiftPower(int cmdPower)
{
  motor[ScissorRight] = cmdPower;
  motor[ScissorLeft] = cmdPower;
}

/////////////////////////////////////////////////////////////////////

void ResetLiftEncoders()
{
  nMotorEncoder[ScissorRight] = 0;
  nMotorEncoder[ScissorLeft] = 0;
}

/////////////////////////////////////////////////////////////////////

long LeftScissorEncoder()
{
  long left = nMotorEncoder[ScissorLeft];
  long right = nMotorEncoder[ScissorRight];

  long val = left;

  if ( left == 0 && right != 0 )
    val = right;

  return val;
}

long RightScissorEncoder()
{
  long left = nMotorEncoder[ScissorLeft];
  long right = nMotorEncoder[ScissorRight];

  long val = right;

  if ( right == 0 && left != 0 )
    val = left;

  return val;
}
