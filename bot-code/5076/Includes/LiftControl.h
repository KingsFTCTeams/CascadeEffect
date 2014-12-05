bool liftMoving = false;
int targetLoc = 0;

bool isLiftMoving() {
	return liftMoving;
}

void setTarget(int target) {
	if(target >= LIFT_COLLECT_ENCODER && target <= LIFT_CENTER_ENCODER && !liftMoving) {
		targetLoc = target;
	}
}

void UpdateLiftPos() {
	if(isLiftMoving()) {
		int currPos = nMotorEncoder[Lift];
		int error = targetLoc - currPos;
		if((error < 0 && nMotorEncoder[Lift] <= 0) || (error > 0 && nMotorEncoder[Lift] >= 19000)) {
			motor[Lift] = 0;
			liftMoving = false;
		}
		else {
			if(abs(error) > LIFT_TARGET_ERROR) {
				motor[Lift] = sgn(error)*LIFT_MOTOR_POWER;
				nxtDisplayCenteredTextLine(2, "Curr Pos: %d", currPos);
				nxtDisplayCenteredTextLine(3, "Error: %d", error);
			}
			else {
				motor[Lift] = 0;
				liftMoving = false;
			}
		}
		nxtDisplayCenteredTextLine(6, "Power: %d", sgn(error)*LIFT_MOTOR_POWER);
	}
	else {
		int oldLoc = targetLoc;
		if(joy2Btn(BTN_LIFT_COLLECT)) {
			setTarget(LIFT_COLLECT_ENCODER);
		}
		else if(joy2Btn(BTN_LIFT_HIGH)) {
			setTarget(LIFT_HIGH_ENCODER);
		}
		else if(joy2Btn(BTN_LIFT_CENTER)) {
			setTarget(LIFT_CENTER_ENCODER);
		}
		else if(joy2Btn(BTN_LIFT_LOW)) {
			setTarget(LIFT_LOW_ENCODER);
		}
		else if(joy2Btn(BTN_LIFT_MID)) {
			setTarget(LIFT_MID_ENCODER);
		}
		if(targetLoc != oldLoc) {
			liftMoving = true;
		}
	}
	nxtDisplayCenteredTextLine(1, "Target: %d", targetLoc);
	nxtDisplayCenteredTextLine(4, "Moving: %d", isLiftMoving());
}

void ManualLiftControl() {

	if(nMotorEncoder[Lift] > 0 && joystick.joy2_y1 < -10) {
		motor[Lift] = joystick.joy2_y1;
	}
	else if(joystick.joy2_y1 > 10 && nMotorEncoder[Lift] < 20000){
		motor[Lift] = joystick.joy2_y1;
	}
	else {
	 motor[Lift] = 0;
	}
}

int CurrLiftPos() {
	return nMotorEncoder[Lift];
}

int isLiftMoving() {
	return liftMoving ? 1 : 0;
}
