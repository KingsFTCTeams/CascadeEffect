#define DISPLAY_CRATE_TEST_DIAGS

#include "GlobalDefines.h"

//#include "JoystickDriver.c"
#include "Drivers/JoystickDriverWithLostFCSCheck2012.h"

#include "Control/Game Specific/2011 Bowled Over/SubSystems/ScissorBot/BallCollection.h"
#include "Control/Game Specific/2011 Bowled Over/SubSystems/ScissorBot/CrateGripAndClaw.h"
#include "Control/Game Specific/2011 Bowled Over/SubSystems/ScissorBot/CrateLiftTask.h"
#include "Control/Game Specific/2011 Bowled Over/SubSystems/ScissorBot/ScissorControl.h"

#include "Control/Game Specific/2011 Bowled Over/SubSystems/ScissorBot/CrateRotateTask.h"
#include "Control/Game Specific/2011 Bowled Over/SubSystems/ScissorBot/BallDispensingTask.h"

void InitializeRobot()
{
  InitScissorLiftSystem();
  InitBallCollectionSystem();
  InitCrateGripAndClaw();

  StartTask(CrateRotationTask);
  StartTask(BallDispenserTask);
  StartTask(CrateLiftTask);
}

task main()
{
  InitializeRobot();

  int nCrateLiftManualPower = 0;

  while ( nNxtButtonPressed != kEnterButton )
  {
    nCrateLiftManualPower = 0;

    if ( nNxtButtonPressed == kLeftButton )
      nCrateLiftManualPower = 20;

    else if ( nNxtButtonPressed == kRightButton )
      nCrateLiftManualPower = -20;

    SetCrateLiftManualPower(nCrateLiftManualPower);
    MoveCrateLift(CRATE_LIFT_DEST_MANUAL_NO_LIMITS_MOVEMENT);
  }
}
