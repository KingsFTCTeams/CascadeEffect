void startRightRedCrate()
{
  DriveStraight(-1.5, 60);
  SwingTurn(40, TURN_RIGHT, BACKWARD);
  DriveStraight(-0.2, 60);
}

void startRightRedCratePos2()
{
  DriveStraight(-1.9, 60);
  PivotTurn(-88);
  DriveStraight(-2.3, 60);
  SwingTurn(40, TURN_LEFT, BACKWARD);
}

void startLeftRedCrate()
{
  DriveStraight(-1.4, 22);
  SwingTurn(37, TURN_LEFT, BACKWARD);
  DriveStraight(-0.3, 33);
}
