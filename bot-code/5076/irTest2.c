#pragma config(Hubs,  S2, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S4, HTMotor,  HTServo,  none,     none)
#pragma config(Sensor, S1,     IRsensor1,      sensorI2CCustom)
#pragma config(Sensor, S3,     GyroSensor,     sensorI2CHiTechnicGyro)
#pragma config(Motor,  mtr_S2_C1_1,     Turntable,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C1_2,     LeftDrive,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C2_1,     ArmJ2,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C2_2,     ArmJ1,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S4_C1_1,     Nothing,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C1_2,     RightDrive,    tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S4_C2_1,    servo1,               tServoStandard)
#pragma config(Servo,  srvo_S4_C2_2,    servo2,               tServoStandard)
#pragma config(Servo,  srvo_S4_C2_3,    servo3,               tServoStandard)
#pragma config(Servo,  srvo_S4_C2_4,    servo4,               tServoStandard)
#pragma config(Servo,  srvo_S4_C2_5,    servo5,               tServoStandard)
#pragma config(Servo,  srvo_S4_C2_6,    servo6,               tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define TEAM_NUMBER 5076

#include "../Common/GlobalDefines.h"
#include "Includes/TeamConfig.h"
#include "../Common/Control/SensorTask.h"
task main()
{
	int irValue;
	while(true)
	{
		StartTask(SensorTask);
		irValue = ReadIR1();
		nxtDisplayTextLine(3, "%i", irValue);
	}
}
