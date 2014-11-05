typedef struct {
	//Speed PID Settings gain constants
	float speedKp;
	float speedKi;
	float speedKd;
	//Speed PID loop control Variables
	float speedI;
	float speedD;
	long speedError;
	long speedLastError;
	long currentTime;
	long lastTime;
	long deltaTime;
	long speedCurrentEncoderCount;
	long lastEncoderCount;
	long deltaEncoderCount;
	float currentTicksPerSecond;
	float goalTicksPerSecond;
  int speedOutput;
	//Positonal PID Settings
	float posKp;
	float posKi;
	float posKd;
	//Positional PID Variables
	float posI;
	float posD;
	long posError;
	long posLastError;
	long posCurrentEncoderCount;
	long goalPosTicks;
	int posOutput;
	long pGoal;
	tMotor motorPort;
	//actual position vars
	long currentGoalPosDegrees;
	long currentGoalSpeedDegreesPerSecond;

	bool bGoalReached;

} pidMotor;

//function definitions
void setMotorSpeedPID(pidMotor *pid, int goalDegreesPerSecond);
int calcPosPID(pidMotor *pid, int goalDegreesPerSecond, int goalPosDegrees);
void initPIDMotor(pidMotor *pid, tMotor motorPort, float posKp, float posKi,
												float posKd, float speedKp, float speedKi, float speedKd);

//create a new motor "pidMotor object"
pidMotor armMotor1;
pidMotor armMotor2;

//main loop for handling arm control
void updateArm() {
	setMotorSpeedPID(armMotor1, calcPosPID(armMotor1, armMotor1.currentGoalSpeedDegreesPerSecond, armMotor1.currentGoalPosDegrees));
	setMotorSpeedPID(armMotor2, calcPosPID(armMotor2, armMotor2.currentGoalSpeedDegreesPerSecond, armMotor2.currentGoalPosDegrees));
}

void initArm() {
	initPIDMotor(armMotor1, ARM_MOTOR_ONE, DEFAULT_POS_KP, DEFAULT_POS_KI, DEFAULT_POS_KD,
										DEFAULT_SPEED_KP, DEFAULT_SPEED_KI, DEFAULT_SPEED_KD);
	initPIDMotor(armMotor2, ARM_MOTOR_TWO, DEFAULT_POS_KP, DEFAULT_POS_KI, DEFAULT_POS_KD,
										DEFAULT_SPEED_KP, DEFAULT_SPEED_KI, DEFAULT_SPEED_KD);
	armMotor1.currentGoalSpeedDegreesPerSecond = 90;
	armMotor2.currentGoalSpeedDegreesPerSecond = 90;
}

void initPIDMotor(pidMotor *pid, tMotor motorPort, float posKp, float posKi,
												float posKd, float speedKp, float speedKi, float speedKd) {
	memset(pid, 0, sizeof(pidMotor));
	pid->posKp = posKp;
	pid->posKi = posKi;
	pid->posKd = posKd;
	pid->motorPort = motorPort;
	pid->speedKp = speedKp;
	pid->speedKi = speedKi;
	pid->speedKd = speedKd;
	pid->motorPort = motorPort;
	nMotorEncoder[motorPort] = 0;
}

void setMotorSpeedPID(pidMotor *pid, int goalDegreesPerSecond) {
	pid->currentTime = nPgmTime;
	pid->deltaTime = pid->currentTime - pid->lastTime;
	pid->lastTime = pid->currentTime;
	pid->speedCurrentEncoderCount = nMotorEncoder[pid->motorPort];
	pid->deltaEncoderCount = pid->speedCurrentEncoderCount - pid->lastEncoderCount;
	pid->lastEncoderCount = pid->speedCurrentEncoderCount;
	pid->currentTicksPerSecond = ((float)pid->deltaEncoderCount * (1000.0 / (float)pid->deltaTime));
	pid->goalTicksPerSecond = ((float)goalDegreesPerSecond * 4.0);
	pid->speedError = pid->goalTicksPerSecond - pid->currentTicksPerSecond;
	pid->speedI += pid->speedError;
	pid->speedD = (pid->speedError - pid->speedLastError);
	pid->speedLastError = pid->speedError;
	pid->speedOutput = (pid->speedError * pid->speedKp) + (pid->speedI * pid->speedKi) + (pid->speedD * pid->speedKd);
	motor[pid->motorPort] = (int)pid->speedOutput;
}

int calcPosPID(pidMotor *pid, int goalDegreesPerSecond, int goalPosDegrees) {
	if(goalPosDegrees != pid->pGoal) {
		pid->pGoal = goalPosDegrees;
		pid->bGoalReached = false;
	}
	pid->posCurrentEncoderCount = nMotorEncoder[pid->motorPort];
	pid->goalPosTicks = goalPosDegrees * 4;
	pid->posError = pid->goalPosTicks - pid->posCurrentEncoderCount;
	if(abs(pid->posError) > 12) {
		pid->posI += pid->posError;
		pid->posD = (pid->posError - pid->posLastError);
		pid->posLastError = pid->posError;
		pid->posOutput = (pid->posError * pid->posKp) + (pid->posI * pid->posKi) + (pid->posD * pid->posKd);
	} else {
		pid->posOutput = 0;
		pid->bGoalReached = true;
	}

	if(pid->posOutput > goalDegreesPerSecond && pid->posOutput > 0)
		pid->posOutput = goalDegreesPerSecond;

	else if(pid->posOutput < -goalDegreesPerSecond && pid->posOutput < 0)
		pid->posOutput = -goalDegreesPerSecond;

	//writeDebugStreamLine("Error: %d", pid->posError);
	return (int)pid->posOutput;
}
