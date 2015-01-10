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

void IRAutoNine() {
	//eraseDisplay();
	//nxtDisplayCenteredBigTextLine(3, "9");
	while(abs(nMotorEncoder[Lift]) < 18507) {
		motor[Lift] = 100;
	}
	motor[Lift] = 0;
	PivotTurn(6);
	DriveStraight(-0.44);
	wait1Msec(2000);
	servo[CenterServo] = 20;
	wait1Msec(2000);
}

void IRAutoSeven() {
	PivotTurn(-45);
	DriveStraight(-0.75, 75);
	PivotTurn(90);
	//servo[PlatformServo] = 198;
	//wait1Msec(1000);
	//servo[PlatformServo] = 127;
	while(abs(nMotorEncoder[Lift]) < 18507) {
		motor[Lift] = 100;
	}
	motor[Lift] = 0;
	DriveStraight(-0.44);
	wait1Msec(1000);
	servo[CenterServo] = 20;
	wait1Msec(2000);
}

void FinalIR() {
	PivotTurn(-45);
	DriveStraight(-1.0, 75);
	PivotTurn(45);
	DriveStraight(-0.75, 75);
	PivotTurn(66);
	while(abs(nMotorEncoder[Lift]) < 18507) {
		motor[Lift] = 100;
	}
	motor[Lift] = 0;
	DriveStraight(-0.23, 75);
	wait1Msec(1000);
	servo[CenterServo] = 20;
	wait1Msec(2000);
}

void IRAuto() {
	eraseDisplay();
	nMotorEncoder[Lift] = 0;
	//SwingTurn(30, TURN_RIGHT, BACKWARD);
	DriveStraight(-1, 75);
	eraseDisplay();
	while(abs(nMotorEncoder[Lift]) < 5507) {
		motor[Lift] = 100;
		nxtDisplayCenteredTextLine(3, "Lift: %d", abs(nMotorEncoder[Lift]));
	}
	//nxtDisplayCenteredTextLine(3, "DONE!");
	motor[Lift] = 0;
	int irPos = ReadIR1();
	//while(true) {
		//nxtDisplayCenteredBigTextLine(3, "IR: %d", ReadIR1());
	//}
	if(irPos == 9) {
		IRAutoNine();
	}
	else if(irPos == 7) {
		IRAutoSeven();
	}
	else {
		FinalIR();
	}
}

//void KnockBallsFromRamp(){
//}
//void ScoreIRFromGround(){
//}
//void ScoreIRFromRamp(){
//}
