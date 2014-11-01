#include "../Common/Autonomous Template.h"

#include "Includes/Game Specific/2012 Ring It Up/AutonomousRoutines/RedCrates.h"
#include "Includes/Game Specific/2014 Cascade Effect/AutonomousRoutines.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// TASK MAIN //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
task main() {
	int Mission = 1;              // list mission names and numbers here
	const int nMissionCount = 2;  // set to number of missions...used for menu routine
	int waitValue = 0;

	InitializeRobot("Autonomous");  // Perform all required initialization sub-routine
	//StartTask(ArmControlTask);
	StartTask(SensorTask);
	// Turn off normal screen diagnostics so can see and use the menu to select autonomous mission
	bDisplayDiagnostics = false;
	eraseDisplay();

	nxtDisplayCenteredBigTextLine(2, "Select");
	nxtDisplayCenteredBigTextLine(4, "Mission");

	wait1Msec(250);

	// present menu and get the mission to run
	while ( nNxtButtonPressed != 3 ) {
		if ( nNxtButtonPressed == 1 && time1[T1] > 500 && Mission < nMissionCount) {
			Mission++;
			time1[T1] = 0;
			} else if ( nNxtButtonPressed == 2 && time1[T1] > 500 && Mission > 1 ) {
			Mission--;
			time1[T1] = 0;
		}

		if ( Mission == 1 ) {
			nxtDisplayCenteredBigTextLine(2, "Mission");
			nxtDisplayCenteredBigTextLine(4, "test");
			} else if ( Mission == 2 ) {
			nxtDisplayCenteredBigTextLine(2, "Mission");
			nxtDisplayCenteredBigTextLine(4, "OffRamp");
			} /*else if ( Mission == 3 ) {
			nxtDisplayCenteredBigTextLine(1, "Mission");
			nxtDisplayCenteredBigTextLine(3, "Defend R");
			nxtDisplayCenteredBigTextLine(5, "Awesomly");
			} else if ( Mission == 4 ) {
			nxtDisplayCenteredBigTextLine(1, "Mission");
			nxtDisplayCenteredBigTextLine(3, "Defend L");
			nxtDisplayCenteredBigTextLine(5, "Awesomly");
			}	else if ( Mission == 5 ) {
			nxtDisplayCenteredBigTextLine(2, "Mission");
			nxtDisplayCenteredBigTextLine(4, "M-L-O");
			}	else if ( Mission == 6 ) {
			nxtDisplayCenteredBigTextLine(2, "Mission");
			nxtDisplayCenteredBigTextLine(4, "M-R-O");}*/

		wait1Msec(25);
		eraseDisplay();
	}

	nxtDisplayCenteredBigTextLine(3, "CONFIRMED");
	wait1Msec(750);
	eraseDisplay();

	// Run the selected mission.
	// The missions are found in "Includes\Game Specific\<game directory>\<mission specific command file>.h"

	////////////////////////////////////////////////////////
	time1[T1] = 0;

	while (nNxtButtonPressed != kEnterButton) {
		nxtDisplayCenteredBigTextLine(2, "Time?");
		nxtDisplayCenteredBigTextLine(4, "%d", waitValue);

		if(time1[T1] > 350) {
			if(nNxtButtonPressed == kRightButton) {
				waitValue += 250;
				time1[T1] = 0;
				} if(nNxtButtonPressed == kLeftButton) {
				waitValue -= 250;
				time1[T1] = 0;
			}
		}
		if (waitValue < 0) {
			waitValue = 0;
		}
	}

	eraseDisplay();
	////////////////////////////////////////////////////////

	bDisplayDiagnostics = false;   // turn back on normal runtime diagnostics

	waitForStart();               // wait for the FCS to say the match has started

	wait1Msec(500);
	//wait1Msec(waitValue);

	if ( Mission == 1 )
	{
		Basic_Testing_Auto();
	}
	else if(Mission == 2)
	{
		DriveOffRamp();
	}
	//else if(Mission == 3)
	//{
	//	AwesomeDefensnessright_Auto();
	//}
	//else if(Mission == 4)
	//{
	//	AwesomeDefensnessleft_Auto();
	//}
	//else if(Mission == 5)
	//{
	//	//Middle_Auto_Drop_Left_Opposite();
	//}
	//else if(Mission == 6)
	//{
	//	//Basic_Testing_Auto();
	//}


}
