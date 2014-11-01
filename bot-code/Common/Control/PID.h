typedef struct
{
	float Kp;
	float Ki;
	float Kd;

	float integral;
	float derivative;
	float lastError;

	long lastTime;
	float prevValue;
	float currValue;

	//in units/sec
	float goalVelocity;

} tVelocityPIDinfo;

typedef struct
{
	float Kp;
	float Ki;
	float Kd;

	float integral;
	float derivative;
	float lastError;

	long lastTime;
	float prevPos;
	float currPos;

	float allowedError;

	//in units
	float goalPos;

	bool bAtGoal;

} tPosPIDinfo;

float calcVelocityPID(tVelocityPIDinfo *pid)
{
	long currentTime = nPgmTime;
	long deltaTime = currentTime - pid->lastTime;
	pid->lastTime = currentTime;
	long deltaValue = pid->currValue - pid->prevValue;
	pid->prevValue = pid->currValue;
	float currVelocity = ((float)deltaValue * (1000.0 / (float)deltaTime));
	long speedError = pid->goalVelocity - currVelocity;
	writeDebugStreamLine("Error: %d", speedError);
	if(abs(pid->goalVelocity) <= 10) {
		pid->integral = 0;
		pid->derivative = 0;
		pid->lastError = 0;
	}
	else {
		pid->integral += speedError;
		pid->derivative = (speedError - pid->lastError);
		pid->lastError = speedError;
	}
	float velocityOutput = (speedError * pid->Kp) + (pid->integral * pid->Ki) + (pid->derivative * pid->Kd);
	return velocityOutput;
}

float calcPositionPID(tPosPIDinfo *pid)
{
	float output = 0;
	float posError = pid->goalPos - pid->currPos;
	pid->bAtGoal = false;
	//writeDebugStreamLine("Pos Error: %f", posError);
	if (abs(posError) > pid->allowedError)
	{
		pid->bAtGoal = false;
		pid->integral += posError;
		pid->derivative = (posError - pid->lastError);
		pid->lastError = posError;
		output = (posError * pid->Kp) + (pid->integral * pid->Ki) + (pid->derivative * pid->Kd);
	}
	else if (!pid->bAtGoal)
	{
		pid->bAtGoal = true;
	}
	return output;
}

float calcCombinedPID(tPosPIDinfo posPID, tVelocityPIDinfo vPID)
{
	vPID.goalVelocity = calcPositionPID(&posPID);
	return calcVelocityPID(&vPID);
}
