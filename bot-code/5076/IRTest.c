#pragma config(Sensor, S3,     IRsensor,       sensorI2CCustom)
#pragma config(Sensor, S4,     GyroSensor,     sensorAnalogInactive)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma systemFile            // this eliminates warning for "unreferenced" functions

#define TEAM_NUMBER 5076

#include "JoystickDriver.c"
#include "Includes/TeamConfig.h"
#include "../Common/GlobalDefines.h"
#include "../Common/Control/SensorTask.h"

///////////////////////////////////////////////////
task main
{
  bDisplayDiagnostics = false;

  eraseDisplay();

  StartTask(SensorTask);

  while ( true )
  {
		float fElapsedTime = nPgmTime / 1000;
		string s = Gyro.bRotating ? "MOVE" : "HOLD";

		nxtDisplayTextLine(0, "%s - %4.2f secs", s, fElapsedTime);
		nxtDisplayTextLine(2, "IR band:   %d", IR1SensorVal());
		nxtDisplayTextLine(3, "Bot angle: %4.2f", Gyro.fAngle);

	  wait1Msec(50);
  }
}

int BeaconSeeker()
{
	int ir1 = IR1SensorVal(); // Each IR seeker returns 1-9 with 0 meaning lost signal
	int ir2 = IR2SensorVal(); // IR1 points right, IR2 left...IR2 directly over IR1 so
														// IR2 band 8 overlaps with IR2 band 2.

	int retVal = ir1 - 2; // use IR1 as the right side (IR values 2-9 return as 0 thru 7

	if ( ir1 < 2 )
		retVal = ir2 - 8;   // use IR2 as the left side (IR values 8-1 return as 0 thru -7

	return retVal;
}
