#pragma systemFile

#include "matrixMath.h"

void  linearAlgCalcFSol(tJointAngles jointAngles, tToolTip *pToolTip);
//float linearAlgCalcInvKin(tToolTip goalToolTip, float J1_start, bool firstPass, tJointAngles *pJointAngles);

bool adjPointByError(tToolTip goalPose, tToolTip currPose, tToolTip *pOutputPose);
void calcDHChainAll(dhStateMatrix *pOutput, tJointAngles joint);

///////////////////////////////////////////////////////////////////////////////////////
//
// Forward Kinematics - given joint angles, calculate the resulting tool tip
//										  position/orientation
//
void calcFSol(tJointAngles jointAngles, tToolTip *pToolTip)
{
	linearAlgCalcFSol(jointAngles, pToolTip);
}

void linearAlgCalcFSol(tJointAngles jointAngles, tToolTip *pToolTip)
{
	dhStateMatrix dhStateMatrix;
	calcDHChainAll(dhStateMatrix, jointAngles);
	bldToolTipFmStateMatrix(dhStateMatrix, pToolTip);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Backward Kinematics - given a goal tool tip pos/orient, derive the actual arm
//											 pose (joint angles and calculated tool tip)
//
/*bool BackSoln(tToolTip goalTP, tToolTip *pSolnTP, tJointAngles *pSolnAngles, int *numTries)
{
	int i;
	float joint1 = 0;

	tToolTip tmpTP;    // tmp tool tip used for error calculation

	SafeMemcpy(pSolnTP, goalTP, sizeof(tToolTip)); // start iterating with the goal tool tip...

	// iterate until we have a solution "close enough"
	bool success = false;
	long startTime;
	for ( i = 0 ; i < 10 && !success ; i++ )
	{
		startTime = nPgmTime;
		joint1 = linearAlgCalcInvKin(*pSolnTP, joint1, (i == 0), pSolnAngles);	// make a pass at the back solution
		//writeDebugStreamLine("back soln try %d: %d ms", i+1, (nPgmTime - startTime));

		calcFSol(*pSolnAngles, &tmpTP);									// get tool tip for calc soln
		success = adjPointByError(goalTP, tmpTP, pSolnTP); // adjust the tool tip based on error and determine if done
		if ( !success )
			wait1Msec(4);
	}

	if ( !success )
		writeDebugStreamLine("***** BackSoln FAILED *****");

	*numTries = i;

	return success;
}*/

/////////////////////////////////////////////////////////////////////////////////////
//
// Adjust the calculated tooltip by how far off it is from the goal tooltip
// Also determine if we're done (close enough)
//
bool adjPointByError(tToolTip goalPose, tToolTip currPose, tToolTip *pOutputPose) {
	tToolTip error;
	error.x = (goalPose.x - currPose.x);
	error.y = (goalPose.y - currPose.y);
	error.z = (goalPose.z - currPose.z);
	error.rx = (goalPose.rx - currPose.rx);
	error.ry = (goalPose.ry - currPose.ry);
	error.rz = (goalPose.rz - currPose.rz);
	pOutputPose->x += error.x;
	pOutputPose->y += error.y;
	pOutputPose->z += error.z;
	//pOutputPose->rx += error.rx;
	pOutputPose->ry += error.ry;
	//pOutputPose->rz += error.rz;
	float linerr = sqrt(error.x * error.x + error.y * error.y + error.z * error.z);
  float roterr = 0;
 	if (linerr < KINEMATICS_BWD_ERR_TOL && roterr < KINEMATICS_BWD_ERR_TOL)
  	return true;
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Calculate the state matrix for the full DH chain
//
void calcDHChainAll(dhStateMatrix *pOutput, tJointAngles joint) {
	dhStateMatrix dhArray;
	dhStateMatrix dhArray2;
	dhStateMatrix dhMatrixMathOutput;
	//joint.joint5 *= -1.0;

	//								n,           theta,     d,     a,           alpha
	buildDH(dhArray,  1,               90,     0,     0,    joint.joint1);
	buildDH(dhArray2, 2,              -90,   j1z,   	0,    -90 + joint.joint2);

	matrixMatrixDot(dhArray, dhArray2, dhMatrixMathOutput);
	buildDH(dhArray, 3,               -90,   j2y,   j2z,              -90);
	matrixMatrixDot(dhMatrixMathOutput, dhArray, dhMatrixMathOutput);
	buildDH(dhArray, 4,     -90 + joint.joint3,     0,   j3y,             -90);
	matrixMatrixDot(dhMatrixMathOutput, dhArray, dhMatrixMathOutput);
	buildDH(dhArray, 5,              -90,   j3z,     0,               0);
	matrixMatrixDot(dhMatrixMathOutput, dhArray, dhMatrixMathOutput);
	buildDH(dhArray, 6,                0,     0,     j1x, joint.joint4-90);
	matrixMatrixDot(dhMatrixMathOutput, dhArray, dhMatrixMathOutput);
	buildDH(dhArray, 7,              -90,   j4y,   j4z,               0);
	matrixMatrixDot(dhMatrixMathOutput, dhArray, dhMatrixMathOutput);
	buildDH(dhArray, 8,  joint.joint5+90,   j5y,   j5x,              90);
	matrixMatrixDot(dhMatrixMathOutput, dhArray, dhMatrixMathOutput);
	buildDH(dhArray, 9,                0,   j5z,     0,               0);
	matrixMatrixDot(dhMatrixMathOutput, dhArray, dhMatrixMathOutput);
	SafeMemcpy(pOutput, dhMatrixMathOutput, sizeof(dhStateMatrix));
	//writeDebugStreamLine("First");
	//dumpStateMatrix(dhMatrixMathOutput);

}

/////////////////////////////////////////////////////////////////////////////////////
//
// Calculate the state matrix for just J4 out to the tool tip
//
void calcDHChainJ45(dhStateMatrix *pOutput, tJointAngles joint) {
	dhStateMatrix dhArray6;
	dhStateMatrix dhArray7;
	dhStateMatrix dhArray8;
	dhStateMatrix dhArray9;
	dhStateMatrix dhMatrixMathOutput;
	//joint.joint5 *= -1.0;

	//								n,           theta,     d,     a,           alpha
	buildDH(dhArray6, 6,               0,     0,     j1x, joint.joint4-90);
	buildDH(dhArray7, 7,             -90,   j4y,   j4z,               0);
	buildDH(dhArray8, 8, joint.joint5+90,   j5y,   j5x,              90);
	buildDH(dhArray9, 9,               0,   j5z,     0,               0);


	matrixMatrixDot(dhArray6, dhArray7, dhMatrixMathOutput);
	matrixMatrixDot(dhMatrixMathOutput, dhArray8, dhMatrixMathOutput);
	matrixMatrixDot(dhMatrixMathOutput, dhArray9, dhMatrixMathOutput);
	SafeMemcpy(pOutput, dhMatrixMathOutput, sizeof(dhStateMatrix));
}

///////////////////////////////////////////////////////////////////////////////////////
//
// Run one iteration of the inverse kinematics
//
//			Given the goal tool tip and starting J1 angle, return the resulting pose
//

// ... we hate life ... calcInvKin() uses SOOO many variables that it seems to cause stack corruption
//                  ... if it is swapped out lots.  So we've moved all of its locals to globals so
//								  ... stack impact during task swaps is minimized.
dhStateMatrix r_J1;
dhStateMatrix tp_J1;
tJointAngles tmpJointAngles;
tToolTip tmpPoint;
dhStateMatrix toolPointStateMatrix;
dhStateMatrix tmpMatrix;
tToolTip adjustedPoint;
float joint1;
dhStateMatrix j4VectorOutput;
dhStateMatrix dhMatrixMathOutput;
dhStateMatrix invertedDHMatrixMathOutput;
tToolTip tp_J1_point;
float length34;
float p24magnitude;
float M1;
float theta;
float alpha;
float beta1;
float M2;
float beta2;
tToolTip finalPoint;

/*float linearAlgCalcInvKin(tToolTip goalToolTip, float J1_start, bool firstPass, tJointAngles *pJointAngles) {

	tmpJointAngles.joint4 = 0;
	tmpJointAngles.joint5 = 0;
	tmpJointAngles.joint1 = 0;
	tmpJointAngles.joint2 = 0;
	tmpJointAngles.joint3 = 0;

	//static float pJ1Goal = 0;

	if(!firstPass)
	{
		createRyStateMatrix(r_J1, -J1_start);
		bldStateMatrixFmToolTip(toolPointStateMatrix, goalToolTip);
		matrixMatrixDot(r_J1, toolPointStateMatrix, tmpMatrix);
		bldToolTipFmStateMatrix(tmpMatrix, &tmpPoint);
		tmpJointAngles.joint4 = 0;//pJointAngles->j4Offset;
    tmpJointAngles.joint5 = 0;//tmpPoint.ry;
	}

	// ESTIMATE J1
	calcDHChainJ45(dhMatrixMathOutput, tmpJointAngles);

	invert4x4HomogeneousMatrix(dhMatrixMathOutput, invertedDHMatrixMathOutput);

	bldStateMatrixFmToolTip(toolPointStateMatrix, goalToolTip);
	matrixMatrixDot(toolPointStateMatrix, invertedDHMatrixMathOutput, j4VectorOutput);

	bldToolTipFmStateMatrix(j4VectorOutput, &adjustedPoint);
	joint1 = atan2(adjustedPoint.x,adjustedPoint.z) * (180/PI);

	createRyStateMatrix(r_J1, -joint1);
	matrixMatrixDot(r_J1, toolPointStateMatrix, tp_J1);

	bldToolTipFmStateMatrix(tp_J1, &adjustedPoint);
	tmpJointAngles.joint4 = 0;//pJointAngles->j4Offset;
	tmpJointAngles.joint5 = 0;//adjustedPoint.ry;
	calcDHChainJ45(dhMatrixMathOutput, tmpJointAngles);
	invert4x4HomogeneousMatrix(dhMatrixMathOutput, invertedDHMatrixMathOutput);
	matrixMatrixDot(toolPointStateMatrix, invertedDHMatrixMathOutput, j4VectorOutput);
	bldToolTipFmStateMatrix(j4VectorOutput, &adjustedPoint);
	joint1 = atan2(adjustedPoint.x,adjustedPoint.z) * (180/PI);
	writeDebugStreamLine("J1: %f, X: %f, Y: %f", joint1, adjustedPoint.x, adjustedPoint.z);

	if(pJointAngles->joint1 == pJointAngles->joint1) {
		pJointAngles->joint1 = joint1;
	}

	// ESTIMATE J3
	createRyStateMatrix(r_J1, -joint1);
	matrixMatrixDot(r_J1, toolPointStateMatrix, tp_J1);
	matrixMatrixDot(r_J1, j4VectorOutput, j4VectorOutput);
	bldToolTipFmStateMatrix(j4VectorOutput, &adjustedPoint);
	bldToolTipFmStateMatrix(tp_J1, &tp_J1_point);

	vectorArrayStruct p24vector;
	p24vector.fArray[0] = (adjustedPoint.x - j1x);
	p24vector.fArray[1] = (adjustedPoint.y - 0);
	p24vector.fArray[2] = (adjustedPoint.z - j1z);

	length34 = sqrt(j3z * j3z + j3y * j3y);

	p24magnitude = sqrt((p24vector.fArray[0]*p24vector.fArray[0]) + (p24vector.fArray[1]*p24vector.fArray[1]) +
														(p24vector.fArray[2]*p24vector.fArray[2]));

	M1 = (j2z * j2z + length34 * length34 - p24magnitude * p24magnitude) / (2 * length34 * j2z);

	theta = acos(M1) * (180/PI);

  alpha = atan2(j3y, j3z) * (180/PI);

  if(pJointAngles->joint3 == pJointAngles->joint3) {
  	pJointAngles->joint3 = 180 - theta + alpha;
	}

  // ESTIMATE J2
  beta1 = -atan2(p24vector.fArray[1], p24vector.fArray[2]) * (180/PI);
  M2 = (j2z * j2z + p24magnitude * p24magnitude - length34 * length34) / (2 * j2z * p24magnitude);
  beta2 = -acos(M2) * (180/PI);
  if(pJointAngles->joint2 == pJointAngles->joint2) {
  		pJointAngles->joint2 = beta1 + beta2;
	}
  // ESTIMATE J4 & J5
  tmpJointAngles.joint1 = 0.0;
  tmpJointAngles.joint2 = pJointAngles->joint2;
  tmpJointAngles.joint3 = pJointAngles->joint3;
  tmpJointAngles.joint4 = 0.0;
  tmpJointAngles.joint5 = 0.0;

  calcDHChainAll(dhMatrixMathOutput,tmpJointAngles);

  bldToolTipFmStateMatrix(dhMatrixMathOutput, &finalPoint);
	//if(pJointAngles->joint4 == pJointAngles->joint4) {
  	pJointAngles->joint4 = (-(pJointAngles->joint2 + pJointAngles->joint3) + 0);//pJointAngles->j4Offset);//tp_J1_point.rx - finalPoint.rx);
	//}
	//if(pJointAngles->joint5 == pJointAngles->joint5) {
  	pJointAngles->joint5 = 0;//= (tp_J1_point.ry - finalPoint.ry);
	//}

	return pJointAngles->joint1;
}*/

/////////////////////////////////////////////////////////////////////////////////////
void dumpKinematicsState(bool success, int i, tToolTip calcToolTip, tJointAngles calcJoints, tJointAngles goalJoints, int fwdTime, int bwdTime)
{
	string s = success ? "PASS" : "FAIL";
	writeDebugStreamLine("%s  # iterations: %d, err: %.3f, fwd: %dms, bwd: %dms", s, i, KINEMATICS_BWD_ERR_TOL, fwdTime, bwdTime);
	writeDebugStreamLine("");
	writeDebugStreamLine("tool tip: X: %3.3f\t\trX: %3.3f", calcToolTip.x, calcToolTip.rx);
	writeDebugStreamLine("          Y: %3.3f\t\trY: %3.3f", calcToolTip.y, calcToolTip.ry);
	writeDebugStreamLine("          Z: %3.3f\t\trZ: %3.3f", calcToolTip.z, calcToolTip.rz);
	writeDebugStreamLine("");
	writeDebugStreamLine("joints:   J1: %3.3f\t(%3d rounded)\tgoal: %4.3f", calcJoints.joint1, round(calcJoints.joint1), goalJoints.joint1);
	writeDebugStreamLine("          J2: %3.3f\t(%3d rounded)\tgoal: %4.3f", calcJoints.joint2, round(calcJoints.joint2), goalJoints.joint2);
	writeDebugStreamLine("          J3: %3.3f\t(%3d rounded)\tgoal: %4.3f", calcJoints.joint3, round(calcJoints.joint3), goalJoints.joint3);
	writeDebugStreamLine("          J4: %3.3f\t(%3d rounded)\tgoal: %4.3f", calcJoints.joint4, round(calcJoints.joint4), goalJoints.joint4);
	writeDebugStreamLine("          J5: %3.3f\t(%3d rounded)\tgoal: %4.3f", calcJoints.joint5, round(calcJoints.joint5), goalJoints.joint5);
}

void dumpToolTip(tToolTip tp, const string s)
{
		writeDebugStreamLine("%s: X: %f Y: %f Z: %f RX: %f RY: %f RZ: %f", s, tp.x, tp.y, tp.z, tp.rx, tp.ry, tp.rz);
}

void dumpJointAngles(tJointAngles a, const string s)
{
		writeDebugStreamLine("%s: %f J2: %f J3: %f J4: %f J5: %f ", s, a.joint1, a.joint2, a.joint3, a.joint4, a.joint5);
}

void dumpArmJointGoals(const string s)
{
		writeDebugStreamLine("%s: %f J2: %f J3: %f J4: %f J5: %f ", s, ReadArmJointGoal(1), ReadArmJointGoal(2), ReadArmJointGoal(3), ReadArmJointGoal(4), ReadArmJointGoal(5));
}
