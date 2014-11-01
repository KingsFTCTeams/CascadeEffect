/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Module: PowerRamping.h
//
// Description:
//        Various methods to ramp power smoothly so that you don't spin out when starting or skid when stopping
//
// Usage:
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

#ifndef POWER_RAMPING_H
#define POWER_RAMPING_H

#pragma systemFile            // this eliminates warning for "unreferenced" functions

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Includes
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// #defines
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public Global Variables
//
typedef enum
{
  COS_CURVE_PWR_RAMP,
  NO_PWR_RAMP

} TPowerRampingType;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private Global Variables
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public Function Declarations
//

//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private Function Declarations
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Task Definitions
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Function Definitions
//

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Ramp power based on cosine function, primary input is percentage of distance traveled
//
float CosPowerCurveForRampedPower(float fGoalDistance, int leftEncoder, int rightEncoder)
{
  const float fAmplitude = COS_PWR_RAMP_AMPLITUDE;  // increasing makes ramp up and down steeper
  const float fCosExponent = COS_PWR_RAMP_EXPONENT; // reducing slightly makes ramp up steeper but doesn't appreciably affect ramp down
  float fMinClip = COS_PWR_RAMP_MIN_CLIP_START;     // offset added to x^2 * cos() function ... establishes minimum power
  const float fMaxClip = COS_PWR_RAMP_MAX_CLIP;     // max power ... output is clipped to this

  float fFractionCompleted = 0;     // fraction of distance completed ... main parameter to cosine function ... the x value
  float fCurrDistance = (leftEncoder + rightEncoder) / 2;

  float fNominalCurve = 0.0;
  float fOffsetClippedCurve = 0.0;

  float fScaledOutputPower = 0.0;

  // Power is determined by a cosine curve ... range of values for fNominalCurve is 0.0 - 1.0
  fFractionCompleted = minf(1.0, fCurrDistance / fGoalDistance); // cap at 100% complete

  fMinClip = (fFractionCompleted > 0.5) ? COS_PWR_RAMP_MIN_CLIP_END : COS_PWR_RAMP_MIN_CLIP_START;

  fNominalCurve = fAmplitude * pow(cos(0.5 * PI * fFractionCompleted), fCosExponent) * pow(fFractionCompleted, 2);
  fOffsetClippedCurve = minf(fNominalCurve + fMinClip, fMaxClip);

  //fScaledOutputPower = (round(fOffsetClippedCurve) * nMaxPower);

	nxtDisplayTextLine(6, "cos:  %3.2f", fNominalCurve);
	nxtDisplayTextLine(7, "clip: %3.2f", fOffsetClippedCurve);

	writeDebugStreamLine("Power: %d fMinClip %6.2f Fraction %6.2f", fOffsetClippedCurve * 100, fMinClip, fFractionCompleted);

  return fOffsetClippedCurve;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTION: Ramp power based in a linear manner
//
float LinearPowerCurveForRampedPower(float fGoalDistance, int leftEncoder, int rightEncoder)
{
  /*
//#define DRIVE_PID_MIN_PWR  13 // minimum power in PID drive control
//#define DRIVE_PID_RAMP_DOWN_TICKS     100 // power ramp down start distance (in encoder ticks)
  const int   nMinPower  = (nMaxPower >= 0) ? DRIVE_PID_MIN_PWR : -DRIVE_PID_MIN_PWR;
  const int   nPwrGradient = nMaxPower - nMinPower;

  const int   nRampDwnDist = fGoalDistance - DRIVE_PID_RAMP_DOWN_TICKS;
  const float fRampDwnSlope = (nMinPower - nMaxPower) / DRIVE_PID_RAMP_DOWN_TICKS;
  const float fRampUpSlope  = (nMaxPower - nMinPower) / DRIVE_PID_RAMP_DOWN_TICKS;

  const bool  bGoingMoreThanHalfSquare = ( fGoalDistance > ONE_SQUARE_ENCODER_COUNT / 2 );

  float fCurrDistance = (leftEncoder + rightEncoder) / 2;

  int nDistRemaining = absValue(fGoalDistance - fCurrDistance);

  int nPwr = 0;

  if ( bGoingMoreThanHalfSquare && nDistRemaining < DRIVE_PID_RAMP_DOWN_TICKS )
  {
    nPwr = fRampDwnSlope * absValue(fCurrDistance - nRampDwnDist) + nMaxPower;
  }
  else if ( bGoingMoreThanHalfSquare )
  {
    nPwr = fRampUpSlope * (fCurrDistance) + nMinPower;
  }
  nPwr = clipf(nPwr, -nMaxPower, nMaxPower);

	nxtDisplayTextLine(5, "min:   %d", nMinPower);
	nxtDisplayTextLine(6, "base:  %d", nMaxPower);
	nxtDisplayTextLine(7, "gradnt:%d", nPwrGradient);

	return nPwr;
	*/
	return 1.0;
}
/*
	    fScaledTicks = DRIVE_PID_PWR_GRADIENT_SCALE * nAvgEncoder;
	    nBleedPwr = nPwrGradient - fScaledTicks;
	    nAdjPwr = nBasePower - nBleedPwr;
	    nPwr = clipf(nAdjPwr, -nBasePower, nBasePower);
*/


#endif // POWER_RAMPING_H
