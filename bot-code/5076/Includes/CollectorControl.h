bool servoDown = false;
int goalServoPos = SERVO_STOW_POSIITON;
int extendServoPower = 0;
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
	int joyReadingY = ScaleJoystickInput(joystick.joy2_y2);

	if(joyReadingY > 0){
		extendServoPower = 255;
  }
  else if(joyReadingY < 0){
  	extendServoPower = 0;
  }
  else{
  	extendServoPower = 127;
  }
	servo[PlatformServo] = extendServoPower;
}
