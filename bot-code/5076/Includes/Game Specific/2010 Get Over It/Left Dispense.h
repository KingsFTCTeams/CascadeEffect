void Left_Dispense();

void Left_Dispense()
{
  eraseDisplay();

  SelectHeight();   // Ask user which height to dispense batons from

  bDisplayDiagnostics = true;

  waitForStart();

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

  DriveXSquares(-1.85);

  TurnClockwise90();

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
