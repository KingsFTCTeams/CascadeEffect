bool servoDown = false;
int goalBackServoPos = SERVO_BACK_STOW_POSIITON;
int goalFrontServoPos = SERVO_FRONT_STOW_POSITION;
int extendServoPower = 0;
void UpdateCollector() {
	motor[Collector] = (joy2Btn(BTN_COLLECTOR_IN)) ? FORWARD_COLLECT_POWER : ((joy2Btn(BTN_COLLECTOR_OUT)) ? REVERSE_COLLECT_POWER : NO_POWER);
}

void UpdateServo() {
	if(joy1Btn(BTN_SERVO_BACK_DOWN)) {
		goalBackServoPos = SERVO_BACK_COLLECT_POSITION;
	}
	else if(joy1Btn(BTN_SERVO_BACK_UP) == 1) {
		goalBackServoPos = SERVO_BACK_STOW_POSIITON;
	}
	servo[BackGrab] = goalBackServoPos;

	if(joy1Btn(BTN_SERVO_FRONT_DOWN)){
		goalFrontServoPos = SERVO_FRONT_COLLECT_POSITION;
	}
	else if(joy1Btn(BTN_SERVO_FRONT_UP)){
		goalFrontServoPos = SERVO_FRONT_STOW_POSITION;
	}
	servo[FrontGrab] = goalFrontServoPos;
}

void UpdateExtention() {
	int joyReadingY = joystick.joy2_y2;

	if(joyReadingY > 10){
		extendServoPower = 255;
  }
  else if(joyReadingY < -10){
  	extendServoPower = 0;
  }
  else{
  	extendServoPower = 128;
  }
	servo[PlatformServo] = extendServoPower;
}
