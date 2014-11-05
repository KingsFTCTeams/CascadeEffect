#pragma systemFile            // this eliminates warning for "unreferenced" functions

/////////////////////////////////////////////////////////////////////////////////////////
//
// Public Ball Collection (belt) control system initialization
//
void InitBallCollectionSystem()
{
}

///////////////////////////////////////////////////////////////////////////////////
//
// Public Crate Rotation Interface
//
typedef enum
{
  BALL_COLLECT_FWD,
  BALL_COLLECT_REV,
  BALL_COLLECT_STOP,

} TBallCollectionCmd;

void BallCollectionSystem(TBallCollectionCmd cmd)
{
  int pwr = 0;

  if ( cmd == BALL_COLLECT_FWD )
    pwr = 100;

  else if ( cmd == BALL_COLLECT_REV )
    pwr = -100;

  motor[BallCollection] = pwr;
}
