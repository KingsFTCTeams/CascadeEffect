#define Right_Balance() \
{ \
  bDisplayDiagnostics = true; \
  \
  DriveXSquares(.75); \
  \
  TurnCounterClockwise90(); \
  \
  DriveXSquares(.9);  \
  \
  TurnCounterClockwise90(); \
  \
  DriveXSquares(-.4); \
  \
  DriveXSquares(.005);  \
  \
  SetSkis(SKIS_OUT);  \
  \
  DriveXSquares(-1.425);  \
}
