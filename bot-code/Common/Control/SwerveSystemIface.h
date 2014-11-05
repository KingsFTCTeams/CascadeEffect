//swerve motor configuration #defines
#define SWERVE_MOTOR_ALLOWED_ERROR 5*7
#define SWERVE_MOTOR_POS_KP 1.2
#define SWERVE_MOTOR_POS_KI 0.0
#define SWERVE_MOTOR_POS_KD 5.0
#define SWERVE_MOTOR_SPEED_KP 0.15
#define SWERVE_MOTOR_SPEED_KI 0.001
#define SWERVE_MOTOR_SPEED_KD 0.00002
#define SWERVE_MOTOR_GEAR_RATIO 7.0/1.0
#define SWERVE_MOTOR SPIN

float g_swerveSystemOffset = 0.0;
int g_swerveDriveDirection = 1;
long g_nSwerveSystemGoalAngle = 0;
bool g_bSwerveSystemAtGoal = true;



long ReadSwerveEncoder()
{
	return nMotorEncoder[SWERVE_MOTOR];
}

long ReadSwerveAngle()
{
	return ReadSwerveEncoder() / 4 / SWERVE_MOTOR_GEAR_RATIO;
}

#include "..\Common\SwerveSystem.h"

void moveSwerveToAngle(int gAngle)
{
	//writeDebugStreamLine("Goal Angle %d", gAngle);
	//writeDebugStreamLine("Gyro Angle %f", Gyro.fAngle);
	//writeDebugStreamLine("Real Angle: %d", ReadSwerveAngle());
	//writeDebugStreamLine("Offset Angle %d", g_swerveSystemOffset);
	g_nSwerveSystemGoalAngle = gAngle;
}

//takes an angle on the joystick and tuns the robot to it
void moveSwerveToJoyAngle(int gAngle)
{
	float travelAngle = 0.0;
	float wheelAngle = 0.0;
	wheelAngle = (float)nMotorEncoder[SPIN]/4.0/SWERVE_MOTOR_GEAR_RATIO;
	travelAngle = calcTravelAngle(gAngle, wheelAngle, Gyro.fAngle, g_swerveSystemOffset);
	moveSwerveToAngle((float)wheelAngle + (float)travelAngle);
	//moveSwerveToAngle(goalAngle);
}

void moveSwerveToAngleBlocking(int gAngle)
{
	g_bSwerveSystemAtGoal = false;
	swervePosPIDInfo.bAtGoal = false;
	g_nSwerveSystemGoalAngle = gAngle;
	wait1Msec(10);
	while(!swervePosPIDInfo.bAtGoal)
	{
		wait1Msec(5);
	}
}

void addToSwerveOffset(float ammount) {
	g_swerveSystemOffset += ammount;
	//writeDebugStreamLine("%f", g_swerveSystemOffset);
}

long ReadSwerveGoalAngle()
{
	return g_nSwerveSystemGoalAngle;
}
