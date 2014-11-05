/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TEAM SPECIFIC SETTINGS ... THESE ARE THE ONLY SETTINGS THAT DIFFER BETWEEN 3717 AND 4590'S BOTS
//
//    TO SELECT A TEAM, COMMENT IN YOUR TEAM IN Autonomous Template.h AND COMMENT OUT THE OTHER TEAM
//
#if TEAM_NUMBER == 3717

  #include "team3717.h"

#elif TEAM_NUMBER == 4590

  #include "team4590.h"

#else

  xxx YOU MUST USE ONE OF THE TEAM #DEFINES IN THE Autonomous Template.h FILE BONEHEAD! xxx

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Timer Use:
//
// --- MAIN TASK TIMER USE - uses T1, T2, T3 --------------------------------------------------------------------
//
//      Dispensing.h
//        T1 - used to timeout attempts to obtain the proper distance from the wall when preparing to dispense
//
//      ArmControl.h
//        T1 - arm elevation
//        T1 - arm rotation  (we'll never be elevating and rotating the arm at the same time in autonomous)
//
//      BridgeControl.h
//        T1 - used to time agitator when actually dispensing batons
//
//      DriveControl.h, DriveControlPID.h, DriveControlEncoder.h
//        T2 -  used for "poor man's" stall protection (driving straight/turning timeouts)
//              also used in "real" stall protection to determine time slice intervals for expected encoder ticks
//        T3 -  used for "real" stall protection to time how long it has been since power was applied
//              to determine if drive system is ramped up allowing us to expect to see appropriate encoder ticks
//        CT1 - used for "real" stall protection to time holding the stall flag true once a stall is detected
//
// --- DetectMagnet TASK TIMER USE - uses T4 ------------------------------------------------------------------
//        T4 - times how long since we detected the magnet
//
