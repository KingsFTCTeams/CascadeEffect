bool liftMoving = false;
int targetLoc = 0;

int CurrLiftPos() {
	return nMotorEncoder[Lift];
}

bool isLiftMoving() {
	return liftMoving;
}

void setTarget(int target) {
	if((target >= LIFT_COLLECT_ENCODER && target <= LIFT_CENTER_ENCODER) && !liftMoving) {
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
			nxtDisplayCenteredTextLine(6, "Power = 0");
		}
		else {
			if(abs(error) > LIFT_TARGET_ERROR) {
				int power = sgn(error)*LIFT_MOTOR_POWER;
				motor[Lift] = power;
			}
			else {
				motor[Lift] = 0;
				liftMoving = false;
			}
		}

	}
	else {
		if(joy2Btn(BTN_LIFT_COLLECT)) {
			setTarget(LIFT_COLLECT_ENCODER);
			liftMoving = true;
		}
		else if(joy2Btn(BTN_LIFT_HIGH)) {
			setTarget(LIFT_HIGH_ENCODER);
			liftMoving = true;
		}
		//else if(joy2Btn(BTN_LIFT_CENTER)) {
			//setTarget(LIFT_CENTER_ENCODER);
			//liftMoving = true;
		//}
		else if(joy2Btn(BTN_LIFT_LOW)) {
			setTarget(LIFT_LOW_ENCODER);
			liftMoving = true;
		}
		else if(joy2Btn(BTN_LIFT_MID)) {
			setTarget(LIFT_MID_ENCODER);
			liftMoving = true;
		}
	}
}

void ManualLiftControl() {
	if(!isLiftMoving()) {
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
}

int isLiftMoving() {
	return liftMoving ? 1 : 0;
}
