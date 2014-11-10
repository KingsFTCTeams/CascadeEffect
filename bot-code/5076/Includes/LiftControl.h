static bool liftMoving = false;
static int targetLoc = 0;
static int currPos = nMotorEncoder[Collector];

void UpdateLiftPos() {
	if(liftMoving) {
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
	else {
		int oldLoc = targetLoc;
		if(joy2Btn(BTN_LIFT_COLLECT)) {
			targetLoc = LIFT_COLLECT_ENCODER;
		}
		else if(joy2Btn(BTN_LIFT_HIGH)) {
			targetLoc = LIFT_HIGH_ENCODER;
		}
		else if(joy2Btn(BTN_LIFT_CENTER)) {
			targetLoc = LIFT_CENTER_ENCODER;
		}
		else if(joy2Btn(BTN_LIFT_LOW)) {
			targetLoc = LIFT_LOW_ENCODER;
		}
		else if(joy2Btn(BTN_LIFT_MID)) {
			targetLoc = LIFT_MID_ENCODER;
		}
		if(targetLoc != oldLoc) {
			liftMoving = true;
		}
	}
}
