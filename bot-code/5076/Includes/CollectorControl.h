bool servoDown = false;

void UpdateCollector() {
	motor[Collector] = (joy2Btn(BTN_COLLECTOR_IN)) ? FORWARD_COLLECT_POWER : ((joy2Btn(BTN_COLLECTOR_OUT)) ? REVERSE_COLLECT_POWER : NO_POWER);
}

void UpdateServo() {
	if(joy1Btn(BTN_SERVO)) {
		if(servoDown) {
			servo[LeftGrab] = 255;
			servo[RightGrab] = 255;
		}
		else {
			servo[LeftGrab] = 0;
			servo[RightGrab] = 0;
		}
		servoDown = !servoDown;
	}
}
