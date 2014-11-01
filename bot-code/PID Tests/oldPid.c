#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     motorJ1,       tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorJ2,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motorJ3,       tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    motorJ4,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    motorJ5,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define TEAM_NUMBER 3717
#include "../3717/Includes/ArmControlConfig.h"
#include "../Common/Control/ArmControl.h"

void updateSpeedPID(int goalDegreesPerSecond);
int updatePosPID(int goalDegreesPerSecond, int goalPosDegrees);

//speed PID settings
float speedKp = 0.009;
float speedKi = 0.002;
float speedKd = 0.0;

//position PID settings
float posKp = 1.5;
float posKi = 0.0;
float posKd = 1.88;

long pTime = 0;

bool goneUp = false;

task main() {
        nMotorEncoder[motorJ1] = 0;
        int posOutput = 0;
        int goalPosition = -10;
        while (true) {
                posOutput = updatePosPID(90, goalPosition);
                updateSpeedPID(posOutput);
                if(nPgmTime - pTime > 4000) {
                        if(goalPosition > -90 && !goneUp) {
                                goalPosition -= 10;
                        } else if(goalPosition == -90 && !goneUp) {
                                goneUp = true;
                        }
                        if(goalPosition < 90 && goneUp) {
                                goalPosition += 10;
                        } else if(goalPosition == 90 && goneUp) {
                                goneUp = false;
                        }
                        pTime = nPgmTime;
                }
                wait1Msec(20);
        }
}

int updatePosPID(int goalDegreesPerSecond, int goalPosDegrees) {
        static float posI = 0.0;
        static float posD = 0.0;
        static long error = 0;
        static long lastError = 0;
        static long currentEncoderCount = 0;
        static long goalPosTicks = 0;
        static int output = 0;

        currentEncoderCount = nMotorEncoder[motorJ1];
        goalPosTicks = goalPosDegrees * 4 * 3;
        error = goalPosTicks - currentEncoderCount;
        if(abs(error) > 40) {
                posI += error;
                posD = (error - lastError);
                lastError = error;
                output = (error * posKp) + (posI * posKi) + (posD * posKd);
        } else {
                output = 0;
        }



        if(output > goalDegreesPerSecond && goalDegreesPerSecond > 0) output = goalDegreesPerSecond;
        if(output < -goalDegreesPerSecond && goalDegreesPerSecond < 0) output = -goalDegreesPerSecond;

        writeDebugStreamLine("Curr Enc Count: %d Goal Enc Count: %d Error: %d", currentEncoderCount, goalPosTicks, error);

        return output;
}

void updateSpeedPID(int goalDegreesPerSecond) {
        static float speedI = 0.0;
        static float speedD = 0.0;
        static long error = 0;
        static long lastError = 0;
        static long currentTime = 0;
        static long lastTime = 0;
        static long deltaTime = 0;
        static long currentEncoderCount = 0;
        static long lastEncoderCount = 0;
        static long deltaEncoderCount = 0;
        static float currentTicksPerSecond = 0;
        static float goalTicksPerSecond = 0;
        static int output = 0;

        currentTime = nPgmTime;
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        currentEncoderCount = nMotorEncoder[motorJ1];
        deltaEncoderCount = currentEncoderCount - lastEncoderCount;
        lastEncoderCount = currentEncoderCount;
        currentTicksPerSecond = ((float)deltaEncoderCount * (1000.0 / (float)deltaTime));

        goalTicksPerSecond = ((float)goalDegreesPerSecond * 4.0);

        error = goalTicksPerSecond - currentTicksPerSecond;
        speedI += error;
        speedD = (error - lastError);
        lastError = error;

        output = (error * speedKp) + (speedI * speedKi) + (speedD * speedKd);
        motor[motorJ1] = (int)output;

        //writeDebugStreamLine("Ticks/Second: %d | Goal Ticks/Sec: %d | Error: %d | Output: %d", currentTicksPerSecond, goalTicksPerSecond, error, output);
}