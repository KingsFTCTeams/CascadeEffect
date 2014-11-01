/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Module: SensorTask.h
//
// Description:
//        Handles all reading of sensors
//          * Accelerometer
//          * IR1 sensor
//          * IR2 sensor
//          * Ultrasonic sensor
//          * Magnetic sensor
//          * Gyro
//
// Usage:
//        1) Run the SensorValuesTask task
//        2) Call the appropriate function to read the desired sensor value
//           ... gyro state is maintained in gyro.h
//
// Direct Motor Use:
//        none
//
// Direct Sensor Use:
//          * Accelerometer
//          * IR1 sensor
//          * IR2 sensor
//          * Ultrasonic sensor
//          * Magnetic sensor
//          * Gyro
//
// Timer Use:
//        none
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H

#pragma systemFile            // this eliminates warning for "unreferenced" functions

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
typedef enum
{
  NOT_USED,
  ON_BRICK,
  ON_SMUX

} tSensorLocation;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "..\Common\Hitech Drivers\hitechnic-sensormux.h"
#include "..\Common\Hitech Drivers\hitechnic-magfield.h"
#include "..\Common\Hitech Drivers\hitechnic-irseeker-v2.h"
#include "..\Common\Hitech Drivers\lego-ultrasound.h"
#include "..\Common\Hitech Drivers\hitechnic-accelerometer.h"
#include "..\Common\Hitech Drivers\hitechnic-gyro.h"
#include "Gyro.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// #defines
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public Global Variables
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private Global Variables ... gyro state is maintained in gyro.h
//
int _nAccelSensorXValue = 0;
int _nAccelSensorYValue = 0;
int _nAccelSensorZValue = 0;
int _nIR1SensorValue = 0;
int _nIR2SensorValue = 0;
int _nUS1SensorValue = 0;
int _nUS2SensorValue = 0;
int _nMagnetSensorValue = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public Function Declarations ... gyro state is maintained in gyro.h
//
#define AccelSensorXVal()	      ( _nAccelSensorXValue )
#define AccelSensorYVal() 	    ( _nAccelSensorYValue )
#define AccelSensorZVal()	      ( _nAccelSensorZValue )
#define IR1SensorVal()	        ( _nIR1SensorValue )
#define IR2SensorVal()	        ( _nIR2SensorValue )
#define US1SensorVal()	        ( _nUS1SensorValue )
#define US2SensorVal()	        ( _nUS2SensorValue )
#define USsensorVal()	          ( (_nUS1SensorValue + _nUS2SensorValue)/2 )
#define MagneticSensorVal() 	  ( _nMagnetSensorValue )

/////////////////////////////////////////////////////////////////////////////////////////
//
// PRIVATE FUNCTION: initialization
//
#define initSensorValues()            \
{                                     \
	_nAccelSensorXValue = 0;            \
  _nAccelSensorYValue = 0;            \
	_nAccelSensorZValue = 0;            \
	_nIR1SensorValue = 0;               \
	_nIR2SensorValue = 0;               \
	_nUS1SensorValue = 0;               \
	_nUS2SensorValue = 0;               \
	_nMagnetSensorValue = 0;            \
  InitMAG();                          \
  InitIR1();                          \
  InitIR2();                          \
  InitUS1();                          \
  InitUS2();                          \
  InitAccel();                        \
  InitGyro(GYRO_LOC);                 \
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private Function Declarations
//


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Task Definitions
//
//    Keep all sensor values updated for sensors that are connected to the SMUX
//
task SensorTask()
{
	initSensorValues();

	int bReadHalfSampleRateSensors = true;

	long nPrevTime = nPgmTime;

	while (true)
	{
	  // Run these every other cycle...
    if ( bReadHalfSampleRateSensors )
    {
    }

    // Run these every cycle...
    _nUS1SensorValue = ReadUS1();
    _nUS2SensorValue = ReadUS2();

    UpdateGyroInfo();

	  ReadAccel();
    _nIR1SensorValue = ReadIR1();
    _nIR2SensorValue = ReadIR2();
    _nMagnetSensorValue = ReadMAG();

    // Toggle the "run every other cycle" flag...
    bReadHalfSampleRateSensors = !bReadHalfSampleRateSensors;

    wait1Msec(SENSOR_TASK_SAMPLE_RATE);
    /*
    // Keep the loop on an as consistent a cycle time as possible, limited by SENSOR_TASK_SAMPLE_RATE
	  long nCurrTime = nPgmTime;
	  int nTimeDelta = nCurrTime - nPrevTime;
    nPrevTime = nCurrTime;
    int nSleepTime = SENSOR_TASK_SAMPLE_RATE - nTimeDelta;

	  long nStartTime = nPgmTime;
    while ( nSleepTime > 0 && (nPgmTime - nStartTime) < nSleepTime )
  		wait1Msec(1);
  	*/
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Function Definitions
//

#endif // SENSOR_TASK_H
