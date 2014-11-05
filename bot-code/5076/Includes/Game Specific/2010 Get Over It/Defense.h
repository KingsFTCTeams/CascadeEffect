void Defense();

void Defense()
{
  int Side = 1;

  nxtDisplayCenteredBigTextLine(2, "Select");
  nxtDisplayCenteredBigTextLine(4, "Dispenser");

  wait1Msec(1000);

  eraseDisplay();

  nxtDisplayCenteredBigTextLine(3, "Center?");

  while ( nNxtButtonPressed != 3 )
  {
    if ( nNxtButtonPressed == 1 && time1[T1] > 500 && Side == 1 )
	  {
	    time1[T1] = 0;
	    Side++;
	  }
	  else if ( nNxtButtonPressed == 2 && time1[T1] > 500 && Side == 2 )
	  {
	    time1[T1] = 0;
	    Side--;
	  }

	  if ( Side == 1 )
	  {
	    nxtDisplayCenteredBigTextLine(3, "Center?");
	  }
	  else if ( Side == 2 )
	  {
	    nxtDisplayCenteredBigTextLine(3, "Side?");
	  }

	  wait1Msec(25);
	  eraseDisplay();
	}

	nxtDisplayCenteredBigTextLine(3 , "CONFIRMED");

	wait1Msec(1000);

	eraseDisplay();

	if ( Side == 2 )
	{
	  nxtDisplayCenteredBigTextLine(2, "FACE");
	  nxtDisplayCenteredBigTextLine(4, "BACKWARDS");

	  wait1Msec(1500);

	  eraseDisplay();
	}

  waitForStart();

  ///////////////////////// Insert Autonomous directions here //////////////////////

  if ( Side == 1 )
  {
    DriveXSquares(2.5);
  }
  else if ( Side == 2 )
  {
    DriveXSquares(-1.25);
  }

  ///////////////////////////////////////////////////////////////////////////////////

}
