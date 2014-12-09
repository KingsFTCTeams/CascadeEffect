bool servoDown = false;
int goalServoPos = SERVO_STOW_POSIITON;
int extendServoPos = 0;
void UpdateCollector() {
	motor[Collector] = (joy2Btn(BTN_COLLECTOR_IN)) ? FORWARD_COLLECT_POWER : ((joy2Btn(BTN_COLLECTOR_OUT)) ? REVERSE_COLLECT_POWER : NO_POWER);
}

void UpdateServo() {
	if(joy1Btn(BTN_SERVO_DOWN)) {
		goalServoPos = SERVO_COLLECT_POSITION;
	}
	else if(joy1Btn(BTN_SERVO_UP) == 1) {
		goalServoPos = SERVO_STOW_POSIITON;
	}
	servo[LeftGrab] = goalServoPos;
}

void UpdateExtention() {
	if(BTN_PLATFORM_OUT) {      //CHOOSE BUTTON
		extendServoPos = 225;
	}
	else if(BTN_PLATFORM_IN) { //CHOOSE BUTTON
		extendServoPos = -255;
	}
	servo[PlatformServo] = extendServoPos;
}
