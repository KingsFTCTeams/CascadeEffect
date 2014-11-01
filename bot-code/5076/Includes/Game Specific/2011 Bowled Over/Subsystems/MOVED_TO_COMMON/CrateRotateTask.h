#pragma systemFile            // this eliminates warning for "unreferenced" functions

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

void SetCrateRotationAction(TCrateRotate rotCmd);

task CrateRotationTask();

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// Internal predecs...
//
void _InitCrateRotationSystem();
void _SetCrateRotationAction(TCrateRotate rotCmd);
void _ServiceCrateRotateCmd();

void _DriveToTargetUsingBuiltInPID(long cmdEncoderTgt);
void _DriveToTargetUsingDirectControl(long cmdEncoderTgt);

void _SetCrateRotationTgt(int nNxtMotorTgt);

void _SetCrateRotationPower(int nNxtMotorPower);
void _SetCrateRotationLeftPower(int nNxtMotorPower);
void _SetCrateRotationRightPower(int nNxtMotorPower);

long _LeftCrateRotationEncoder();
long _RightCrateRotationEncoder();
void _ResetCrateRotationEncoders();

TCrateRotate _tCrateRotateCmd = CRATE_ROT_STOP;

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// Crate Rotation Task
//
task CrateRotationTask()
{
  _InitCrateRotationSystem();

  while (true)
  {
    _ServiceCrateRotateCmd();
    wait1Msec(10);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// This is the public interface to rotate crates ... simply sets the command for the
// crate rotation task to respond to.
//
//    New commands are ignored while rotating, i.e. until motors hit encoder target.
//
//    Have tried motor sync but rotation backwards is very inconsistent and inaccurate
//    for the synched motor.
//
//    Having decent luck with using built-in PID and encoder targets.
//
void SetCrateRotationAction(TCrateRotate rotCmd)
{
  _tCrateRotateCmd = rotCmd;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// Internal stuff...
//
void _InitCrateRotationSystem()
{
  _tCrateRotateCmd = CRATE_ROT_STOP;
  _ResetCrateRotationEncoders();
}

/////////////////////////////////////////////////////////////////////////////////////////

void _ServiceCrateRotateCmd()
{
  TCrateRotate rot = _tCrateRotateCmd;

  // if we're stopped, have nothing to do, early out.
  if (rot == CRATE_ROT_STOP)
    return;

  // determine the rotation target
  long cmdEncoderTgt = CRATE_ROTATE_DEFAULT_TURN;

  if ( rot == CRATE_ROT_REV )
      cmdEncoderTgt = -CRATE_ROTATE_DEFAULT_TURN;

  else if ( rot == CRATE_ROT_FINE_ADJ_FWD )
      cmdEncoderTgt = CRATE_ROTATE_SMALL_TURN;

  else if ( rot == CRATE_ROT_FINE_ADJ_REV )
      cmdEncoderTgt = -CRATE_ROTATE_SMALL_TURN;

  //_DriveToTargetUsingBuiltInPID(cmdEncoderTgt);
  _DriveToTargetUsingDirectControl(cmdEncoderTgt);

  _tCrateRotateCmd = CRATE_ROT_STOP;
}

//////////////////////////////////////////////////////////////////////////////////////////

void _DriveToTargetUsingBuiltInPID(long cmdEncoderTgt)
{
  // Set the rotation target
  _ResetCrateRotationEncoders();

  _SetCrateRotationTgt(cmdEncoderTgt);

  // Get the motors moving
  int nNxtMotorPower = (cmdEncoderTgt > 0) ? CRATE_ROTATE_POWER : -CRATE_ROTATE_POWER;
  _SetCrateRotationPower(nNxtMotorPower);

  // Wait until both of the motors reach the target
  bool bLeftAtTarget = false;
  bool bRightAtTarget = false;
  while ( !bLeftAtTarget || !bRightAtTarget )
  {
    if ( !bLeftAtTarget )
      bLeftAtTarget  = (nMotorRunState[LeftCrateRotate] == runStateIdle);

    if ( !bRightAtTarget )
      bRightAtTarget = (nMotorRunState[RightCrateRotate] == runStateIdle);

    if ( bLeftAtTarget )
      _SetCrateRotationLeftPower(0);

    if ( bRightAtTarget )
      _SetCrateRotationRightPower(0);
  }

	// ...hit the target, stop the motors and set stop state
  _SetCrateRotationPower(0);
}

//////////////////////////////////////////////////////////////////////////////////////////

void _DriveToTargetUsingDirectControl(long cmdEncoderTgt)
{
  // Get the motors moving
  _ResetCrateRotationEncoders();

  int nNxtMotorPower = (cmdEncoderTgt > 0) ? CRATE_ROTATE_POWER : -CRATE_ROTATE_POWER;
  _SetCrateRotationPower(nNxtMotorPower);

  // Wait until either of the motors reaches the target
  bool bLeftAtTarget = false;
  bool bRightAtTarget = false;
  while ( !bLeftAtTarget || !bRightAtTarget )
  {
    bLeftAtTarget  = WithinRange(_LeftCrateRotationEncoder(), cmdEncoderTgt, CRATE_ROTATE_ENCODER_SKID );
    bRightAtTarget = WithinRange(_RightCrateRotationEncoder(), cmdEncoderTgt, CRATE_ROTATE_ENCODER_SKID );

    if ( bLeftAtTarget )
      _SetCrateRotationLeftPower(0);

    if ( bRightAtTarget )
      _SetCrateRotationRightPower(0);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

void _SetCrateRotationPower(int nNxtMotorPower)
{
  _SetCrateRotationLeftPower(nNxtMotorPower);
  _SetCrateRotationRightPower(nNxtMotorPower);
}

void _SetCrateRotationLeftPower(int nNxtMotorPower)
{
  motor[LeftCrateRotate]  = nNxtMotorPower;
}

void _SetCrateRotationRightPower(int nNxtMotorPower)
{
  motor[RightCrateRotate]  = nNxtMotorPower;
}

//////////////////////////////////////////////////////////////////////////////////////////

void _SetCrateRotationTgt(int nNxtMotorTgt)
{
  nMotorEncoderTarget[LeftCrateRotate]  = nNxtMotorTgt;
  nMotorEncoderTarget[RightCrateRotate] = nNxtMotorTgt;
}

//////////////////////////////////////////////////////////////////////////////////////////

long _LeftCrateRotationEncoder()
{
  return nMotorEncoder[LeftCrateRotate];
}

//////////////////////////////////////////////////////////////////////////////////////////

long _RightCrateRotationEncoder()
{
  return nMotorEncoder[RightCrateRotate];
}

//////////////////////////////////////////////////////////////////////////////////////////

void _ResetCrateRotationEncoders()
{
  nMotorEncoder[LeftCrateRotate] = 0;
  nMotorEncoder[RightCrateRotate] = 0;
}
