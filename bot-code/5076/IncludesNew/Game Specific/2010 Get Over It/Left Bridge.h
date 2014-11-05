void Left_Bridge();

void Left_Bridge()
{
  bDisplayDiagnostics = true;

  ///////////////////////// Insert Autonomous directions here //////////////////////

  DriveXSquares(.75);

  TurnClockwise90();

  DriveXSquares(.9);

  TurnClockwise90();

  DriveXSquares(-.4);

  DriveXSquares(.005);

  SetSkis(SKIS_OUT);

  DriveXSquares(-1);

  SetSkis(SKIS_IN);

  DriveXSquares(-2.4);

  DriveXSquares(.15);

  TurnCounterClockwise90();

  DriveXSquares(1.65);

  DriveXSquares(-.15);

  TurnCounterClockwise90();

  DriveXSquares(-.5);

  TrackIR();

  SetArmElevation(RAISED);

  SetBridge(APPROACH_MID);

  SetArmElevation(LOWERED);

  AlignWithDispenser();
  AlignWithDispenser();
  AlignWithDispenser();

  SetBridge(DISPENSE_MID);

  DispenseXBatons(5);

  SetArmElevation(RAISED);

  DispenseXBatons(12);

  SetBridge(BRIDGE_UP);
  ///////////////////////////////////////////////////////////////////////////////////

}
