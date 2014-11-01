#pragma config(Hubs,  S3, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S4, HTMotor,  HTServo,  none,     none)
#pragma config(Sensor, S3,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S3_C1_1,     Turntable,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S3_C1_2,     RightDrive,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S3_C2_1,     ArmJ2,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S3_C2_2,     ArmJ1,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C1_1,     Nothing,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C1_2,     LeftDrive,     tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S4_C2_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S4_C2_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S4_C2_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S4_C2_4,    servo4,               tServoStandard)
#pragma config(Servo,  srvo_S4_C2_5,    servo5,               tServoStandard)
#pragma config(Servo,  srvo_S4_C2_6,    servo6,               tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define TEAM_NUMBER 5076

#pragma systemFile            // this eliminates warning for "unreferenced" functions

#include "../Common/GlobalDefines.h"
#include "JoystickDriver.c"
#include "Includes/TeamConfig.h"
#include "../Common/Control/ArmControl.h"

task main()
{
	long pTime = 0;
	int motorGoalSpdJ1 = 0;
	float motorGoalSpdJ2 = ARM_J2_JCS_OFFSET_DEG;
	float motorGoalSpd = 0;
	int servoGoalPos = 0;
	int servoDelta = 35;
	servo[servo4] = 0;
	servo[servo6] = 180;

	eraseDisplay();

	nxtDisplayTextLine(3, "initializing...");
	/*
	nxtDisplayTextLine(5, "0");
	servo[motorJ4] = 0;
	wait1Msec(5000);

	nxtDisplayTextLine(5, "90");
	servo[motorJ4] = 255/2;
	wait1Msec(5000);

	nxtDisplayTextLine(5, "190");
	servo[motorJ4] = 255;
	wait1Msec(5000);

	ArmJoint_MoveToPosAtRate(1, 30000, 90.0);
	ArmJoint_MoveToPosAtRate(2, 30000, 90.0);
	ArmJoint_MoveToPosAtRate(3, 30000, 90.0);

	*/
	StartTask(ArmControlTask);
	//wait1Msec(5000);

	while (true)
	{
		getJoystickSettings(joystick);
		long g1 = ReadArmJointGoal(1);
		long g2 = ReadArmJointGoal(2);
		long g3 = ReadArmJointGoal(3);
		long e1 = ReadJointEncoder(1);
		long e2 = ReadJointEncoder(2);
		long e3 = ReadJointEncoder(3);
		nxtDisplayTextLine(0, "J1 goal: %d", e1);
		nxtDisplayTextLine(1, "  error: %d", g1-e1);
		nxtDisplayTextLine(2, "J2 goal: %d", e2);
		nxtDisplayTextLine(3, "  error: %d", g2-e2);
		nxtDisplayTextLine(4, "J3 goal: %d", servoGoalPos);
		nxtDisplayTextLine(5, "  error: %d", g3-e3);
		nxtDisplayTextLine(6, "J4/5 delta: %d", servoDelta);


		if (joystick.joy1_y1 > 10)
		{
			motorGoalSpdJ1 = 40;
		}
		else if (joystick.joy1_y1 < -10)
		{
			motorGoalSpdJ1 = -40;
		}
		else
		{
			motorGoalSpdJ1 = 0;
			ArmJoint_Hold(1);
		}


		if (joystick.joy1_y2 > 10)
		{
			motorGoalSpdJ2 = 40;
		}
		else if (joystick.joy1_y2 < -10)
		{
			motorGoalSpdJ2 = 40;
		}
		else
		{
			ArmJoint_Hold(2);
			motorGoalSpdJ2 = 0;
		}


		if (joy1Btn(02))//claw open
		{
			servo[servo4] += 5;
			servo[servo6] -= 5;
		}


		if (joy1Btn(01))//claw closed
		{
			servo[servo4] -= 5;
			servo[servo6] += 5;
		}


		if (joystick.joy1_TopHat == 0)//wrist up
		{
			servo[servo5] += 5;
		}


		if (joystick.joy1_TopHat == 4)//wrist down
		{
			servo[servo5] -= 5;
		}

		if (joy1Btn(07))//turntable
		{
			motor[Turntable] = 20;
		}
		else if (joy1Btn(08))
		{
			motor[Turntable] = -20;
		}
		else
		{
			motor[Turntable] = 0;
		}


		/*
		if(joystick.joy2_y2 <= 8 && joystick.joy2_y2 >= -8)
		{
			motor[RightDrive] = 0;
		}
		else
		{
			motor[RightDrive] = joystick.joy2_y2*-1;
		}
		if(joystick.joy1_y1 <= 8 && joystick.joy2_y1 >= -8)
		{
			motor[LeftDrive] = 0;
		}
		else
		{
			motor[LeftDrive] = joystick.joy2_y1*1;
		}*/





		ArmJoint_MoveToPosAtRate(1, 900, motorGoalSpdJ1);
		ArmJoint_MoveToPosAtRate(2, 10, motorGoalSpdJ2);


		wait1Msec(5);
	}
}