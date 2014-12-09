bool servoDown = false;
int goalServoPos = SERVO_STOW_POSIITON;
//int extendServoPos = 0;
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

/*void UpdateExtention() {
	if(joy2Btn() == 1) {      //CHOOSE BUTTON
		extendServoPos = 225
	}
	else if(joy2Btn() == 1) { //CHOOSE BUTTON
		extendServoPos = 0;
	}
	//servo[] = extendServoPos;
}*/
