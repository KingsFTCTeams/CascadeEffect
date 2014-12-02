bool liftMoving = false;
int targetLoc = 0;

bool isLiftMoving() {
	return liftMoving;
}

void setTarget(int target) {
	if(target > LIFT_COLLECT_ENCODER && target < LIFT_CENTER_ENCODER && !liftMoving) {
		targetLoc = target;
	}
}

void UpdateLiftPos() {
	if(isLiftMoving()) {
		if(nMotorEncoder[Lift] <= 0 || nMotorEncoder[Lift] >= 20000) {
			motor[Lift] = 0;
			liftMoving = false;
		}
		else {
			int currPos = nMotorEncoder[Collector];
			int error = targetLoc - currPos;
			if(abs(error) > LIFT_TARGET_ERROR) {
				motor[Lift] = sgn(error)*LIFT_MOTOR_POWER;
			}
			else {
				motor[Lift] = 0;
				liftMoving = false;
			}
		}
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
