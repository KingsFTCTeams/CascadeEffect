#pragma systemFile            // this eliminates warning for "unreferenced" functions

#include "ArmControlIface.h"

void resetValues();

dhStateMatrix jumpStateMatrix;

tJointAngles g_CurrJointAngles;
tToolTip     g_BaseToolTip;

#define JOINT_TO_FOLLOW 999
//#define DEBUG_ARM_JOTIN_ANGLES

//////////////////////////////////////////////////////////////////////////////////
//
// State information for each joint
//
tJointInfo joints[MAX_NUM_JOINTS+1];

//////////////////////////////////////////////////////////////////////////////////
//
// Containers for joint save points
//
tJointAngles rackPosition;
tJointAngles dispensePosition;

void updateGJointPoses();

bool ArmJoint_KinematicsEnabled(int joint)
{
	return joints[joint].bKinematicsEnabled;
}

//////////////////////////////////////////////////////////////////////////////////
//
// Command the arm to move the indicated joint to the designated position at a rate (positive)
//
void ArmJoint_MoveToPosAtRate(int joint, float posInDegrees, float rateInDegPerSec)
{
	//hogCPU();
	// save the goals expressed in motor units not joint...so apply offset and gear ratio
	float clippedPos = clipf(posInDegrees, joints[joint].minPosInDegrees, joints[joint].maxPosInDegrees);
	float goalInDegrees = (clippedPos - joints[joint].JCSoffsetDeg) / joints[joint].gearRatio;
	if ( goalInDegrees != joints[joint].posPIDinfo.goalInDegrees ) {
		joints[joint].posPIDinfo.atGoal = false;
		joints[joint].servoCurrPos = joints[joint].posPIDinfo.goalInDegrees;
		joints[joint].posPIDinfo.goalInDegrees = goalInDegrees;
		float rateInDegPerSecCon = clipf(abs(rateInDegPerSec) / abs(joints[joint].gearRatio), -900, 900);
		joints[joint].spdPIDinfo.goalInDegrees = rateInDegPerSecCon;
		updateGJointPoses();
		if ( joint == JOINT_TO_FOLLOW )
		{
			writeDebugStreamLine("Clipped Pose: %f Clipped Deg/s: %f ", clippedPos, rateInDegPerSecCon);
			//writeDebugStreamLine("clippedPos: %3.1f, min %3.1f, max %3.1f", clippedPos, joints[joint].minPosInDegrees, joints[joint].maxPosInDegrees);
			//writeDebugStreamLine("%d Goal: %3.1f/%3.1f (m/j), %3.1f (curr), %3.1f (rate), %3.1f (clip), %3.1f (off), %3.1f (gratio)",
			//	joint, joints[joint].posPIDinfo.goalInDegrees, posInDegrees, joints[joint].servoCurrPos, rateInDegPerSec, clippedPos, joints[joint].JCSoffsetDeg, joints[joint].gearRatio);
		}
	}

	//releaseCPU();

}

void updateGJointPoses() {
	g_CurrJointAngles.joint1 = ReadArmJointGoal(1);
	g_CurrJointAngles.joint2 = ReadArmJointGoal(2);
	g_CurrJointAngles.joint3 = ReadArmJointGoal(3);
	g_CurrJointAngles.joint4 = ReadArmJointGoal(4);
	g_CurrJointAngles.joint5 = ReadArmJointGoal(5);
}

//////////////////////////////////////////////////////////////////////////////////
//
// Command the arm to stop moving and hold the current position
//
void ArmJoint_Hold(int joint)
{
	float currPos = ReadArmJointAngle(joint);
	ArmJoint_MoveToPosAtRate(joint, currPos, 90);
}

//////////////////////////////////////////////////////////////////////////////////
//
// Return the current position goal for a given joint
//
float ReadArmJointAngle(int joint)
{
	float motorDeg = 0;
	if(joints[joint].motorType == tetrix12VDC)
	{
		if(joints[joint].variableJoint)
		{
			return joints[joint-1].JCSoffsetDeg - ReadArmJointGoal(joint-1) + MotorDegToJointDeg(joint, joints[joint].posPIDinfo.goalInDegrees);
		}
		else
		{
			motorDeg = ReadJointEncoder(joint) / 4;
		}
	}
	else
	{
		motorDeg = joints[joint].posPIDinfo.goalInDegrees;
	}

	return MotorDegToJointDeg(joint, motorDeg);
}

//////////////////////////////////////////////////////////////////////////////////
//
// Return the current position goal for a given joint
//
float ReadArmJointGoal(int joint)
{
	return MotorDegToJointDeg(joint, joints[joint].posPIDinfo.goalInDegrees);
}

//////////////////////////////////////////////////////////////////////////////////
//
// Return true/false the joint is at position goal
//
bool ArmJointIsAtGoal(int joint)
{
	return joints[joint].posPIDinfo.atGoal;
}

//////////////////////////////////////////////////////////////////////////////////
//
// number of joints used on this bot...set by InitArm()
//
bool _armControlTaskReady = false;

bool ArmControlTaskReady()
{
	return _armControlTaskReady;
}

//////////////////////////////////////////////////////////////////////////////////
//
// number of joints used on this bot...set by InitArm()
//
int _numJoints = 0;

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//
// Task responsible for moving the arm to a given position and holding it there.
// Is able to also control speed moving to position.
//
task ArmControlTask()
{
	// bookkeeping for keeping the task loop running on a set cycle
	long nTimeLoop = 0;

	InitArm();

	#ifdef TELEOP
		recallJointAngles();
		resetValues();
	#endif

	_armControlTaskReady = true;

	while ( true )
	{
		// For every joint in the arm, move it to/keep it at its commanded position
		for ( int i = 1 ; i <= _numJoints ; i++ )
		{
			PIDjoint(i);
		}

    // Keep the loop on an as consistent a cycle time as possible, limited by ARM_CONTROL_TASK_CYCLE_TIME
		while( (nPgmTime - nTimeLoop) < ARM_CONTROL_TASK_CYCLE_TIME)
		{
			wait1Msec(1);
		}

		//nxtDisplayTextLine(7, "Arm Task: %d mSec", nPgmTime - nTimeLoop);
		//writeDebugStreamLine("Arm Task: %d mSec", nPgmTime - nTimeLoop);
		nTimeLoop = nPgmTime;
	}

}

/////////////////////////////////////////////////////////
//
// Initialize the arm control system
//
void InitArm()
{
	_numJoints = 0;
	memset(&joints, 0, sizeof(joints));

#ifdef USING_ARM_J1
	InitJoint(1, ARM_J1_MOTOR_ID, ARM_J1_MOTOR_TYPE, ARM_J1_GEAR_RATIO,
							 ARM_J1_JCS_OFFSET_DEG, ARM_J1_STARTING_POS, ARM_J1_MAX_POS, ARM_J1_MIN_POS,
							 ARM_J1_POS_PID_Kp, ARM_J1_POS_PID_Ki, ARM_J1_POS_PID_Kd,
							 ARM_J1_SPD_PID_Kp, ARM_J1_SPD_PID_Ki, ARM_J1_SPD_PID_Kd, ARM_J1_VARIABLE_ANGLE);
	InitJoint2(1, ARM_J1_INVERT_MOTOR, ARM_J1_PID_ENABLED, ARM_J1_KINEMATICS_ENABLED);
#endif

#ifdef USING_ARM_J2
	InitJoint(2, ARM_J2_MOTOR_ID, ARM_J2_MOTOR_TYPE, ARM_J2_GEAR_RATIO,
							 ARM_J2_JCS_OFFSET_DEG, ARM_J2_STARTING_POS, ARM_J2_MAX_POS, ARM_J2_MIN_POS,
							 ARM_J2_POS_PID_Kp, ARM_J2_POS_PID_Ki, ARM_J2_POS_PID_Kd,
							 ARM_J2_SPD_PID_Kp, ARM_J2_SPD_PID_Ki, ARM_J2_SPD_PID_Kd, ARM_J2_VARIABLE_ANGLE);
	InitJoint2(2, ARM_J2_INVERT_MOTOR, ARM_J2_PID_ENABLED, ARM_J2_KINEMATICS_ENABLED);
#endif

#ifdef USING_ARM_J3
	InitJoint(3, ARM_J3_MOTOR_ID, ARM_J3_MOTOR_TYPE, ARM_J3_GEAR_RATIO,
							 ARM_J3_JCS_OFFSET_DEG, ARM_J3_STARTING_POS, ARM_J3_MAX_POS, ARM_J3_MIN_POS,
							 ARM_J3_POS_PID_Kp, ARM_J3_POS_PID_Ki, ARM_J3_POS_PID_Kd,
							 ARM_J3_SPD_PID_Kp, ARM_J3_SPD_PID_Ki, ARM_J3_SPD_PID_Kd, ARM_J3_VARIABLE_ANGLE);
	InitJoint2(3, ARM_J3_INVERT_MOTOR, ARM_J3_PID_ENABLED, ARM_J3_KINEMATICS_ENABLED);
#endif

#ifdef USING_ARM_J4
	InitJoint(4, ARM_J4_MOTOR_ID, ARM_J4_MOTOR_TYPE, ARM_J4_GEAR_RATIO,
							 ARM_J4_JCS_OFFSET_DEG, ARM_J4_STARTING_POS, ARM_J4_MAX_POS, ARM_J4_MIN_POS,
							 ARM_J4_POS_PID_Kp, ARM_J4_POS_PID_Ki, ARM_J4_POS_PID_Kd,
							 ARM_J4_SPD_PID_Kp, ARM_J4_SPD_PID_Ki, ARM_J4_SPD_PID_Kd, ARM_J4_VARIABLE_ANGLE);
	InitJoint2(4, ARM_J4_INVERT_MOTOR, ARM_J4_PID_ENABLED, ARM_J4_KINEMATICS_ENABLED);
#endif

#ifdef USING_ARM_J5
	InitJoint(5, ARM_J5_MOTOR_ID, ARM_J5_MOTOR_TYPE, ARM_J5_GEAR_RATIO,
							 ARM_J5_JCS_OFFSET_DEG, ARM_J5_STARTING_POS, ARM_J5_MAX_POS, ARM_J5_MIN_POS,
							 ARM_J5_POS_PID_Kp, ARM_J5_POS_PID_Ki, ARM_J5_POS_PID_Kd,
							 ARM_J5_SPD_PID_Kp, ARM_J5_SPD_PID_Ki, ARM_J5_SPD_PID_Kd, ARM_J5_VARIABLE_ANGLE);
	InitJoint2(5, ARM_J5_INVERT_MOTOR, ARM_J5_PID_ENABLED, ARM_J5_KINEMATICS_ENABLED);
#endif

#ifdef USING_ARM_J6
	InitJoint(6, ARM_J6_MOTOR_ID, ARM_J6_MOTOR_TYPE, ARM_J6_GEAR_RATIO,
							 ARM_J6_JCS_OFFSET_DEG, ARM_J6_STARTING_POS, ARM_J6_MAX_POS, ARM_J6_MIN_POS,
							 ARM_J6_POS_PID_Kp, ARM_J6_POS_PID_Ki, ARM_J6_POS_PID_Kd,
							 ARM_J6_SPD_PID_Kp, ARM_J6_SPD_PID_Ki, ARM_J6_SPD_PID_Kd, ARM_J6_VARIABLE_ANGLE);
	InitJoint2(6, ARM_J6_INVERT_MOTOR, ARM_J6_PID_ENABLED, ARM_J6_KINEMATICS_ENABLED);
#endif

	// keep number of joints in range.
	VERIFY(_numJoints<MAX_NUM_JOINTS);

	// wait a little bit to let the encoders reset...
	wait1Msec(50);
}

/////////////////////////////////////////////////////////////////////////
//
// Initialize a 12VDC motor joint
//
void InitJoint(int joint, tMotor motorID, tMotorType motorType, float gearRatio,
							 float JCSoffset, float startPos, float maxPos, float minPos,
							 float posKp, float posKi, float posKd, float spdKp, float spdKi, float spdKd, bool variableJoint)
{
	joints[joint].motorID = motorID;
	InitJoint(joint, motorType, gearRatio, JCSoffset, startPos, maxPos, minPos,
							     posKp, posKi, posKd, spdKp, spdKi, spdKd, variableJoint);
}

/////////////////////////////////////////////////////////////////////////
//
// Initialize a servo motor joint
//
void InitJoint(int joint, TServoIndex servoID, tMotorType motorType, float gearRatio,
						   float JCSoffset, float startPos, float maxPos, float minPos,
							 float posKp, float posKi, float posKd, float spdKp, float spdKi, float spdKd, bool variableJoint)
{
	joints[joint].servoID = servoID;
	InitJoint(joint, motorType, gearRatio, JCSoffset, startPos, maxPos, minPos,
							     posKp, posKi, posKd, spdKp, spdKi, spdKd, variableJoint);
}

/////////////////////////////////////////////////////////////////////////
//
// Initialize the rest of the joint (all except the motor/servo ID)
// ...keep track of how many joints we have.  Service routine for above.
//
void InitJoint(int joint, tMotorType motorType, float gearRatio,
							 float JCSoffset, float startPos, float maxPos, float minPos,
							 float posKp, float posKi, float posKd, float spdKp, float spdKi, float spdKd, bool variableJoint)
{
	_numJoints++;
	joints[joint].jointNumber = _numJoints;
	joints[joint].motorType = motorType;
	//writeDebugStreamLine("%d G Ratio %f", joint, gearRatio);
	joints[joint].gearRatio = gearRatio;

	// The JCSoffset allows us to map between joint coordinate system and world coord sys.
	// Joint zero is based on the motor.  For a 12VDC motor, zero is where ever it starts.
	// For a servo, zero is the hard stop.  JCSoffsetDeg is the number of degrees the
	// starting position is off from world reference zero.  All other position references
	// are in world coord sys.
	joints[joint].JCSoffsetDeg = JCSoffset;

	joints[joint].posPIDinfo.goalInDegrees = JointDegToMotorDeg(joint, startPos); // startPos is in world coord sys.
	joints[joint].posPIDinfo.atGoal = false;
	joints[joint].posPIDinfo.pValue = 4 * joints[joint].posPIDinfo.goalInDegrees;
	joints[joint].spdPIDinfo.pValue = 4 * joints[joint].posPIDinfo.goalInDegrees;

	joints[joint].spdPIDinfo.goalInDegrees = 90;
	joints[joint].spdPIDinfo.atGoal = false;

	joints[joint].maxPosInDegrees = maxPos;
	joints[joint].minPosInDegrees = minPos;

	joints[joint].posPIDinfo.Kp = posKp;
	joints[joint].posPIDinfo.Ki = posKi;
	joints[joint].posPIDinfo.Kd = posKd;

	joints[joint].spdPIDinfo.Kp = spdKp;
	joints[joint].spdPIDinfo.Ki = spdKi;
	joints[joint].spdPIDinfo.Kd = spdKd;

	joints[joint].allowedError = 12;

	joints[joint].variableJoint = variableJoint;

	ResetJointEncoder(joint);
}

void InitJoint2(int joint, bool bInvertMotor, bool PIDEnabled, bool KinematicsEnabled)
{
	joints[joint].bInvertMotor = bInvertMotor;
	joints[joint].bPIDEnabled = PIDEnabled;
	joints[joint].bKinematicsEnabled = KinematicsEnabled;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Control a joint moving/keeping it at a commanded position.  Move at commanded rate
//
void PIDjoint(int joint)
{
	if(joints[joint].bPIDEnabled)
	{
		if ( joints[joint].motorType == tetrix12VDC )
		{
			PID_12VDC_Joint(joint);
		}
		else if ( joints[joint].motorType == tetrixServo )
		{
			PID_Servo_Joint_RateControlled(joint);
		}
	}
}

// control a 12VDC motor
void PID_12VDC_Joint(int joint)
{
	float goalInDegreesConverted;

	if(joints[joint].variableJoint)
	{
			float j3 = MotorDegToJointDeg(joint, joints[joint].posPIDinfo.goalInDegrees);
			float j3Start = joints[joint].JCSoffsetDeg;
			float j2 = ReadArmJointAngle(joint-1);
			float j2Start = joints[joint-1].JCSoffsetDeg;
			goalInDegreesConverted = JointDegToMotorDeg(joint, j3 + j2 - j2Start);
	}
	else
	{
		goalInDegreesConverted = joints[joint].posPIDinfo.goalInDegrees;
	}
	int spd = calcPosPID(joint, goalInDegreesConverted, joints[joint].spdPIDinfo.goalInDegrees);
	setMotorSpeedPID(joint, spd);

	#define ARM_STALL_PROTECTION_SAMPLE_COUNT 8

	#ifdef ENABLE_ARM_STALL_PROTECTION
		static long encoderReadings[ARM_STALL_PROTECTION_SAMPLE_COUNT];
		static int motorSpeeds[ARM_STALL_PROTECTION_SAMPLE_COUNT];
		static int sample = 0;
		encoderReadings[sample] = ReadJointEncoder(joint);
		motorSpeeds[sample] = joints[joint].motorPower;
		if(sample == ARM_STALL_PROTECTION_SAMPLE_COUNT - 1) {
			long encoderSum = 0;
			int motorPowerSum = 0;
			int motorPowerAvg = 0;
			for(int i = 0; i < ARM_STALL_PROTECTION_SAMPLE_COUNT; i++) {
				encoderSum += encoderReadings[i];
				motorPowerSum += motorSpeeds[i];
			}
			motorPowerAvg = motorPowerSum/ARM_STALL_PROTECTION_SAMPLE_COUNT;
			if(motorPowerAvg > 40 && encoderSum < 5) {
				assert();
			}
			sample = 0;
		}
	#endif

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// control a servo --- this is massively broken
void PID_Servo_Joint(int joint)
{
	int adjGoal = joints[joint].posPIDinfo.goalInDegrees;
	adjGoal = MotorDegToJointDeg(joint, joints[joint].posPIDinfo.goalInDegrees) - joints[joint].minPosInDegrees;
	adjGoal *= joints[joint].gearRatio;
	long clipped = clipf(adjGoal, 0, 180);
	adjGoal = clipped * 255 / 180;
	servo[joints[joint].servoID] = (int)adjGoal;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// rate control a servo --- this is pretty messed up ... will only work if the joint is not geared
void PID_Servo_Joint_RateControlled(int joint)
{
	static int count = 0;

	// rate control the servo by moving the servo based on the commanded rate and the current time slice
	float adjGoal = joints[joint].posPIDinfo.goalInDegrees;
	float distGoal = adjGoal - joints[joint].servoCurrPos;
	int dir = distGoal / abs(distGoal);
	distGoal = abs(distGoal); // drop the sign so we can find the true minimum between raw distance and time slice distance
	float distCycle = joints[joint].spdPIDinfo.goalInDegrees * ARM_CONTROL_TASK_CYCLE_TIME / 1000;
	float delta = minf(distGoal, distCycle);
	delta *= dir; // reapply the sign for direction
	float newPos = joints[joint].servoCurrPos + delta;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// quick hack to bypass the rate control because we're going to try to directly drive J4 (the only servo joint)
	newPos = joints[joint].posPIDinfo.goalInDegrees;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if(joints[joint].bInvertMotor) {
		 adjGoal = joints[joint].maxPosInDegrees - MotorDegToJointDeg(joint, newPos);
	}
	else {
		adjGoal = MotorDegToJointDeg(joint, newPos) - joints[joint].minPosInDegrees;
	}

	adjGoal = adjGoal * joints[joint].gearRatio;

	long clipped = clipf(adjGoal, 0, 180);

	if ( delta != 0 && joint == JOINT_TO_FOLLOW )
	{
		count++;
		writeDebugStreamLine("%d goal: %3.1f, adj: %d, curr: %3.1f, dg: %3.1f, dc: %3.1f, d: %3.1f, rate: %3.1f, pos: %3.1f",
				count, joints[joint].posPIDinfo.goalInDegrees, clipped, joints[joint].servoCurrPos, distGoal, distCycle, delta, joints[joint].spdPIDinfo.goalInDegrees, newPos);
	}
	adjGoal = clipped * 255 / 180;
	servo[joints[joint].servoID] = (int)adjGoal;
	joints[joint].servoCurrPos = newPos;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// Joint position and speed control PID algorithms for 12VDC motors
// Work in the motor reference coordinate system
//
void setMotorSpeedPID(int joint, int goalMotorDegreesPerSecond)
{
	tPIDinfo *pid = &joints[joint].spdPIDinfo;

	long currentTime = nPgmTime;
	long deltaTime = currentTime - pid->lastTime;
	long lastTime = pid->lastTime;
	pid->lastTime = currentTime;
	long speedCurrentEncoderCount = ReadJointEncoder(joint);
	long deltaEncoderCount = speedCurrentEncoderCount - pid->pValue;
	long lastEncoderCount = pid->pValue;
	pid->pValue = speedCurrentEncoderCount;
	float currentTicksPerSecond = ((float)deltaEncoderCount * (1000.0 / (float)deltaTime));
	float goalTicksPerSecond = ((float)goalMotorDegreesPerSecond * 4.0);
	long speedError = goalTicksPerSecond - currentTicksPerSecond;
	pid->integral += speedError;
	pid->derivative = (speedError - pid->lastError);
	pid->lastError = speedError;
	long speedOutput = (speedError * pid->Kp) + (pid->integral * pid->Ki) + (pid->derivative * pid->Kd);
	if ( joint == JOINT_TO_FOLLOW )
	{
		static long timeSinceLastSpeed = nPgmTime;
		static bool wroteDebugStream = false;

		//writeDebugStream("SPD j: %d, goal: %d/%4.2f, delta: %d, err: %d, time: %d, Sout: %d, Int: %d",
		//											joint, goalMotorDegreesPerSecond, goalTicksPerSecond,
		//											(int)deltaEncoderCount, (int)speedError, (int)deltaTime, speedOutput, (int)pid->integral);
	}
	SetJointPower(joint, speedOutput);
}

int calcPosPID(int joint, int goalMotorPosDegrees, int goalMotorDegreesPerSecond)
{
	static long startTime = nPgmTime;
	tPIDinfo *pid = &joints[joint].posPIDinfo;

	float output = 0;
	long posCurrentEncoderCount = ReadJointEncoder(joint);
	long goalMotorPosTicks = goalMotorPosDegrees * 4;
	long posError = goalMotorPosTicks - posCurrentEncoderCount;
	string s = "hold";
	if (abs(posError) > joints[joint].allowedError)
	{
		pid->atGoal = false;
		pid->integral += posError;
		pid->derivative = (posError - pid->lastError);
		pid->lastError = posError;
		output = (posError * pid->Kp) + (pid->integral * pid->Ki) + (pid->derivative * pid->Kd);
		s = "move";
	}
	else if (!pid->atGoal)
	{
		pid->atGoal = true;
	}

	// the following implies spdGoal should never be negative...
	if (output > goalMotorDegreesPerSecond && output > 0)
	{
		output = goalMotorDegreesPerSecond;
	}
	else if (output < -goalMotorDegreesPerSecond && output < 0)
	{
		output = -goalMotorDegreesPerSecond;
	}

	//output =

	if ( joint == JOINT_TO_FOLLOW )
	{
		long currTime = nPgmTime;
		long deltaTime = currTime - startTime;
		startTime = nPgmTime;
		//writeDebugStreamLine("POS j: %d, %s, goal: %d/%d, curr: %d,err: %d(%d), time: %d, Pout: %d, ",
			//										joint, s, goalMotorPosDegrees, (int)goalMotorPosTicks,
			//										(int)posCurrentEncoderCount, posError, joints[joint].allowedError, (int)deltaTime, output);
		writeDebugStreamLine("Goal Pos: %f Curr Pos: %f Error: %d Goal Deg/s: %d",
												MotorDegToJointDeg(2, goalMotorPosDegrees),
												MotorDegToJointDeg(2, posCurrentEncoderCount / 4),
												MotorDegToJointDeg(2, posError / 4),
												goalMotorDegreesPerSecond);
	}
	return output;
}

//////////////////////////////////////////////////////////////////////////////
//
// Joint 12VDC motor wrappers
//
void ResetJointEncoder(int joint)
{
	nMotorEncoder[joints[joint].motorID] = 0;
}

long ReadJointEncoder(int joint)
{
	return nMotorEncoder[joints[joint].motorID];
}

void SetJointPower(int joint, long power)
{
	if(!g_DisableArm) {
		motor[joints[joint].motorID] = power;
		joints[joint].motorPower = power;
		if ( joint == JOINT_TO_FOLLOW )
		{
			//writeDebugStreamLine("pwr: %d", (int)power);
		}
	}
	else {
		motor[joints[joint].motorID] = 0;
		joints[joint].motorPower = 0;
	}
}

void DisableArm() {
	g_DisableArm = true;
}

float MotorDegToJointDeg(int joint, float deg)
{
	return deg * joints[joint].gearRatio + joints[joint].JCSoffsetDeg;
}

float JointDegToMotorDeg(int joint, float deg)
{
	return (deg - joints[joint].JCSoffsetDeg) / joints[joint].gearRatio;
}

void disablePID(int joint)
{
	joints[joint].bPIDEnabled = false;
}

void enablePID(int joint)
{
	joints[joint].bPIDEnabled = true;
}

/////////////////////////////////////////////////////////
void JoyControlJoint(int joint, float pos, float rate)
{
	if ( rate != 0 ) {
		ArmJoint_MoveToPosAtRate(joint, pos, rate);
		writeDebugStreamLine("JoyControlJoint(%d, %3.1f, %3.1f)", joint, pos, rate);
	}
	else {
		ArmJoint_Hold(joint);
		writeDebugStreamLine("HOLD J1 -------------");
	  writeDebugStreamLine("JoyControlJoint(%d, %3.1f, %3.1f)", joint, pos, rate);
	}
}
