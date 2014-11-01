#pragma config(Sensor, S1,     GyroSensor,     sensorAnalogInactive)
#pragma config(Motor,  motorA,          turnMotor,     tmotorNXT, PIDControl, reversed, encoder)
#pragma config(Motor,  motorB,          driveMotor,    tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          driveMotor,    tmotorNXT, PIDControl, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//int GyroSensor = 0;

#define TEAM_NUMBER 5076

//temp

#define DEFAULT_SPEED_KP 0.014
#define DEFAULT_SPEED_KI 0.0019
#define DEFAULT_SPEED_KD 0.00002

#define DEFAULT_POS_KP 1.5
#define DEFAULT_POS_KI 0.0
#define DEFAULT_POS_KD 1.98

#define ARM_MOTOR_ONE mtr_S1_C1_1
#define ARM_MOTOR_TWO mtr_S1_C1_2

//end temp

#include "JoystickDriver.c"
#include "Includes/TeamConfig.h"
#include "../Common/GlobalDefines.h"
#include "../Common/Control/SensorTask.h"
#include "pidControlForSwerve.h"

float calcJoystickAngle(int xaxis, int yaxis); //function prototypes
int calcJoyPower(int xaxis, int yaxis);
float calcGoalAngle(float joyAngle, float gyroAngle, float wheelAngle);
void turnSwerveSystem(float goalAngle2, float joyPower2);

float joyAngle = 0.0;
int joyPower = 0;
float goalAngle = 0.0;
int driveDirection = 100;

task main()
{
	initArm();
	StartTask(SensorTask);
	StartTask(updateArm);
  eraseDisplay();
	nMotorEncoder[motorA] = 0;

  int yaxis = 0;
  int xaxis = 0;

  while (true) {
  	disableDiagnosticsDisplay();
  	getJoystickSettings(joystick);
	  yaxis = joystick.joy1_y1;
    xaxis = joystick.joy1_x1;

    joyAngle = calcJoystickAngle(xaxis, yaxis);
    /*if (joyAngle == 0) {
    	driveDirection = 0;
    }
    else {
    	driveDirection = -60;
    }*/
    joyPower = calcJoyPower(xaxis, yaxis);
    goalAngle = calcGoalAngle((float)joyAngle, Gyro.fAngle, -(float)nMotorEncoder[motorA]/4.2);
    turnSwerveSystem(goalAngle, (float)joyPower);
    nxtDisplayCenteredTextLine(2, "%f", joyAngle);
    nxtDisplayCenteredTextLine(4, "%f", Gyro.fAngle);
    nxtDisplayCenteredTextLine(6, "%f", goalAngle);

    wait1Msec(10); //buffer for debugger console
	}
}

float calcJoystickAngle(int xaxis, int yaxis) {
  float angle;
	if(abs(yaxis) > 10 || abs(xaxis) > 10) {
	  angle = atan(((float)yaxis/(float)xaxis));
	  angle = angle * (180.0/PI);
		if(yaxis >= 0 && xaxis >= 0) {
		  angle = 90.0 - abs(angle);
		}
		else if(yaxis <= 0 && xaxis >= 0) {
		  angle = abs(angle) + 90.0;
		}
		else if(yaxis <= 0 && xaxis <= 0) {
		  angle = 90.0 - abs(angle) + 180.0;
		}
		else if(yaxis >= 0 && xaxis <= 0) {
		  angle = abs(angle) + 270.0;
		}
		//convert 0<->360 range to -180<->180
		if (angle > 180) {
		  angle = (360.0-angle) * -1.0;
	  }
	  return angle;
  }
  else {
    return 0;
  }
}

float calcGoalAngle(float joyAngle, float gyroAngle, float wheelAngle) {
  float oldAlgorithm = ((gyroAngle + wheelAngle) * -1) + joyAngle; //returns the number of degrees needed acheive desired goal.
  //float newAlgorithm = (180 - oldAlgorithm) * -1;
  /*if (abs(oldAlgorithm) < abs(newAlgorithm)) {
    driveDirection = 100;
    return oldAlgorithm;
  }
  else {
    driveDirection = -100;
    return newAlgorithm;
  }*/
  writeDebugStreamLine("Goal: %4.1f,  Joy: %4.1f,  Gyro: %4.1f,  Wheel:  %4.1f", oldAlgorithm, joyAngle, gyroAngle, wheelAngle);
  return oldAlgorithm;
}

int calcJoyPower(int xaxis, int yaxis)
{
	float power = sqrt(pow(abs(xaxis),2) + pow(abs(yaxis),2));
	return (int)(power*0.78125); //translate max joystick value of 128 to max motor value of 100
}

void turnSwerveSystem(float goalAngle2, float joyPower2) {
	turnMotor1.currentGoalPosDegrees = goalAngle2;
	turnMotor1.currentGoalSpeedDegreesPerSecond = 360;

	motor[motorB] = joyPower2;
	motor[motorC] = joyPower2;
}