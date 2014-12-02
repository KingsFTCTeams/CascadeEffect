bool servoDown = false;
int servoPos = 60;
void UpdateCollector() {
	motor[Collector] = (joy2Btn(BTN_COLLECTOR_IN)) ? FORWARD_COLLECT_POWER : ((joy2Btn(BTN_COLLECTOR_OUT)) ? REVERSE_COLLECT_POWER : NO_POWER);
}

void UpdateServo() {
	if(joy1Btn(BTN_SERVO)) {
			servoPos = 225;
	}
	else if(joy1Btn(BtnRT) == 1) {
			servoPos = 60;
		}
		servo[LeftGrab] = servoPos;
		//nxtDisplayCenteredTextLine(3, "Servo: %d", servo[LeftGrab]);
		//servoDown = !servoDown;
	}
