void DriveOffRamp()
{
	nMotorEncoder[Lift] = 0;
	DriveStraight(3.0, 75);
	while(nMotorEncoder[Lift] < 13000) {
		motor[Lift] = 100;
	}
	motor[Lift] = 0;
	servo[PlatformServo] = 198;
	wait1Msec(2250);
	servo[PlatformServo] = 127;
	motor[Collector] = -100;
	wait1Msec(1500);
	motor[Collector] = 0;
	servo[PlatformServo] = 64;
	wait1Msec(2250);
	servo[PlatformServo] = 127;
	DriveStraight(-0.2, 30);
	while(nMotorEncoder[Lift] > 0) {
		motor[Lift] = -100;
	}
	motor[Lift] = 0;

	//PivotTurn(-90);

}

void KnockBallsFromGround(){
	nMotorEncoder[Lift] = 0;
	motor[Lift] = 0;
	DriveStraight(0.8, 50);
	PivotTurn(-90);
	DriveStraight(2, 50);
	PivotTurn(90);
	DriveStraight(1, 50);

	//from here, find pos for robot to read IR
	//add cases for each IR position


}

//void KnockBallsFromRamp(){
//}
//void ScoreIRFromGround(){
//}
//void ScoreIRFromRamp(){
//}
