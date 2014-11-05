void Left_Score();

void Left_Score()
{
  int Length = 1;
  nxtDisplayCenteredBigTextLine(3, "HALF?");

  time1[T1] = 0;

  while ( nNxtButtonPressed != 3 )
  {
    if ( nNxtButtonPressed == 1 && Length < 4 && time1[T1] > 250 )
    {
      Length++;
      time1[T1] = 0;
    }
    else if ( nNxtButtonPressed == 2 && Length > 1 && time1[T1] > 250 )
    {
      Length--;
      time1[T1] = 0;
    }
    if ( Length == 1 )
    {
      nxtDisplayCenteredBigTextLine(3, "HALF?");
    }
    if ( Length == 2 )
    {
      nxtDisplayCenteredBigTextLine(3, "FULL?");
    }
    if ( Length == 3 )
    {
      nxtDisplayCenteredBigTextLine(3, "BALANCE?");
    }
    if ( Length == 4 )
    {
      nxtDisplayCenteredBigTextLine(3, "SIDE?");
    }
  }

  eraseDisplay();

  nxtDisplayCenteredBigTextLine(3, "CONFIRMED");

  wait1Msec(500);

  if ( Length == 4 )
  {
    SelectHeight();
  }

  eraseDisplay();

  bDisplayDiagnostics = true;

  waitForStart();

  DriveXSquares(1.6);

  ScoreBatons(LEFT_GOAL);

  //wait1Msec(2000);

  if ( Length == 1 )
  {
    SetAbortAutonomous(true);
    return;
  }

  if ( Length == 2 )
  {
	  DriveXSquares(.8); // was .75

	  TurnCounterClockwise90();

	  DriveXSquares(3.85);

	  if ( USsensorVal() < TRACK_IR_MINIMUM_DISTANCE )
	  {
	    SetDriveSidePower(LEFT_SIDE, 100);
	    SetDriveSidePower(RIGHT_SIDE, 0);
	    wait1Msec(1000);
      SetDrivePower(0);
	    wait1Msec(250);
	    SetDriveSidePower(LEFT_SIDE, -100);
	    SetDriveSidePower(RIGHT_SIDE, 0);
	    wait1Msec(1000);
	    SetDrivePower(0);
	    wait1Msec(250);

	    SetDriveSidePower(LEFT_SIDE, 0);
	    SetDriveSidePower(RIGHT_SIDE, 100);
	    wait1Msec(1000);
      SetDrivePower(0);
	    wait1Msec(250);
	    SetDriveSidePower(LEFT_SIDE, 0);
	    SetDriveSidePower(RIGHT_SIDE, -100);
	    wait1Msec(1000);
	    SetDrivePower(0);
	    wait1Msec(250);

	    DriveXSquares(.5);
	    DriveXSquares(-.5);
	  }

	  TrackIR();

	  SetArmElevation(RAISED);

    SetBridge(APPROACH_MID);

    SetArmElevation(LOWERED);

	  AlignWithDispenser();

	  SetBridge(DISPENSE_MID);

	  DispenseXBatons(5);

	  SetArmElevation(RAISED);

	  DispenseXBatons(12);
	}

	if ( Length == 3 )
	{
	  DriveXSquares(-.45);

	  TurnClockwise90();

	  DriveXSquares(-1.25);

	  DriveXSquares(.005);

	  SetSkis(SKIS_OUT);

	  DriveXSquares(-1.425);

	  SetSkis(SKIS_IN);

	  AutoBalance();
	}

	if ( Length == 4 )
	{
	  DriveXSquares(-.45);

	  TurnClockwise90();

	  DriveXSquares(-1.25);

	  DriveXSquares(.005);

	  SetSkis(SKIS_OUT);

	  DriveXSquares(-1.425);

	  SetSkis(SKIS_IN);

	  DriveXSquares(-1.25);

	  TurnClockwise90();

	  SetArmElevation(RAISED);

	  if ( High == true )
	  {
	    SetBridge(APPROACH_HIGH);

	    SetArmElevation(LOWERED);

	    AlignWithDispenserID(HIGH_DISPENSER);

	    SetBridge(DISPENSE_HIGH);
	  }
	  else
	  {
	    SetBridge(APPROACH_LOW);

	    SetArmElevation(LOWERED);

	    AlignWithDispenserID(LOW_DISPENSER);

	    SetBridge(DISPENSE_LOW);
	  }

	  DispenseXBatons(15);
	}
}
