void Basic_Testing_Auto()
{
	/*//DriveStraight(1, 100);
	PivotTurn(-90);
	//DriveStraight(2, 100);
	PivotTurn(90);*/
	while(true) {
		nxtDisplayCenteredBigTextLine(2, "IR: %d", ReadIR1());
	}
}

void MiddleDrop_Auto()
{
//	ArmJoint_MoveToPosAtRate(2, 250, 900);
//	ArmJoint_MoveToPosAtRate(1, 145, 180);
//	wait1Msec(2500);
//	DriveStraight(2, 100);
//	ArmJoint_MoveToPosAtRate(2, 10, 900);
//	ArmJoint_MoveToPosAtRate(1, 160, 180);
//	wait1Msec(2500);
//	DriveStraight(-1, 100);
//	ArmJoint_MoveToPosAtRate(2, 0, 900);
//	ArmJoint_MoveToPosAtRate(1, ARM_J1_JCS_OFFSET_DEG, 180);
//	wait1Msec(2500);
}

void AwesomeDefensnessright_Auto()
{

	DriveStraight(2.5, 100);
	SwingTurn(-45, TURN_LEFT, FORWARD);
	DriveStraight(2, 100);
	DriveStraight(-2, 100);
	DriveStraight(2.1, 100);
	PivotTurn(95);
}
void AwesomeDefensnessleft_Auto()
{

	DriveStraight(2.5, 100);
	SwingTurn(45, TURN_RIGHT, FORWARD);
	DriveStraight(2, 100);
	DriveStraight(-2, 100);
	DriveStraight(2.1, 100);
	PivotTurn(-40);
}
