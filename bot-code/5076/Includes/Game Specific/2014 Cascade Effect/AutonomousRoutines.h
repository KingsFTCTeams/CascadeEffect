void DriveOffRamp()
{
	nMotorEncoder[Lift] = 0;
	DriveStraight(3.0, 75);
	while(nMotorEncoder[Lift] < 12800) {
		motor[Lift] = 100;
	}
	motor[Lift] = 0;
	DriveStraight(0.6, 30);
	wait1Msec(1000);
	motor[Collector] = -100;
	wait1Msec(1500);
	motor[Collector] = 0;
	DriveStraight(-0.2, 30);
	while(nMotorEncoder[Lift] > 0) {
		motor[Lift] = -100;
	}
	motor[Lift] = 0;

	//PivotTurn(-90);

}
