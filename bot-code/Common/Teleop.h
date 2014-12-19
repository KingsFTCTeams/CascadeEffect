////////////////////////////////////////////////////////////////////////
//
// See TeamConfig.h for controller setup
//

////////////////////////////////////////////////////////////////////////
//
// TIMER USAGE
//
//   none
//

#include "../Common/GlobalDefines.h"
#include "JoystickDriver.c"
//#include "Drivers/JoystickDriverWithLostFCSCheck.h"
//#include "Drivers/JoystickDriverWithLostFCSCheck2012.h"
#include "Includes/MotorControl.h"
#include "Includes/CollectorConfig.h"
#include "CollectorControl.h"
#include "Includes/LiftControlConfig.h"
#include "Includes/LiftControl.h"

//function definitions
void InitializeRobot();
void SetDrive();

tMovementDir tDriveTeamDirectionPreference = DRIVE_TEAM_DIRECTION_PREFERENCE;

/////////////////////////////////////////////////////////////////////

task main() {
  InitializeRobot();

	#if defined(DISPLAY_CRATE_TEST_DIAGS) || defined(DISPLAY_SCISSOR_TEST_DIAGS) || defined(DISPLAY_DRIVE_DIAGS)
	  // Turn off normal screen diagnostics so can see subsystem diagnostics
	  bDisplayDiagnostics = false;
	  eraseDisplay();
	#endif

  waitForStart();

	//main teleop loop
	eraseDisplay();
  while (true) {
    getJoystickSettings(joystick);
    SetDrive();
    ManualLiftControl();
    UpdateCollector();
    UpdateLiftPos();
    UpdateServo();
    UpdateExtention();
    UpdateCenterServo();
    wait1Msec(5);
  }
}

/////////////////////////////////////////////////////////////////////

void InitializeRobot() {
	bDisplayDiagnostics = false;
	//nMotorEncoder[Lift] = 0; //uncomment when ready to runn
  //run any code required to initalize the robot here
	eraseDisplay();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Drive system
//
//    Joystick controlled ... scaling is handled in ScaleJoystickInput()
//
//    Normal mode is "half power" mode.  Must hold down "turbo" button to go full speed
//
//    Also have "drive fwd" and "drive bwd" buttons
//
void SetDrive() {
  float fLeftDrivePower  = ScaleJoystickInput(joystick.joy1_y1);
  float fRightDrivePower = ScaleJoystickInput(joystick.joy1_y2);

  if (joy1Btn(BTN_DRIVE_STRAIGHT_FWD)) {
    fLeftDrivePower = DRIVE_STRAIGHT_FWD_PWR;
    fRightDrivePower = DRIVE_STRAIGHT_FWD_PWR;
  } else if (joy1Btn(BTN_DRIVE_STRAIGHT_REV)) {
    fLeftDrivePower = DRIVE_STRAIGHT_REV_PWR;
    fRightDrivePower = DRIVE_STRAIGHT_REV_PWR;
  }

  // Normal mode is "half power" mode ... overridden with TURBO button
  //fLeftDrivePower  *= DRIVE_HALF_SPEED_SCALE_FACTOR;
  //fRightDrivePower *= DRIVE_HALF_SPEED_SCALE_FACTOR;

  if (joy1Btn(BTN_DRIVE_TURBO_PWR)) {
    // Actually dividing the motor power in half will not necessarily produce half power...
    //fLeftDrivePower  /= DRIVE_HALF_SPEED_SCALE_FACTOR;
    //fRightDrivePower /= DRIVE_HALF_SPEED_SCALE_FACTOR;

    //Switched. Turbo button is now a half-speed button that applies half power
  	fLeftDrivePower  *= DRIVE_HALF_SPEED_SCALE_FACTOR;
  	fRightDrivePower *= DRIVE_HALF_SPEED_SCALE_FACTOR;
  }

  if ( tDriveTeamDirectionPreference == FORWARD ) {
	  ApplyMotorPower(LEFT_SIDE,  (int)fLeftDrivePower);
	  ApplyMotorPower(RIGHT_SIDE, (int)fRightDrivePower);
  } else {
    // Reverse motor sides so that pushing the joystick forward actually drives the bot backwards
	  ApplyMotorPower(RIGHT_SIDE,-(int)fLeftDrivePower);
	  ApplyMotorPower(LEFT_SIDE, -(int)fRightDrivePower);
  }
}
