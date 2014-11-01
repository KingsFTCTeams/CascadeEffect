///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// TASKS
//
//    main
//
//    SensorValuesTask      - provides all sensor value readings including drive encoders
//    AlertsMonitorTask     - monitors alerts and uses lights to indicate
//
//    JoystickDriver.c:
//        readMsgFromPC
//        displayDiagnostics
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Timer Use:
//
// --- MAIN TASK TIMER USE - uses T1, T2 --------------------------------------------------------------------
//
//        DriveControl.h, DriveControlPID.h, DriveControlEncoder.h
//        T2 -  used for "poor man's" stall protection (driving straight/turning timeouts)
//              also used in "real" stall protection to determine time slice intervals for expected encoder ticks
//        CTstall -  used for "real" stall protection to time how long it has been since power was applied
//              to determine if drive system is ramped up allowing us to expect to see appropriate encoder ticks
//        CT1 - used for "real" stall protection to time holding the stall flag true once a stall is detected
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma systemFile            // this eliminates warning for "unreferenced" functions


#if TEAM_NUMBER == 3717
#elif TEAM_NUMBER == 4590
#elif TEAM_NUMBER == 5076
#elif TEAM_NUMBER == 5423
#else
xxx YOU MUST USE ONE OF THE TEAM #DEFINES IN THE .c FILE BONEHEAD! xxx
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Common\Hitech Drivers\common.h"
#include "..\Common\Control\SensorTask.h"

#include "..\Common\GlobalDefines.h"

#include "Includes\MotorControl.h"

#include "JoystickDriver.c"

#include "..\Common\ir.h"

#ifdef ENABLE_NXT2EXCEL_DEBUGGING
#include "..\Common\NXT2excel.h"
#endif
#include "..\Common\Control\EncoderTask.h"
#include "..\Common\Control\DriveControl.h"
#include "..\Common\Control\superproIface.h"
#include "..\Common\Control\AlertsMonitor.h" // Include if you want visual alerts

#include "..\Common\NxtButtons.h"
#include "..\Common\Control\PID.h"

//void recallJointAngles();

#if TEAM_NUMBER == 3717
//	#include "..\Common\FileSystem.h"

//	#include "..\Common\WeightedRings.h"
//	#include "..\Common\Control\SwerveSystemIface.h"
//	#include "..\Common\Control\ArmKinematics.h"
//	#include "..\Common\Control\ArmKinematics_Simple.h"
//	#include "..\Common\Control\ArmControl.h"
//	#include "..\Common\Control\ArmPlannerTask.h"
//	#include "Includes/Game Specific/2012 Ring It Up/JointRecovery.h"
//	#include "Includes\TeamArmControl.h"

#endif

#define Cmd(x) { x; while ( nNxtButtonPressed != kEnterButton ) {} }

////////// Function Definitions ///////////

void IdentifyTeamVersReqBtty(string sProgName);

void RequireGyroSanity()
{
	float angle = Gyro.fAngle;
	if ( angle != 0 )
	{
		nxtDisplayCenteredBigTextLine(1, "GYRO ERROR");
		nxtDisplayCenteredBigTextLine(2, "angle: %3.2f", angle);
		nxtDisplayCenteredBigTextLine(3, "MUST RESTART");
		nxtDisplayCenteredBigTextLine(5, "AND RECALIB");
		while ( true ) {
			wait1Msec(5);
		}
	}
}

///////// Tasks //////////

task UpdateJoystick()
{
	while (true) {									    // Polls Joystick for
		getJoystickSettings(joystick);	  // any new settings
		wait1Msec(25);					          // every 25 ms
	}									                  // Used for WaitForStart
}									                    //

//void displayIRColumn()
//{
//	int d = irFindColumn();
//	nxtDisplayTextLine(3, "%d", d);
//	wait1Msec(2000);
//}

void InitializeRobot(const string sProgName)
{
	wait1Msec(500);

	nVolume = 4; // brick volume to max

	//IdentifyTeamVersReqBtty(sProgName);

	SetAbortAutonomous(false);

	//InitDriveSystem();

	StartTask(EncoderTask);
	StartTask(SensorTask);

	//StartTask(ArmControlTask);
	//while ( !ArmControlTaskReady() )
	//wait1Msec(5);

	#if TEAM_NUMBER == 3717
		//StartTask(ArmPlannerTask);
		//StartTask(swerveSystemTask);
		//ArmJoint_MoveToPosAtRate(4, -999, 90);
		//initWeightedRingSystem();
	#endif

	ResetDriveEncoders();

	eraseDisplay();
}

//
// Identify:
//    * which team the program is built for
//    * which build version this program is
//
//
// Require that the external battery be on and have enough juice!
//    Don't allow to continue until the battery is turned on
//    Allow to continue if low battery after ENTER is hit on the brick
//
//
// Require that the SMUX be on and have enough juice!
//    Don't allow to continue until it is turned on
//
void IdentifyTeamVersReqBtty(string sProgName)
{
	bool bPriorDisplayDiags = bDisplayDiagnostics;
	bDisplayDiagnostics = false;

	// Build the message string
	int teamNum = TEAM_NUMBER;
	float versionNum = BUILD_VERS;

	eraseDisplay();
	nxtDisplayCenteredBigTextLine(0, sProgName);
	nxtDisplayCenteredBigTextLine(2, "TEAM");
	nxtDisplayCenteredBigTextLine(4, "%d", teamNum);
	nxtDisplayCenteredBigTextLine(6, "v%2.2f", versionNum);
	wait1Msec(3000);

	bool bLightToggle = false;

	// Require that the external battery be turned on
	eraseDisplay();
	while ( externalBatteryAvg == -1 ) {
		nxtDisplayCenteredBigTextLine(1, "TURN");
		nxtDisplayCenteredBigTextLine(3, "BATTERY");
		nxtDisplayCenteredBigTextLine(5, "ON!!!");
		if ( bLightToggle ) {
			//LightsOn();
			PlaySound(soundException);
			} else {
			//LightsOff();
		}
		bLightToggle = !bLightToggle;
		wait1Msec(250);
	}

	// Check if external battery has enough charge ... if low, allow override with the ENTER button
	eraseDisplay();
	bool bApproved = false;
	while ( externalBatteryAvg < MIN_REQ_EXT_BATTERY_VOLTAGE && !bApproved ) {
		float fBattery = (float)externalBatteryAvg / 1000;
		nxtDisplayCenteredBigTextLine(1, "BTRY LOW");
		nxtDisplayCenteredBigTextLine(3, "%2.1f VDC", fBattery);
		nxtDisplayCenteredBigTextLine(5, "<ENTER>");
		if ( bLightToggle ) {
			//LightsOn();
			PlaySound(soundException);
			} else {
			//LightsOff();
		}
		bLightToggle = !bLightToggle;
		bApproved = (nNxtButtonPressed == kEnterButton);
		wait1Msec(250);
	}

	bLightToggle = false;

#ifdef USING_SMUX_LO
	// Require that the lower SMUX be turned on
	eraseDisplay();
	while ( HTSMUXreadPowerStatus(HTSmuxLo) ) {
		nxtDisplayCenteredBigTextLine(1, "TURN");
		nxtDisplayCenteredBigTextLine(3, "LOWER SMUX");
		nxtDisplayCenteredBigTextLine(5, "ON!!!");
		if ( bLightToggle ) {
			LightsOn();
			PlaySound(soundException);
			} else {
			LightsOff();
		}
		bLightToggle = !bLightToggle;
		wait1Msec(250);
	}
#endif

#ifdef USING_SMUX_HI
	// Require that the upper SMUX be turned on
	eraseDisplay();
	while ( HTSMUXreadPowerStatus(HTSmuxHi) ) {
		nxtDisplayCenteredBigTextLine(1, "TURN");
		nxtDisplayCenteredBigTextLine(3, "UPPER SMUX");
		nxtDisplayCenteredBigTextLine(5, "ON!!!");
		if ( bLightToggle ) {
			LightsOn();
			PlaySound(soundException);
			} else {
			LightsOff();
		}
		bLightToggle = !bLightToggle;
		wait1Msec(250);
	}
#endif

	// All good to go, clear the display and restore display diagnostics to prior state
	eraseDisplay();
	bDisplayDiagnostics = bPriorDisplayDiags;
}
