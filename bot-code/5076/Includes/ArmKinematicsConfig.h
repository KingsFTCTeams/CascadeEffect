#ifndef TEAM_5076_ARM_KINEMATICS_H
#define TEAM_5076_ARM_KINEMATICS_H

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TEAM SPECIFIC SETTINGS FOR ARM KINEMATICS
//
#pragma systemFile            // this eliminates warning for "unreferenced" functions

#if TEAM_NUMBER == 5076

////////////////////////////////////////////////
//
// Public interface for the arm kinematics system
//
typedef struct {
	float joint1;
	float joint2;
	float joint3;
	float joint4;
	float joint5;
	float joint6;
	float j4Offset;
} tJointAngles;

// defines tool tip in terms of position and orientation
typedef struct {
	float x;
	float y;
	float z;
	float rx;
	float ry;
	float rz;
} tToolTip;

// Denavit Hartenberg (DH) state matrix
typedef struct {
	float dhArray[4][4];
} dhStateMatrix;

void calcFSol(tJointAngles jointAngles, tToolTip *pToolTip);
int BackSoln(tToolTip goalTP, tToolTip *pSolnTP, tJointAngles *pSolnAngles);

////////////////////////////////////////////////
//
// Constants for DH forward kinemantics
//
#define KINEMATICS_j1z 11.5
#define KINEMATICS_j1x -0.98
#define KINEMATICS_j2y 0.0
#define KINEMATICS_j2z 18.0
#define KINEMATICS_j3y 2.5
#define KINEMATICS_j3z 17.83
#define KINEMATICS_j4y 1.0
#define KINEMATICS_j4z 2.8
#define KINEMATICS_j5x 2.15
#define KINEMATICS_j5y 0
#define KINEMATICS_j5z 4.28

////////////////////////////////////////////////
//
// Error tolerance for backward kinematics
//
#define KINEMATICS_BWD_ERR_TOL 0.1


#else

  xxx YOU MUST USE ONE OF THE TEAM #DEFINES IN THE Autonomous Template.h FILE BONEHEAD! xxx

#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TEAM_5076_ARM_KINEMATICS_H
