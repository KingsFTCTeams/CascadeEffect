#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     GyroSensor,     sensorI2CCustom)
#pragma config(Motor,  motorA,          ,              tmotorNormal, openLoop)
#pragma config(Motor,  motorB,          RightCrateRotate, tmotorNormal, openLoop)
#pragma config(Motor,  motorC,          LeftCrateRotate, tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     DriveRight,    tmotorNormal, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     DriveLeft,     tmotorNormal, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     ScissorRight,  tmotorNormal, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     ScissorLeft,   tmotorNormal, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     CrateLift,     tmotorNormal, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     BallCollection, tmotorNormal, openLoop, reversed)
#pragma config(Servo,  srvo_S2_C1_1,    BallDispense,         tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    RightCrateGrip,       tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    LeftCrateGrip,        tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    ChimneyLatch,         tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    CrateClaw,            tServoStandard)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define TEAM_NUMBER 3717

#include "..\Common\Autonomous Template.h"
#include "..\Common\menu.h"

task main()
{
  InitializeRobot("Cust Auto");

  if(initMenuAndParse())
  {
	  parse(PROGRAM_SIZE);
  }
  else
  {
    PlayImmediateTone(1440, 150);
    nxtDisplayCenteredBigTextLine(2, "Error!");
    nxtDisplayCenteredBigTextLine(3, "Exiting...");
    wait1Msec(2000);
  }
}
