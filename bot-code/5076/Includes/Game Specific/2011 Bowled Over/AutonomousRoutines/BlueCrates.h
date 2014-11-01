void startRightBlueCrateFront()
{
  DriveStraight(-1.4, 22);
  SwingTurn(44, TURN_RIGHT, BACKWARD);
  DriveStraight(-0.3, 33);
}

void startLeftBlueCrateFront()
{
  DriveStraight(-1.5, 16);
  PivotTurn(42);
  DriveStraight(-1.25, 40, COS_CURVE_PWR_RAMP);
}
