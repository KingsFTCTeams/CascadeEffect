void Right_Dispense();

void Right_Dispense()
{
  eraseDisplay();

  SelectHeight();

  bDisplayDiagnostics = true;

  waitForStart();

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

  DriveXSquares(-1.85);

  TurnCounterClockwise90();

  SetArmElevation(RAISED);

  if ( High == true )
  {
    SetBridge(APPROACH_HIGH);
  }
  else
  {
    SetBridge(APPROACH_LOW);
  }

  SetArmElevation(LOWERED);

  AlignWithDispenser();
  AlignWithDispenser();
  AlignWithDispenser();

  if ( High == true )
  {
    SetBridge(DISPENSE_HIGH);
  }
  else
  {
    SetBridge(DISPENSE_LOW);
  }

  DispenseXBatons(5);

  SetArmElevation(RAISED);

  DispenseXBatons(12);

  SetBridge(BRIDGE_UP);

  SetArmElevation(LOWERED);

  ///////////////////////////////////////////////////////////////////////////////////

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// END TASK MAIN ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
