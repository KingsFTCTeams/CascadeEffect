tVelocityPIDinfo swerveSpeedPIDInfo;
tPosPIDinfo swervePosPIDInfo;

void _turnSwerveSystem(long goalAngle);
float calcTravelAngle(float joyAngle, float wheelAngle, float gyroAngle, float offset);

#define SWERVE_SYSTEM_PID_LOOP_TIME 20

void initSwerveMotor()
{
	swervePosPIDInfo.Kp = SWERVE_MOTOR_POS_KP;
	swervePosPIDInfo.Ki = SWERVE_MOTOR_POS_KI;
	swervePosPIDInfo.Kd = SWERVE_MOTOR_POS_KD;
	swervePosPIDInfo.goalPos = 0;
	swervePosPIDInfo.allowedError = SWERVE_MOTOR_ALLOWED_ERROR;
	swervePosPIDInfo.bAtGoal = true;
	swerveSpeedPIDInfo.Kp = SWERVE_MOTOR_SPEED_KP;
	swerveSpeedPIDInfo.Ki = SWERVE_MOTOR_SPEED_KI;
	swerveSpeedPIDInfo.Kd = SWERVE_MOTOR_SPEED_KD;
	swerveSpeedPIDInfo.goalVelocity = 0;
	swerveSpeedPIDInfo.prevValue = 0;
}


task swerveSystemTask()
{
	long pastLoopTime = 0;
	initSwerveMotor();
	nMotorEncoder[SPIN] = 0;
	while(true)
	{
		bDisplayDiagnostics = false;
		#ifndef DISABLE_SWERVE_DEBUG
			nxtDisplayCenteredTextLine(1, "Goal Angle %d", g_nSwerveSystemGoalAngle);
			nxtDisplayCenteredTextLine(2, "Curr Angle %d", ReadSwerveAngle());
			nxtDisplayCenteredTextLine(3, "Error Pos %d", swervePosPIDInfo.lastError);
			nxtDisplayCenteredTextLine(4, "Error Vel %d", swerveSpeedPIDInfo.goalVelocity);
		#endif
		_turnSwerveSystem(g_nSwerveSystemGoalAngle);
		while(nPgmTime - pastLoopTime < 20)
		{
			wait1Msec(5);
		}
		pastLoopTime = nPgmTime;
	}
}

void _turnSwerveSystem(long goalAngle)
{
	goalAngle = goalAngle * 4 * SWERVE_MOTOR_GEAR_RATIO;
	swervePosPIDInfo.currPos = nMotorEncoder[SWERVE_MOTOR];
	swervePosPIDInfo.goalPos = goalAngle;
	swerveSpeedPIDInfo.goalVelocity = calcPositionPID(swervePosPIDInfo);
	float velocity = calcVelocityPID(swerveSpeedPIDInfo);
	if(swervePosPIDInfo.bAtGoal)
	{
		g_bSwerveSystemAtGoal = true;
	}
	else
	{
		g_bSwerveSystemAtGoal = false;
	}
	motor[SWERVE_MOTOR] = velocity;
}

float calcTravelAngle(float joyAngle, float wheelAngle, float gyroAngle, float offset)
{
	long TD = (int)joyAngle - (int)wheelAngle + (int)offset - (int)gyroAngle;
	//	long TD = (int)joyAngle - (int)wheelAngle + (int)offset;

	//TD = (360 + TD) % 360;
	//if (TD > 180) {
	//	TD = -(360 - TD);
	//}

	while(TD>=180)
		TD-=360;
	while(TD<-180)
		TD+=360;

	g_swerveDriveDirection = 1;
	if (abs(TD) > 90) {
		TD = (180 - abs(TD)) * -sgn(TD);
		g_swerveDriveDirection = -1;
	}

	return TD;
}
