//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes:
//
//    1) JoystickDriver.c - Handles bluetooth and joystick commands
//
//    2) MagneticBaton.h - Handles detection of magnetic baton and indication to drivers

#include "Includes\teamConfig.h"

#undef READ_RAW_ARM_ENCODER_VALUES
#undef USE_ARM_ENCODER_SPIKE_PROTECTION

#define READ_RAW_ARM_ENCODER_VALUES

#include "..\Common\GlobalDefines.h"

#include "RobotC Drivers v1.8.1\drivers\common.h"

#include "..\Common\Drivers\JoystickDriverWithLostFCSCheck.h"
//#include "..\Common\Drivers\JoystickDriver.h"

#define DriveIsStalled() ( false ) // provided in place of DriveStallProtect.h

#include "..\Common\Control\EncoderTask.h"
#include "..\Common\Control\SensorTask.h"
#include "..\Common\Control\Indicator.h"
#include "..\Common\Control\MagneticBaton.h"
#include "..\Common\Control\AlertsMonitor.h"

/////////////////////////////////////////////////////////////////////////////
// Initialization Routine:

void InitializeRobot()
{
  wait1Msec(500);

  SetAbortAutonomous(false);

  StartTask(EncoderTask);

  StartTask(SensorTask);

  StartTask(DetectMagnetTask);

  StartTask(AlertsMonitorTask);

  servo[Left_Ski] = 255;  // Reset skis
  servo[Right_Ski] = 0;

  servo[ScoringShields] = 130;  // Reset scoring shields

  servo[WristControl] = 45;   // Reset scoop to horizontal
}

/////////////////////////////// Drive System ////////////////////////////////

int Lmotor  = 0;
int Rmotor = 0;

void DriveSystemUpdate()
{
  // Drive Power Calculations
  if ( abs(joystick.joy1_y2) > 10 )
  {
    Rmotor = joystick.joy1_y2 * .78;
  }
  else
  {
    Rmotor = 0;
  }

  if ( abs(joystick.joy1_y1) > 10)
  {
    Lmotor = joystick.joy1_y1 * .78;
  }
  else
  {
    Lmotor = 0;
  }

  motor[motorD] = Lmotor;
  motor[motorH] = Lmotor;
  motor[motorE] = Rmotor;
  motor[motorI] = Rmotor;

  //////////// Bridge Ski Control /////////////////

  if (joy1Btn(8))
  {
    servo[Left_Ski] = 115;
    servo[Right_Ski] = 140;
  }
  else
  {
    servo[Left_Ski] = 255;
    servo[Right_Ski] = 0;
  }
}

///////////////////////////////// Arm Control //////////////////////////////

int ARMMOTOR = 0;

void ArmControlUpdate ()
{
  static bool RESET = false;
  static int Encoder_Target = 0;

  if ( joy2Btn(10) && RESET == false )
  {
    time1[T1] = 0;
    RESET = true;   // state that arm is in reset mode

    Encoder_Target = ArmRotEncoderVal();

    nMotorEncoderTarget[motorF] = -Encoder_Target;  //
                                                    //  Determine which way is
    if ( Encoder_Target > 0 )                       //  closest to zero position
    {                                               //
      motor[motorF] = -20;                           //
    }
    else if ( Encoder_Target < 0 )
    {
      motor[motorF] = 20;
    }
  }

  if ( RESET == true )    // if in reset mode
  {
    servo[WristControl] = 45;  // reset position of shovel arm

    if ( joy2Btn(9) || time1[T1] > 1500 || abs(ArmRotEncoderVal()) < 30)
    {
      motor[motorF] = 0;
      RESET = false;
    }
  }

  if ( RESET == false ) // only allow normal arm action if not resetting
  {
	  // open/close scoring shields on arm
	  if ( joy2Btn(6) )
	  {
	    servo[ScoringShields] = 15;
	  }
	  else
	  {
	    servo[ScoringShields] = 130;
	  }

	  // wrist control
	  if (joy2Btn(3))
	  {
	    if (ServoValue[WristControl] < 255)
	    {
	      servo[WristControl] = ServoValue[WristControl] + 1;
	    }
	  }
	  else if (joy2Btn(1))
	  {
	    if (ServoValue[WristControl] > 0 )
	    {
	      servo[WristControl] = ServoValue[WristControl] - 1;
	    }
	  }

	  // motor elbow control ( encoder based )
	  if (joystick.joy2_y1 < -10)
	  {
	    ARMMOTOR = 12;
	  }

	  else if (joystick.joy2_y1 > 10)
	  {
	    // up with force
	    if (ARMMOTOR > -30 * 16)
	    {
	      if (ARMMOTOR == 0)
	      {
	        ARMMOTOR = -10 * 16;
	      }
	      else
	      {
	        ARMMOTOR = ARMMOTOR - 32;
	      }
	    }
	  }

	  else
	  {
	    // stay
	    ARMMOTOR = 0;
	  }

	  // Twist Control
	  if ( abs(joystick.joy2_x2) > 10 )
	  {
	    motor[motorF] = joystick.joy2_x2 * -.25;
	  }
	  else
	  {
	    motor[motorF] = 0;
	  }

	  motor[motorG] = ARMMOTOR / 12;
  }
}

/////////////////////////////// Bridge Control /////////////////////////////

#define SERVO1_UP  240
#define SERVO2_UP  15
//delta of 190
int servoAngle = 0;
bool AgitatorRunning = false;

void BridgeControlUpdate ()
{
  if (joy1Btn(10)) // resets bridge to up position
  {
    servoAngle = 0;
  }
  else if (joy1Btn(7) && servoAngle < SERVO1_UP)// brings down bridge
  {
    servoAngle = servoAngle + 1;
  }
  else if (joy1Btn(5) && servoAngle > SERVO2_UP) //brings bridge up
  {
    servoAngle = servoAngle - 1;
  }

  if (joy1Btn(1)) //set bridge for middle goal
  {
    servoAngle = 152;
  }

  if (joy1Btn(2)) //set bridge for low goal
  {
    servoAngle = 175;
  }
  if (joy1Btn(4)) //set bridge for high goal
  {
    servoAngle = 129;
  }

  servo[RightBridge] = SERVO1_UP - servoAngle;
  servo[LeftBridge] = SERVO2_UP + servoAngle;


	if ( DetectedMagnet() == false )  // IF (not a magnetic baton) OR (it is but the override button is pressed) ...
	{
	  if ( joy1Btn(6) && AgitatorRunning == false )
	  {
	       AgitatorRunning = true;
	       nMotorEncoder[motorC] = 0; // start the agitator fwd
	       motor[motorC] = 100;
	  }

	  if ( joy1Btn(3) ) // agitator reverse button
	  {
	    AgitatorRunning = true;
	    motor[motorC] = 0;
	    nMotorEncoder[motorC] = 0;   // start the agititor bwd
	    motor[motorC] = -100;
	  }

	  if (AgitatorRunning == true  && abs(nMotorEncoder[motorC]) > 300)		// Take care of detecting magnet while agitator running
	  {
	    motor[motorC] = 0;           // have turned far enough to dispense on baton, stop the agitator
	    AgitatorRunning = false;
	  }
	}

	else
	{
		motor[motorC] = 0;             // have detected a magnetic baton, stop agitating
		AgitatorRunning = false;
	}
}
