/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Module: gyro.h
//
// Description:
//        Encapsulates all access to the gyro
//
// Usage:
//        1) Call InitGyroInfo passing in the gyro ID for the gyro being used
//           Each gyro has slightly different drift, and is calibrated empirically.  The gyro ID identifies the
//           scalar to use to compensate for that particular gyro's drift.
//        2) In a loop somewhere, call UpdateGyroInfo()
//        3) Access the Gyro structure object to obtain the current angle (heading), i.e. Gyro.fAngle
//        4) Call ResetGyro() to reset the gyro angle/heading info so as to obtain a fresh relative angle
//
// Direct Motor Use:
//        none
//
// Direct Sensor Use:
//        none
//
// Timer Use:
//        none
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GYRO_H
#define GYRO_H

#pragma systemFile            // this eliminates warning for "unreferenced" functions

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC API
//
//    The global/public gyro information:
//    ...this is intended to provide access to user level gyro information
//    ...the parameters controlling the Runge-Kutta integration are encapsulated inside UpdateGyroInfo()
//       with the exception of f3_private which is not intended for general use...oh for C++ and
//        classes with private data members!
//
typedef struct
{
  // Public / general use data
	bool  bGyroDisabled;								 // true if unable to use the gyro ... couldn't determine a bias

  float fAngle;                        // current angle reading ... set from one of the following (controlled in teamconfig.h)

  bool  bRotating;										 // true if the bot thinks we are rotating

  float fAngleByDebiasedRotRate;       // angle calculated using debiased rotational rate
  float fAngleByWeightedAvg;           // angle calculated using a weighted average of debiased rotational rate
  float fAngleByTrapezoidalApprox;     // angle calculated using a trapezoidal rule approximation (using debiased rotation rate)

  // Private data not intended for general use...used inside UpdateGyroInfo()
  tSensorLocation   nGyroLoc;     // where the gyro is plugged in...behaves differently and needs to be scaled

  float fRawRotRate;              // current raw (not debiased) angular rate of change
  float fRotBias;                 // initial/static bias of raw gyro reading (weighted average of angular rate of change)

  float fDebiasedRotRate;         // debiased current angular rate of change
  float fPriorDebiasedRotRate;    // prior debiased angular rate of change

  float fWeightedAvgRotRate;      // ...above ran through a low-pass filter
  float fPriorWeightedAvgRotRate; // ...prior weighted average rotational rate

  float fAngleScale;              // scaling factor to be applied to the calculated angle ... each gyro is slightly different

  long  lTimeDelta;               // amount of time (mSec) between last two readings
  long  lPrevTime;                // time stamp of the last reading
  float fTimeSlice;               // how much of a second has passed since last reading


} tGyroInfo;

tGyroInfo Gyro;

void InitGyro(tSensorLocation nGyroLoc);
void ResetGyro();
void UpdateGyroInfo();

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: ZeroGyroInfo
//
// Description: Zero out the global Gyro data
//
// Input:  none
//
// Output: none ... updates the global gyro info structure
//
void ZeroGyroInfo()
{
	Gyro.bRotating = false;
  Gyro.fWeightedAvgRotRate = 0.0;
  Gyro.fAngle = 0.0;
  Gyro.fAngleByDebiasedRotRate = 0.0;
  Gyro.fAngleByWeightedAvg = 0.0;
  Gyro.fAngleByTrapezoidalApprox = 0.0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: CalibrateGyro
//
// Description: Calculate initial:
//                -Gyro bias: raw reading (rotational information) bias (arithmetic average)
//
//              Bias can be safely determine in 100-200 samples
//              Gryo can be read about 333 times a second (every 3 mSecs)
//
//              *** ASSUMES THE SENSOR IS STATIONARY ***
//
// Input:  none
//
// Output: none ... updates the global gyro info structure
//
#define BIAS_CALC_SAMPLE_SIZE 125 // want this as low as possible so have as little delay at start of teleop as possible
#define BIAS_CALC_LOOP_TIME     5 // mSecs to sleep before getting another sample reading

#define GYRO_RAW_RANGE 1024
int rawValFreqs[GYRO_RAW_RANGE+1];

void CalibrateGyro()
{
	// init our frequency array
	for (int i = 0 ; i < GYRO_RAW_RANGE ; i++ )
	{
		rawValFreqs[i] = 0;
	}

	// Read our sample set, recording frequency of raw values
  int _numSamples = BIAS_CALC_SAMPLE_SIZE;
  for (int i = 0; i < _numSamples; i++)
  {
  	int raw = ReadGYRO();
    rawValFreqs[raw] += 1;
    wait1Msec(BIAS_CALC_LOOP_TIME);
  }

  // Find the raw value that was seen the most
  int max = -1;
  int maxIndex = -1;
	for (int i = 0 ; i < GYRO_RAW_RANGE ; i++ )
	{
		if ( rawValFreqs[i] > max )
		{
			maxIndex = i;
			max = rawValFreqs[i];
		}
	}

	// If we couldn't find a max something is horribly wrong and we're not going to be able to find a bias...
	// If we can't find a bias, we can't run the gyro...
	if ( maxIndex < 0 || GYRO_RAW_RANGE < maxIndex )
	{
		Gyro.bGyroDisabled = true;
		ZeroGyroInfo();
	}
	else
	{
		Gyro.bGyroDisabled = false;

		// Find its neighbor that has the next most hits...normal raw values are within a range of 1 when not rotating
		int nxtIndex = maxIndex - 1;
		if ( rawValFreqs[maxIndex+1] > rawValFreqs[maxIndex-1] )
			nxtIndex = maxIndex + 1;

	  // Our bias is the average of the two most frequent values
		long fSumMaxRaw = (long)maxIndex * rawValFreqs[maxIndex];
		long fSumNxtRaw = (long)nxtIndex * rawValFreqs[nxtIndex];

	  Gyro.fRotBias = (float)(fSumMaxRaw + fSumNxtRaw) / (float)(rawValFreqs[maxIndex] + rawValFreqs[nxtIndex]);
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: InitGyro
//
// Description: Initialize the sensor itself and the global gyro information structure
//
// Input:  tIntegrationMode nMode   (type of integration to use when determining gyro angle)
//
// Output: none ... updates the global gyro info structure
//
void InitGyro(tSensorLocation nGyroLoc)
{
  Gyro.nGyroLoc = nGyroLoc;

	if ( nGyroLoc ==  NOT_USED )
  {
  	ZeroGyroInfo();
  }
  else
	{
		//bool bPriorDisplayDiags = bDisplayDiagnostics;
	  //bDisplayDiagnostics = false;
	  eraseDisplay();

	  nxtDisplayCenteredBigTextLine(0, "CALIBRATING");
	  nxtDisplayCenteredBigTextLine(3, "DO NOT");
	  nxtDisplayCenteredBigTextLine(6, "MOVE BOT");

	  switch( Gyro.nGyroLoc )
	  {
	    case ON_BRICK:
	            Gyro.fAngleScale = GYRO_ON_BRICK_SCALE;
	            break;
	    case ON_SMUX:
	            //Gyro.fAngleScale = GYRO_ON_SMUX_SCALE;
	            break;
	    default:
	            assert();
	  }

	  InitGYRO();

	  ResetGyro();

	  Gyro.lPrevTime = nPgmTime;

	  eraseDisplay();
	  //bDisplayDiagnostics = bPriorDisplayDiags;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: ResetGyro
//
// Description: Allow resetting of gyro information for relative turning.
//
//              Reinitialize/reset the global gyro angle information only.
//              Preserves the bias/calibration information and only resets
//              angle calculation.
//
// Input:  none
//
// Output: none ... updates the global gyro info structure
//
void ResetGyro()
{
	ZeroGyroInfo();
  CalibrateGyro(); // robot better be stopped!
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: DebiasedRawGyroReading
//
// Description: Return debiased raw gyro readings (angular rate of change)
//
// Input:  none
//
// Output: none ... updates the global gyro info structure
//
#define RUNNING_BIAS_HISTORY_WEIGHT .998
#define RUNNING_BIAS_NEW_VAL_WEIGHT .002

#define MOTIONLESS_DEADBAND 2	// Define motionless as less than this number of deg/sec rotational rate

void DebiasedRawGyroReading()
{
	// nothing to do is the gyro is unusable...
	if ( Gyro.bGyroDisabled )
		return;

  // Get current raw rotational rate (degrees / second)
  Gyro.fRawRotRate = ReadGYRO();

  // Determine if we are rotating (rotation rate >= 2 deg/sec in either direction)
  // If we are not rotating, keep a running update of bias using a weighted average.
  Gyro.bRotating = true;
  if ( Gyro.fRotBias-MOTIONLESS_DEADBAND < Gyro.fRawRotRate && Gyro.fRawRotRate < Gyro.fRotBias+MOTIONLESS_DEADBAND )
	{
		Gyro.bRotating = false;
		Gyro.fRotBias = RUNNING_BIAS_HISTORY_WEIGHT * Gyro.fRotBias + RUNNING_BIAS_NEW_VAL_WEIGHT * Gyro.fRawRotRate;
	}

  // The rotational rate is degrees / second ... figure out how much of a second since our last reading
  long  lCurrTime = nPgmTime;
  Gyro.lTimeDelta = lCurrTime - Gyro.lPrevTime;
  Gyro.lPrevTime = lCurrTime;
  Gyro.fTimeSlice = (float)Gyro.lTimeDelta / 1000;

  // Determine actual rotational rate
  Gyro.fPriorDebiasedRotRate = Gyro.fDebiasedRotRate;
  Gyro.fDebiasedRotRate = (float)Gyro.fRawRotRate - Gyro.fRotBias;

  // Weighted average of rotational rate
  Gyro.fPriorWeightedAvgRotRate = Gyro.fWeightedAvgRotRate;
  Gyro.fWeightedAvgRotRate = .9*Gyro.fWeightedAvgRotRate + .1*Gyro.fDebiasedRotRate;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TASK: UpdateGyroInfo
//
// Description: Calculate current gyro angle
//
//    The gyro returns angular rotation rate in degrees/second, up to +/-360 degrees/sec.
//    Raw reading range is 0 - 1023.  Typical bias is 580 - 620ish.
//    Subtract the raw reading from the bias and you will have actual rotational rate.
//
//    To determine the actual angular displacement, integrate the debiased rotational rate.
//
void UpdateGyroInfo()
{
	// keep the world sane if we couldn't get the gyro working...
	if ( Gyro.bGyroDisabled == true )
	{
		Gyro.fAngle = 0;
		return;
	}

  // Get the debiased rotational reading (consider deadband), place in Gyro.fDebiasedRotRate
  // ...also calculates the time slice ... number of seconds that have passed since last reading
  DebiasedRawGyroReading();

  // The angular displacement is the rotational rate divided by number of seconds since the last reading...
  // ...additionally each gyro performs slightly different but predictably.  Through observation (manual sensor
  //    calibration, the linear scaling factor is determined for each gyro and applied here.
  Gyro.fAngleByDebiasedRotRate += (Gyro.fDebiasedRotRate * Gyro.fTimeSlice) * Gyro.fAngleScale;

  // ...version using weighted average rotational rate
  Gyro.fAngleByWeightedAvg += (Gyro.fWeightedAvgRotRate * Gyro.fTimeSlice) * Gyro.fAngleScale;

  // ...version using trapezoidal rule approximation
  Gyro.fAngleByTrapezoidalApprox += (Gyro.fTimeSlice) * (Gyro.fPriorDebiasedRotRate + Gyro.fDebiasedRotRate) * (Gyro.fAngleScale) / 2;

  // return the angular displacement based on settings in teamconfig.h
#ifdef GYRO_USE_DEBIASED_ROT_RATE
  Gyro.fAngle = Gyro.fAngleByDebiasedRotRate;

#elif defined(GYRO_USE_WEIGHTED_AVG_ROT_RATE)
  Gyro.fAngle = Gyro.fAngleByWeightedAvg;

#elif defined(GYRO_USE_TRAPEZOIDAL_APPROX)
  Gyro.fAngle = Gyro.fAngleByTrapezoidalApprox;
#else
  *** MUST SELECT GYRO ANGLE METHOD IN TEAMCONFIG.H ***

#endif
}

#endif // GYRO_H
