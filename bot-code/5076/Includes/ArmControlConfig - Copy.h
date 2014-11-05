#ifndef TEAM_5076_ARM_CONTROL_H
#define TEAM_5076_ARM_CONTROL_H

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TEAM SPECIFIC SETTINGS FOR ARM CONTROL
//
#pragma systemFile            // this eliminates warning for "unreferenced" functions

#if TEAM_NUMBER == 5076

////////////////////////////////////////////////
//
// Public interface for the arm control system
//
task ArmControlTask();

void ArmJoint_MoveToPosAtRate(int joint, float posInDegrees, float rateInDegPerSec);
void ArmJoint_Hold(int joint);
float ReadArmJointAngle(int joint);
float ReadArmJointGoal(int joint);
bool ArmJointIsAtGoal(int joint);

////////////////////////////////////////////////
//
// Joint control info for each joint
//
//		To use a joint:
//			1) comment in its "USING_ARM_Jn" line
//			2) fill in each of its parameters
//			3) if the joint is a servo we still need
//				 all the parameters even if not used
//			4) gear ratio is motor:joint and if even number of gears make it negative
//

// joint 1
#define USING_ARM_J1
#define ARM_J1_MOTOR_TYPE 		tetrix12VDC
#define ARM_J1_MOTOR_ID   		ArmJ1
#define ARM_J1_GEAR_RATIO			(-1.0/3.0)
#define ARM_J1_ALLOWED_ERROR 	12.0

#define ARM_J1_JCS_OFFSET_DEG -45
#define ARM_J1_STARTING_POS   ARM_J1_JCS_OFFSET_DEG

#define ARM_J1_MAX_POS  90
#define ARM_J1_MIN_POS -45

#define ARM_J1_POS_PID_Kp 1.5
#define ARM_J1_POS_PID_Ki 0.0
#define ARM_J1_POS_PID_Kd 1.98

#define ARM_J1_SPD_PID_Kp 0.012 //0.012
#define ARM_J1_SPD_PID_Ki 0.0019
#define ARM_J1_SPD_PID_Kd 0.00002

// joint 2
#define USING_ARM_J2
#define ARM_J2_MOTOR_TYPE 		tetrix12VDC
#define ARM_J2_MOTOR_ID   		ArmJ2
#define ARM_J2_GEAR_RATIO			(1)
#define ARM_J2_ALLOWED_ERROR 	12.0

#define ARM_J2_JCS_OFFSET_DEG 0
#define ARM_J2_STARTING_POS   ARM_J2_JCS_OFFSET_DEG

#define ARM_J2_MAX_POS  90
#define ARM_J2_MIN_POS  -40

#define ARM_J2_POS_PID_Kp 1.5
#define ARM_J2_POS_PID_Ki 0.0
#define ARM_J2_POS_PID_Kd 1.98

#define ARM_J2_SPD_PID_Kp 1.0
#define ARM_J2_SPD_PID_Ki 0.0
#define ARM_J2_SPD_PID_Kd 1.0

// joint 3
//#define USING_ARM_J3
#define ARM_J3_MOTOR_TYPE 		tetrix12VDC
#define ARM_J3_MOTOR_ID   		motorJ3
#define ARM_J3_GEAR_RATIO			(-40.0/80.0)
#define ARM_J3_ALLOWED_ERROR 	12.0

#define ARM_J3_JCS_OFFSET_DEG -45.0
#define ARM_J3_STARTING_POS   ARM_J3_JCS_OFFSET_DEG

#define ARM_J3_MAX_POS  99999
#define ARM_J3_MIN_POS -99999

#define ARM_J3_POS_PID_Kp 1.5
#define ARM_J3_POS_PID_Ki 0.0
#define ARM_J3_POS_PID_Kd 1.98

#define ARM_J3_SPD_PID_Kp 0.012
#define ARM_J3_SPD_PID_Ki 0.0019
#define ARM_J3_SPD_PID_Kd 0.00002

// joint 4
//#define USING_ARM_J4
#define ARM_J4_MOTOR_TYPE 		tetrixServo
#define ARM_J4_MOTOR_ID   		motorJ4
#define ARM_J4_GEAR_RATIO			(1.0/1.0)
#define ARM_J4_ALLOWED_ERROR 	0.0

#define ARM_J4_JCS_OFFSET_DEG 15
#define ARM_J4_STARTING_POS   45

#define ARM_J4_MAX_POS (180.0 + ARM_J4_JCS_OFFSET_DEG)
#define ARM_J4_MIN_POS (0.0 + ARM_J4_JCS_OFFSET_DEG)

#define ARM_J4_POS_PID_Kp 0.0
#define ARM_J4_POS_PID_Ki 0.0
#define ARM_J4_POS_PID_Kd 0.0

#define ARM_J4_SPD_PID_Kp 0.0
#define ARM_J4_SPD_PID_Ki 0.0
#define ARM_J4_SPD_PID_Kd 0.0

// joint 5
//#define USING_ARM_J5
#define ARM_J5_MOTOR_TYPE 		tetrixServo
#define ARM_J5_MOTOR_ID   		motorJ5
#define ARM_J5_GEAR_RATIO			(1.0/1.0)
#define ARM_J5_ALLOWED_ERROR  0.0

#define ARM_J5_JCS_OFFSET_DEG -15
#define ARM_J5_STARTING_POS    45

#define ARM_J5_MAX_POS (180.0 + ARM_J5_JCS_OFFSET_DEG)
#define ARM_J5_MIN_POS (0.0 + ARM_J5_JCS_OFFSET_DEG)

#define ARM_J5_POS_PID_Kp 0.0
#define ARM_J5_POS_PID_Ki 0.0
#define ARM_J5_POS_PID_Kd 0.0

#define ARM_J5_SPD_PID_Kp 0.0
#define ARM_J5_SPD_PID_Ki 0.0
#define ARM_J5_SPD_PID_Kd 0.0

// joint 6
//#define USING_ARM_J6
#define ARM_J6_MOTOR_TYPE 		tetrixServo
#define ARM_J6_MOTOR_ID   		motorJ6
#define ARM_J6_GEAR_RATIO			(1.0/1.0)
#define ARM_J6_ALLOWED_ERROR  0.0

#define ARM_J6_JCS_OFFSET_DEG 0
#define ARM_J6_STARTING_POS   0

#define ARM_J6_MAX_POS 0
#define ARM_J6_MIN_POS 0

#define ARM_J6_POS_PID_Kp 0.0
#define ARM_J6_POS_PID_Ki 0.0
#define ARM_J6_POS_PID_Kd 0.0

#define ARM_J6_SPD_PID_Kp 0.0
#define ARM_J6_SPD_PID_Ki 0.0
#define ARM_J6_SPD_PID_Kd 0.0

// Make sure joint configuration is sane...if you use one you must use all the preceding ones
#if defined(USING_ARM_J6) && !(defined(USING_ARM_J5) && defined(USING_ARM_J4) && defined(USING_ARM_J3)  && defined(USING_ARM_J2) && defined(USING_ARM_J1))
*** ARM JOINT CONFIG ERROR ***
#endif
#if defined(USING_ARM_J5) && !(defined(USING_ARM_J4) && defined(USING_ARM_J3) && defined(USING_ARM_J2) && defined(USING_ARM_J1))
*** ARM JOINT CONFIG ERROR ***
#endif
#if defined(USING_ARM_J4) && !(defined(USING_ARM_J3) && defined(USING_ARM_J2) && defined(USING_ARM_J1))
*** ARM JOINT CONFIG ERROR ***
#endif
#if defined(USING_ARM_J3) && !(defined(USING_ARM_J2) && defined(USING_ARM_J1))
*** ARM JOINT CONFIG ERROR ***
#endif
#if defined(USING_ARM_J2) && !defined(USING_ARM_J1)
*** ARM JOINT CONFIG ERROR ***
#endif

// Arm control task cycle time in mSecs
#define ARM_CONTROL_TASK_CYCLE_TIME 50

#else

  xxx YOU MUST USE ONE OF THE TEAM #DEFINES IN THE Autonomous Template.h FILE BONEHEAD! xxx

#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TEAM_5076_ARM_CONTROL_H
