#define Left_Balance()  \
{ \
  bDisplayDiagnostics = true;   \
  \
  DriveXSquares(.75); \
  \
  TurnClockwise90();  \
  \
  DriveXSquares(.9);  \
  \
  TurnClockwise90();  \
  \
  DriveXSquares(-.4); \
  \
  DriveXSquares(.005);  \
  \
  SetSkis(SKIS_OUT);  \
  \
  DriveXSquares(-1.425);  \
  \
  SetSkis(SKIS_IN); \
  \
}
