void Right_Bridge();

void Right_Bridge()
{
  bDisplayDiagnostics = true;

  ///////////////////////// Insert Autonomous directions here //////////////////////

  DriveXSquares(.75);

  TurnCounterClockwise90();

  DriveXSquares(.9);

  TurnCounterClockwise90();

  DriveXSquares(-.4);

  DriveXSquares(.005);

  SetSkis(SKIS_OUT);

  DriveXSquares(-1);

  SetSkis(SKIS_IN);

  DriveXSquares(-1.4);

  DriveXSquares(.15);

  TurnCounterClockwise90();

  DriveXSquares(-1.5);

  TurnCounterClockwise90();

  DriveXSquares(-.65);

  DriveXSquares(.15);

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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// END TASK MAIN ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
