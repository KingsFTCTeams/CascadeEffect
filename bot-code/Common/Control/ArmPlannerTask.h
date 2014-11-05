#pragma systemFile            // this eliminates warning for "unreferenced" functions

#define ARM_PLANNER_TASK_MS 50

void SimpleJogArmSetDist(float dx, float dy, float dz, float timeMS);
void calcBackSolnMoveArm(tToolTip goalToolTip, float timeMS);
void CalcAndMoveToNewArmPosWithInt(const string debugMsg, tToolTip goalToolTip, float timeMS);
void CalcAndMoveToNewArmPos(const string debugMsg, tToolTip goalToolTip, float timeMS);
void _CalcAndJogArmSetDist(float dx, float dy, float dz, float timeMS);

/////////////////////////////////////////////////////////////////////////////////////
//
// These are not to be directly used outside this module
//
float _JumpX = 0;
float _JumpY = 0;
float _JumpZ = 0;
float _JumpMS = ARM_PLANNER_TASK_MS;

/////////////////////////////////////////////////////////////////////////////////////
//
// Cause a jump on the indicated axis
//
void SimpleJogArmSetDist(float dx, float dy, float dz, float timeMS)
{
	_JumpX = dx;
	_JumpY = dy;
	_JumpZ = dz;
	_JumpMS = timeMS;
}
void SimpleJogArmSetDist(float dx, float dy, float dz)
{
	SimpleJogArmSetDist(dx, dy, dz, ARM_PLANNER_TASK_MS);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// J4 Management - we always want J4 to be horizontal (with some driver controlled
//                 offset) so we'll position it relative to J2 & J3
//								 j4 = -(j2 + j3) + offset
//
bool g_keepJ4horiz = false;
float g_j4offset = 0;

// call this to have the planner start keeping J4 orientated
void ArmPlanner_KeepJ4Horiz(bool state)
{
	g_keepJ4horiz = state;
}

// call this to adjust the J4 offset
void ArmPlanner_AddToJ4offset(float delta)
{
	g_j4offset += delta;
}

// call this to set the J4 offset to a discrete value
void ArmPlanner_SetJ4Offset(float offset)
{
	g_j4offset = offset;
}

// ...this is called by the arm planner task to manage J4
void KeepJ4Orientated()
{
	if ( g_keepJ4horiz )
	{
		float newJ4 = -(ReadArmJointAngle(2) + ReadArmJointAngle(3)) + g_j4offset + ARM_J4_JCS_OFFSET_DEG;
		ArmJoint_MoveToPosAtRate(4, newJ4, 90);
	}
}

bool g_bNewToolTip = false;
tToolTip g_globalToolTip;
long g_timeToReachToolTip;

void GotoToolTip(tToolTip newToolTip, long timeMS) {
	memcpy(g_globalToolTip, newToolTip, sizeof(tToolTip));
	g_timeToReachToolTip = timeMS;
	g_bNewToolTip = true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Arm Planner Task -- manages moving the tool tip incrementally
//
task ArmPlannerTask()
{
	long lastLoopTime = nPgmTime;

	_JumpX = _JumpY = _JumpZ = 0.0;
	_JumpMS = ARM_PLANNER_TASK_MS;

	g_keepJ4horiz = false; // will be set true when we unstow
	g_j4offset = 0;

	while ( true )
	{
		//if there is a new tool tip then goto it
		if(g_bNewToolTip)
		{
			g_bNewToolTip = false;
			CalcAndMoveToNewArmPosWithInt("", g_globalToolTip, g_timeToReachToolTip);
		}
		//otherwise check if there are new deltas and move the arm set deltas
		else if(_JumpX || _JumpZ || _JumpY)
		{
			_CalcAndJogArmSetDist(_JumpX, _JumpY, _JumpZ, _JumpMS);
		}

		if ( !ArmJoint_KinematicsEnabled(4) )
		{
			KeepJ4Orientated();
		}

		while ( nPgmTime - lastLoopTime < ARM_PLANNER_TASK_MS )
			wait1Msec(5);

		lastLoopTime = nPgmTime;
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Move the tool tip to preset positions:
//
//  1: low
//	2: mid
//	3: high
//
#define RADIAL_MAX 43.0
#define SAFE_RADIAL 40.0
void MoveArmToRackRow(int row, float offset = 2.5)
{
	g_CancleMove = true;
	tToolTip currToolTip, xlatedToolTip;

  // make sure the row number is sane
  row = clipf(row, 1, 3);

  // determine our current tool tip
  simpleCalcFSol(g_CurrJointAngles, &currToolTip);

  // translate in the y-axis to the indicated row
  memcpy(xlatedToolTip, currToolTip, sizeof(tToolTip));
  float yOffset = 0;
  if(row <= 2)
  {
          yOffset = (j4z * sin((g_j4offset * (PI/180))));
  }
  xlatedToolTip.y = (g_BaseToolTip.y + 13.5 * (row - 1) + offset) + yOffset;

  //If the radial location is out of bounds calculate a new z value
  if(sqrt((xlatedToolTip.y*xlatedToolTip.y) + (xlatedToolTip.z*xlatedToolTip.z)) > RADIAL_MAX)
	{
		float newZVal = sqrt((SAFE_RADIAL*SAFE_RADIAL) - (xlatedToolTip.y*xlatedToolTip.y));
		xlatedToolTip.z = newZVal;
	}

	// rate will be one row per second (or 13.5 in/sec)
	float distY = xlatedToolTip.y - currToolTip.y;
	float distZ = xlatedToolTip.z - currToolTip.z;
  float timeMS = abs(distY) / 13.5 * 1000;

  // move there!
  //string debugMsg;
  //sprintf(debugMsg, "JUMP %2.1f inches", distY);
  //CalcAndMoveToNewArmPos(debugMsg, xlatedToolTip, timeMS);
  //writeDebugStreamLine("Y: %f Z: %f", distY, distZ);
	//SimpleJogArmSetDist(0, distY, distZ, timeMS);
  GotoToolTip(xlatedToolTip, timeMS);
}

void MoveArmToRackRowBack(int row)
{
	g_CancleMove = true;
	tToolTip currToolTip, xlatedToolTip;

  // make sure the row number is sane
  row = clipf(row, 1, 3);

  // determine our current tool tip
  simpleCalcFSol(g_CurrJointAngles, &currToolTip);



	// translate in the y-axis to the indicated row
  memcpy(xlatedToolTip, currToolTip, sizeof(tToolTip));
  float yOffset = 0;
  if(row <= 2)
  {
          yOffset = (j4z * sin((g_j4offset * (PI/180))));
  }
  xlatedToolTip.y = (g_BaseToolTip.y + 13.5 * (row - 1) + 1.5) + yOffset;

  ArmPlanner_SetJ4Offset(-35);

  //If the radial location is out of bounds calculate a new z value
  if(sqrt((xlatedToolTip.y*xlatedToolTip.y) + (xlatedToolTip.z*xlatedToolTip.z)) > RADIAL_MAX)
	{
		float newZVal = sqrt((SAFE_RADIAL*SAFE_RADIAL) - (xlatedToolTip.y*xlatedToolTip.y));
		xlatedToolTip.z = newZVal;
	}

	// rate will be one row per second (or 13.5 in/sec)
	float distY = xlatedToolTip.y - currToolTip.y;
  float timeMS = abs(distY) / 13.5 * 1000;

  GotoToolTip(xlatedToolTip, timeMS);
}

void JumpArmSetDistFromHome(float y, float zVal)
{
	tToolTip currToolTip, xlatedToolTip;
	// determine our current tool tip
	simpleCalcFSol(g_CurrJointAngles, &currToolTip);
	// translate in the y-axis to the indicated row
	memcpy(xlatedToolTip, currToolTip, sizeof(tToolTip));
	xlatedToolTip.y = g_BaseToolTip.y + y;
	xlatedToolTip.z = g_BaseToolTip.z + zVal;
	// rate will be one row per second (or 13.5 in/sec)
	float distY = xlatedToolTip.y - currToolTip.y;
	float distZ = xlatedToolTip.z - currToolTip.z;
	float timeMS = abs(distY) / 13.5 * 1000;

	// move there!
	//SimpleJogArmSetDist(0, distY, distZ, timeMS);
	GotoToolTip(xlatedToolTip, timeMS);
}

void CalcAndJogArmSetDist(float dx, float dy, float dz, float speed) {
	g_CancleMove = true;
	tToolTip currToolTip, xlatedToolTip;
	simpleCalcFSol(g_CurrJointAngles, &currToolTip);
	memcpy(xlatedToolTip, currToolTip, sizeof(tToolTip));
	xlatedToolTip.z += dz;
	xlatedToolTip.y += dy;
	float distY = xlatedToolTip.y - currToolTip.y;
	float timeMS = abs(distY) / speed * 1000;
	GotoToolTip(xlatedToolTip, timeMS);
	dumpToolTip(xlatedToolTip, "After");
}

void CalcAndJogArmSetDist(float dx, float dy, float dz) {
	CalcAndJogArmSetDist(dx, dy, dz, 13.5);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Move the tool tip incrementally
//
// This function handles interpolation, tool tip limit checking, and all
// backwards and forward solutions necessary to actually move joints
// it is only called by the arm planner task
//
void _CalcAndJogArmSetDist(float dx, float dy, float dz, float timeMS)
{
	tToolTip xlatedToolTip;

	// determine our current tool tip
	simpleCalcFSol(g_CurrJointAngles, &xlatedToolTip);
	if(xlatedToolTip.y != xlatedToolTip.y || xlatedToolTip.z != xlatedToolTip.z)
	{
		return;
	}
	// 2D translation in YZ plane
	xlatedToolTip.y += dy;
	xlatedToolTip.z += dz;

	CalcAndMoveToNewArmPosWithInt("", xlatedToolTip, timeMS);
}

#define TOOL_TIP_Y_MAX 38
#define TOOL_TIP_Y_MIN 0
#define TOOL_TIP_Z_MAX 9999
#define TOOL_TIP_Z_MIN 0.0//7.0

bool ToolTipWithinLimits(tToolTip tp)
{
	bool inLimits = (TOOL_TIP_Y_MIN < tp.y && tp.y < TOOL_TIP_Y_MAX && tp.z > TOOL_TIP_Z_MIN && tp.z < TOOL_TIP_Z_MAX && sqrt((tp.y*tp.y) + (tp.z*tp.z)) < RADIAL_MAX);
	if ( !inLimits )
	{
		writeDebugStreamLine("************** TOOL TIP OUT OF LIMITS! **************");
		dumpToolTip(tp, "BAD TOOL TIP");
		writeDebugStreamLine("*****************************************************");
	}
	return inLimits;
}

#define NUMBER_OF_JUMP_STAGES 3

/////////////////////////////////////////////////////////////////////////////////////
//
// Move the tool tip to the indicated position (by tool tip)
// within the indicated time keeping all joints moving in synch
// and in plane via interpolation
//
void CalcAndMoveToNewArmPosWithInt(const string debugMsg, tToolTip goalToolTip, float timeMS)
{
	g_CancleMove = false;
	long startTime = nPgmTime;

	// determine new tool tip and joint angles
	tToolTip currToolTip;
	simpleCalcFSol(g_CurrJointAngles, &currToolTip);

	if(abs(goalToolTip.y - currToolTip.y) > 4)
	{
		//jumping in stages
		float jumpStages = (goalToolTip.y - currToolTip.y) / NUMBER_OF_JUMP_STAGES;
		float jumpTime = timeMS / NUMBER_OF_JUMP_STAGES;
		tToolTip tmpToolTip;
		memcpy(tmpToolTip, currToolTip, sizeof(tToolTip));
		tmpToolTip.z = goalToolTip.z;
		for(int i = 0; i < NUMBER_OF_JUMP_STAGES; i++)
		{
			writeDebugStreamLine("Jumping Stage %d", i);
			tmpToolTip.y += jumpStages;
			calcBackSolnMoveArm(tmpToolTip, jumpTime);
			KeepJ4Orientated();
			long _startTime = nPgmTime;
			//wait1Msec(jumpTime);
			while(nPgmTime - _startTime < jumpTime) {
				KeepJ4Orientated();
				wait1Msec(2);
			}
			if(g_CancleMove) {
					break;
			}
		}
	}
	else
	{
		//just jump
		calcBackSolnMoveArm(goalToolTip, timeMS);
	}
}

void calcBackSolnMoveArm(tToolTip goalToolTip, float timeMS)
{
	tToolTip newToolTip;
	tJointAngles newJointAngles;
	memcpy(newJointAngles, g_CurrJointAngles, sizeof(tJointAngles));
	if ( simpleBackSoln(goalToolTip, &newToolTip, &newJointAngles) && ToolTipWithinLimits(newToolTip) )
	{
		// normalize the rates
		float j2delta = newJointAngles.joint2 - g_CurrJointAngles.joint2;
		float j3delta = newJointAngles.joint3 - g_CurrJointAngles.joint3;
		float j4delta = newJointAngles.joint4 - g_CurrJointAngles.joint4;
		float rateJ2 = abs(j2delta) * 1000 / timeMS;
		float rateJ3 = abs(j3delta) * 1000 / timeMS;
		float rateJ4 = abs(j4delta) * 1000 / timeMS;

		// move the arm
		ArmJoint_MoveToPosAtRate(2, newJointAngles.joint2, rateJ2);
		ArmJoint_MoveToPosAtRate(3, newJointAngles.joint3, rateJ3);
		if ( ArmJoint_KinematicsEnabled(4) )
		{
			ArmJoint_MoveToPosAtRate(4, newJointAngles.joint4, rateJ4);
		}
		ArmPlanner_KeepJ4Horiz(true);
	}
	else
	{
		writeDebugStreamLine("Backsoln Fail");
	}
}
