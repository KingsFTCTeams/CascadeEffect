#ifndef TEAM_5076_H
#define TEAM_5076_H

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TEAM SPECIFIC SETTINGS ... THESE ARE THE ONLY SETTINGS THAT DIFFER BETWEEN 3717 AND 4590'S BOTS
//
#if TEAM_NUMBER == 5076

	#include "SensorConfig.h"
	#include "DriveSysConfig.h"
	#include "ArmControlConfig.h"
	#include "ArmKinematicsConfig.h"


  //-------------------------------------------------------------------------------------------
	// comment in/out the following individually to enable/disable the feature
	//
	//#define ENABLE_NXT2EXCEL_DEBUGGING

  //-------------------------------------------------------------------------------------------
	// Alerts for various subsystems
	// ... unless the flag has a value such as ALERTS_DEFAULT_STATE_ON, just comment in or out to enable/disable
  #define ALERTS_DEFAULT_STATE_ON true // set to true or false ... causes alert lights to be on (or off) by default
  //#define ENABLE_COLLECTION_ALERTS
  //#define ENABLE_PAYLOAD_ALERTS
  //#define ENABLE_SPECIAL_RESOURCE_ALERT

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// GAME SPECIFIC VALUES - these will change from year to year
	//----------------------------------------------------------------------------------------------
  // Controller Mappings for Payload Operator (joystick 2)
  //		LT: 		Collector in
  //    RT: 		Collector out
  //		A:  		Lift to Collect
  //    B:  		Lift to Low
  //    X:  		Lift to Mid
  //    Y:  		Lift to High
  //		RB: 		Lift to Collect
  #define BTN_COLLECTOR_IN  BtnLT
  #define BTN_COLLECTOR_OUT BtnRT

  #define BTN_LIFT_COLLECT  BtnA
  #define BTN_LIFT_LOW			BtnB
	#define BTN_LIFT_MID		  BtnX
	#define BTN_LIFT_HIGH     BtnY
  #define BTN_LIFT_CENTER   BtnRB

	#define BTN_PLATFORM_OUT  joy2TopHat_N()
	#define BTN_PLATFORM_IN   joy2TopHat_S()


  //-------------------------------------------------------------------------------------------
	// Controller mappings for driver (joystick 1)
	//    LT:       Servo Control
	//    LB:       Turbo (full) Power Drive ... normal is 1/2 power
	//    RT:       Ball Collection Reverse
	//    RB:       Ball Collection Forward
	//    LJoy:     Left Drive Train
	//    RJoy:     Right Drive Train
	//    DPad:     unused
	//    A:        Drive straight backwards
	//    B:        unused
	//    X:        unused
	//    Y:        Drive straight forwards
	//    Start:    unused
	//
	#define BTN_DRIVE_TURBO_PWR   BtnLB


  #define BTN_SERVO_DOWN         	BtnLT
  #define BTN_SERVO_UP			      BtnRT

  #define BTN_DRIVE_STRAIGHT_FWD   BtnY
  #define BTN_DRIVE_STRAIGHT_REV   BtnA

  #define BTN_DRIVE_SPIN_LEFT      BtnX
  #define BTN_DRIVE_SPIN_RIGHT     BtnB

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#include "../Common/CommonConfig.h"

#else

  xxx YOU MUST USE ONE OF THE TEAM #DEFINES IN THE Autonomous Template.h FILE BONEHEAD! xxx

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TEAM_5076_H
