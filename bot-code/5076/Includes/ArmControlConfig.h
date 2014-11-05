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

#define Servo_W_1 servo4
#define Servo_W_2 servo1
#define Servo_W_3 servo2
#define Servo_W_1_offset 75
#define Servo_W_2_offset 105
#define Servo_W_3_offset 90

void ArmJoint_MoveToPosAtRate(int joint, float posInDegrees, float rateInDegPerSec);
void ArmJoint_Hold(int joint);
float ReadArmJointAngle(int joint);
float ReadArmJointGoal(int joint);
bool ArmJointIsAtGoal(int joint);
void SaveDispenserPosition();
void SaveRackPosition();
void PackArm();
void UnPackArm();

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

#define ARM_MANUAL_MAX_MOVE_RATE_IN_SEC 2.0

#define DISP_LEFT_RIGHT_DIST 13.5
#define RACK_LEFT_RIGHT_DIST 20.5
#define RACK_UP_DOWN_DIST    13.5

//jumping distences rack
#define ARM_JUMP_RACK_PEG_LEFT -RACK_LEFT_RIGHT_DIST
#define ARM_JUMP_RACK_PEG_RIGHT RACK_LEFT_RIGHT_DIST
#define ARM_JUMP_RACK_PEG_UP 	  RACK_UP_DOWN_DIST
#define ARM_JUMP_RACK_PEG_DOWN -RACK_UP_DOWN_DIST

//jumping distences dispencer
#define ARM_JUMP_DISP_PEG_LEFT -DISP_LEFT_RIGHT_DIST
#define ARM_JUMP_DISP_PEG_RIGHT DISP_LEFT_RIGHT_DIST


#define JOINT_PACK_MOVE_RATE 90
#define JOINT_TO_SAVE_MOVE_RATE 90
#define ARM_JOINT_JUMP_RATE 90.0

// pack angles
//joint 3 to 150
//joint 4 to 0
//joint 5 to 0
//joint 2 to -80
//joint 4 to -70
#define JOINT_ONE_PACK_ANGLE   0
#define JOINT_TWO_PACK_ANGLE   0
#define JOINT_THREE_PACK_ANGLE 90
#define JOINT_FOUR_PACK_ANGLE  0
#define JOINT_FIVE_PACK_ANGLE  0

// un-pack angles
#define JOINT_ONE_UNPACK_ANGLE   0
#define JOINT_TWO_UNPACK_ANGLE   0
#define JOINT_THREE_UNPACK_ANGLE 0
#define JOINT_FOUR_UNPACK_ANGLE  0
#define JOINT_FIVE_UNPACK_ANGLE  0

// joint 1 ----- Shoulder
#define USING_ARM_J1
#define ARM_J1_MOTOR_TYPE 		tetrix12VDC
#define ARM_J1_MOTOR_ID   		ArmJ1
#define ARM_J1_GEAR_RATIO			(1.0/9.0)
#define ARM_J1_ALLOWED_ERROR 	12.0

#define ARM_J1_JCS_OFFSET_DEG -22.5
#define ARM_J1_STARTING_POS   ARM_J1_JCS_OFFSET_DEG

#define ARM_J1_MAX_POS  160
#define ARM_J1_MIN_POS ARM_J1_JCS_OFFSET_DEG

#define ARM_J1_POS_PID_Kp 1.5
#define ARM_J1_POS_PID_Ki 0.0
#define ARM_J1_POS_PID_Kd 2.3

#define ARM_J1_SPD_PID_Kp 0.012 //0.01
#define ARM_J1_SPD_PID_Ki 0.0024 //0.002
#define ARM_J1_SPD_PID_Kd 0.00008 //0.001

#define ARM_J1_VARIABLE_ANGLE 0
#define ARM_J1_INVERT_MOTOR 0
#define ARM_J1_PID_ENABLED 1
#define ARM_J1_KINEMATICS_ENABLED 0

// joint 2 ----- Extention
#define USING_ARM_J2
#define ARM_J2_MOTOR_TYPE 		tetrix12VDC
#define ARM_J2_MOTOR_ID   		ArmExtention
#define ARM_J2_GEAR_RATIO			(-1.0/1.0)
#define ARM_J2_ALLOWED_ERROR 	12.0

#define ARM_J2_JCS_OFFSET_DEG 0
#define ARM_J2_STARTING_POS   ARM_J2_JCS_OFFSET_DEG
#define ARM_J2_MAX_POS  9999
#define ARM_J2_MIN_POS  0

#define ARM_J2_POS_PID_Kp 1.5
#define ARM_J2_POS_PID_Ki 0.0
#define ARM_J2_POS_PID_Kd 1.98

#define ARM_J2_SPD_PID_Kp 0.01
#define ARM_J2_SPD_PID_Ki 0.001
#define ARM_J2_SPD_PID_Kd 0.00005

#define ARM_J2_VARIABLE_ANGLE 0
#define ARM_J2_INVERT_MOTOR 0
#define ARM_J2_PID_ENABLED 1
#define ARM_J2_KINEMATICS_ENABLED 0

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

#define ARM_J3_VARIABLE_ANGLE 1

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

#define ARM_J4_VARIABLE_ANGLE 0

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

#define ARM_J5_VARIABLE_ANGLE 0

// joint 6
//#define USING_ARM_J6
//#define ARM_J6_MOTOR_TYPE 		tetrixServo
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

#define ARM_J6_VARIABLE_ANGLE 0

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
