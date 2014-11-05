#ifndef ARM_CONTROL_IFACE_H
#define ARM_CONTROL_IFACE_H

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private/internal interface for the arm control system ... not to be used elsewhere
//
#pragma systemFile            // this eliminates warning for "unreferenced" functions

// maximum number of joint the system supports
#define MAX_NUM_JOINTS 6

// types of motors used on the arm
typedef enum
{
	tetrix12VDC,
	tetrixServo

} tMotorType;

// state information for PID control ... gain constants and state variables
typedef struct
{
	float goalInDegrees;

	bool atGoal;

	float Kp;
	float Ki;
	float Kd;

	float integral;
	float derivative;
	float lastError;

	long lastTime;
	long pValue;
	long currValue;

} tPIDinfo;

// state information necessary to control a joint
typedef struct
{
	int jointNumber;  // joint number 1 - n

	tMotor motorID; 		 // from pragmas
	TServoIndex servoID; // from pragmas

	tMotorType motorType;	 // either tetrix12VDC or tetrixServo
	float JCSoffsetDeg;    // Offset of joint coord system to world coord system
	float startingPos;     // Starting pos of joint in world coordinate system
	float allowedError;    // deadzone around goal
	float gearRatio;			 // motor:joint

	float maxPosInDegrees;
	float minPosInDegrees;

	tPIDinfo posPIDinfo; // PID gain constants and static control variables
	tPIDinfo spdPIDinfo;
	bool variableJoint;
	bool bInvertMotor;
	bool bPIDEnabled;
	bool bKinematicsEnabled;

	int motorPower;

	float servoCurrPos;

} tJointInfo;

// Initialize the arm
void InitArm();

// init 12VDC motor joint
void InitJoint(int joint, tMotor motorID, tMotorType motorType, float gearRatio,
							 float JCSoffset, float startPos, float maxPos, float minPos,
							 float posKp, float posKi, float posKd, float spdKp, float spdKi, float spdKd, bool variableJoint);

// init servo joint
void InitJoint(int joint, TServoIndex servoID, tMotorType motorType, float gearRatio,
						   float JCSoffset, float startPos, float maxPos, float minPos,
							 float posKp, float posKi, float posKd, float spdKp, float spdKi, float spdKd, bool variableJoint);

// service routine used by above...inits all but motor ID
void InitJoint(int joint, tMotorType motorType, float gearRatio,
							 float JCSoffset, float startPos, float maxPos, float minPos,
							 float posKp, float posKi, float posKd, float spdKp, float spdKi, float spdKd, bool variableJoint);

// other arm control private functions
void PIDjoint(int joint);
void PID_12VDC_Joint(int joint);
void PID_Servo_Joint(int joint);
void PID_Servo_Joint_RateControlled(int joint);
void setMotorSpeedPID(int joint, int goalMotorDegreesPerSecond);
int calcPosPID(int joint, int goalMotorPosDegrees, int goalMotorDegreesPerSecond);
void ResetJointEncoder(int joint);
long ReadJointEncoder(int joint);
void SetJointPower(int joint, long power);
float MotorDegToJointDeg(int joint, float deg);
float JointDegToMotorDeg(int joint, float deg);
void InitJoint2(int joint, bool bInvertMotor, bool PIDEnabled, bool KinematicsEnabled);
void JumpArmSetDist(float x, float y, float z, tJointAngles currentJointAngles);
void JumpArmSetDist(float x, float y, float z, tJointAngles currentJointAngles, float speed);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // ARM_CONTROL_IFACE_H
