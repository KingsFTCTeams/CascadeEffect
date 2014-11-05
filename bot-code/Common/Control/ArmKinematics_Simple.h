#pragma systemFile

void simpleCalcFSol(tJointAngles jointAngles, tToolTip *pToolTip);
void simpleCalcInvKin(tToolTip goalToolTip, tJointAngles *pJointAngles);

bool simpleAdjPointByError(tToolTip goalPose, tToolTip currPose, tToolTip *pOutputPose);

///////////////////////////////////////////////////////////////////////////////////////
//
// Forward Kinematics - given joint angles, calculate the resulting tool tip
//										  position/orientation
//
float j2, j3, j4;
float cosj2j3, sinj2j3;
float cosj2j3j4, sinj2j3j4;

void simpleCalcFSol(tJointAngles jointAngles, tToolTip *pToolTip)
{
	j2 = jointAngles.joint2 * d2r;
	j3 = jointAngles.joint3 * d2r;
	j4 = -(jointAngles.joint2 + jointAngles.joint3) * d2r;

	cosj2j3 = cos(j2 + j3);
	sinj2j3 = sin(j2 + j3);
	sinj2j3j4 = sin(j2 + j3 + j4);
	cosj2j3j4 = cos(j2 + j3 + j4);

	// what about the rotational values????  I think all we care about is the positional info.
	pToolTip->x = 0.0;
	pToolTip->y = j3y * cosj2j3 - j3z * sinj2j3 - j2z * sin(j2) + j4y * cosj2j3j4 - j4z * sinj2j3j4;
	pToolTip->z = j3z * cosj2j3 + j3y * sinj2j3 + j2z * cos(j2) + j4z * cosj2j3j4 + j4y * sinj2j3j4;
	pToolTip->rx = pToolTip->ry = pToolTip->rz = 0;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Backward Kinematics - given a goal tool tip pos/orient, derive the actual arm
//											 pose (joint angles and calculated tool tip)
//
bool simpleBackSoln(tToolTip goalTP, tToolTip *pSolnTP, tJointAngles *pSolnAngles)
{
	tToolTip calculatedTP;    // tmp tool tip used for error calculation
	SafeMemcpy(pSolnTP, goalTP, sizeof(tToolTip)); // start iterating with the goal tool tip...

	long startTime = nPgmTime;
	simpleCalcInvKin(goalTP, pSolnAngles);	// make a pass at the back solution
	//writeDebugStreamLine("back soln (simple): %d ms", (nPgmTime - startTime));

	simpleCalcFSol(*pSolnAngles, &calculatedTP);						    // calc tool tip based on back solution
	bool success = simpleAdjPointByError(goalTP, calculatedTP, pSolnTP); // so we can check if succeeded

	if ( !success )
	{
		//nxtDisplayTextLine(6, "BACKSOLN FAIL");
		//writeDebugStreamLine("***************************** BackSoln FAILED *****************************");
		//writeDebugStreamLine("***************************** BackSoln FAILED *****************************");
		//writeDebugStreamLine("***************************** BackSoln FAILED *****************************");
		//assert();
	}
	if(success)
	{
		nxtDisplayTextLine(6, "BACKSOLN GOOD");
	}

	return success;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Adjust the calculated tooltip by how far off it is from the goal tooltip
// Also determine if we're done (close enough)
//
bool CheckForNAN(tToolTip tp)
{
	return ( tp.x != tp.x || tp.y != tp.y || tp.z != tp.z );
}

bool simpleAdjPointByError(tToolTip goalPose, tToolTip currPose, tToolTip *pOutputPose) {
	tToolTip error;

	if (CheckForNAN(goalPose) || CheckForNAN(currPose))
		return false;

	error.x = (goalPose.x - currPose.x);
	error.y = (goalPose.y - currPose.y);
	error.z = (goalPose.z - currPose.z);
	float linerr = sqrt(error.x * error.x + error.y * error.y + error.z * error.z);
 	if (linerr < 0.001)
  	return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////
//
// Run one iteration of the inverse kinematics
//
//			Given the goal tool tip and starting J1 angle, return the resulting pose
//

float r34, r34_squared;
float p4len, p4len_squared;
float beta, phi, gamma;
vectorArrayStruct p4;

void simpleCalcInvKin(tToolTip goalToolTip, tJointAngles *pJointAngles)
{
	// calc and save J4 offset
	float j4offset = ReadArmJointGoal(4) + (ReadArmJointGoal(2) + ReadArmJointGoal(3));
	if ( 0 )
	{
		writeDebugStreamLine("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
		writeDebugStreamLine("j4offset: %3.1f, J4: %3.1f, J2: %3.1f, J3: %3.1f", j4offset, ReadArmJointGoal(4), ReadArmJointGoal(2), ReadArmJointGoal(3));
		writeDebugStreamLine("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
	}

	// vector from J1 to J4
	p4.fArray[0] = 0;
	p4.fArray[1] = goalToolTip.y - j4y;
	p4.fArray[2] = goalToolTip.z - j4z;

	// calculate J3
	r34 = sqrt(j3y_squared + j3z_squared);
	r34_squared = r34 * r34;
	p4len = sqrt(p4.fArray[0]*p4.fArray[0] + p4.fArray[1]*p4.fArray[1] + p4.fArray[2]*p4.fArray[2]);
	p4len_squared = p4len * p4len;
	alpha = acos((-p4len_squared + r34_squared + j2z_squared) / (2 * r34 * j2z)) * r2d;
	beta = atan(j3y / j3z) * r2d; // atan2 here?
	pJointAngles->joint3 = 180 - alpha + beta;

	// calculate J2
	phi = acos((-r34_squared + j2z_squared + p4len_squared) / (2 * j2z * p4len)) * r2d;
	gamma = atan(p4.fArray[1] / p4.fArray[2]) * r2d; // atan2 here?
	pJointAngles->joint2 = -(phi + gamma);

	// calculate J4 ... keep it horizontal ... do we really want to do this since J4 is manually controlled?
	pJointAngles->joint4 = -(pJointAngles->joint2 + pJointAngles->joint3) + j4offset;
}
