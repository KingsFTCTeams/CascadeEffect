#define DISPLAY_SCISSOR_TEST_DIAGS
#define SCISSOR_TEST_BUTTON_INPUT

typedef enum
{
  MANUAL,
  LAB,
  FULL

} tMenuOption;

tMenuOption menuOption = MANUAL;

int ScaleJoystickInput(float input);

#include "GlobalDefines.h"

#include "JoystickDriver.c"

#include "Control/Game Specific/2011 Bowled Over/SubSystems/ScissorBot/BallCollection.h"
#include "Control/Game Specific/2011 Bowled Over/SubSystems/ScissorBot/CrateGripAndClaw.h"
#include "Control/Game Specific/2011 Bowled Over/SubSystems/ScissorBot/CrateLiftTask.h"
#include "Control/Game Specific/2011 Bowled Over/SubSystems/ScissorBot/ScissorControl.h"

#include "Control/Game Specific/2011 Bowled Over/SubSystems/ScissorBot/CrateRotateTask.h"
#include "Control/Game Specific/2011 Bowled Over/SubSystems/ScissorBot/BallDispensingTask.h"

void Menu();
void SetScissor();

void InitializeRobot()
{
  InitScissorLiftSystem();
  InitBallCollectionSystem();
  InitCrateGripAndClaw();

  SetChimneyLatchPos(CHIMNEY_LATCH_OPEN);

  StartTask(CrateRotationTask);
  StartTask(BallDispenserTask);
  StartTask(CrateLiftTask);
}

task main()
{
  InitializeRobot();

  _bIgnoreScissorLiftLimits = true;

  Menu();

  while(true)
  {
    getJoystickSettings(joystick);

    SetScissor();
  }
}

/////////////////////////////////////////////////////////////////////

void Menu()
{
  const int optionCount = 3;

  // Turn off normal screen diagnostics so can see and use the menu to select autonomous mission
  bDisplayDiagnostics = false;
  eraseDisplay();

  nxtDisplayCenteredBigTextLine(2, "Select");
  nxtDisplayCenteredBigTextLine(4, "Option");

  wait1Msec(750);

  // present menu and get the mission to run
  menuOption = MANUAL;
  while ( nNxtButtonPressed != kEnterButton )
  {
    if ( nNxtButtonPressed == kRightButton && time1[T1] > 500 && menuOption < optionCount)
    {
      menuOption++;
      time1[T1] = 0;
    }
    else if ( nNxtButtonPressed == kLeftButton && time1[T1] > 500 && menuOption > 1 )
    {
      menuOption--;
      time1[T1] = 0;
    }

    if ( menuOption == MANUAL )
    {
      nxtDisplayCenteredBigTextLine(2, "Manual?");
    }

    else if ( menuOption == LAB )
    {
      nxtDisplayCenteredBigTextLine(2, "Lab");
      nxtDisplayCenteredBigTextLine(4, "Bench?");
    }

    else if ( menuOption == FULL )
    {
      nxtDisplayCenteredBigTextLine(2, "Full");
      nxtDisplayCenteredBigTextLine(4, "Height?");
    }

    wait1Msec(25);
    eraseDisplay();
  }

  nxtDisplayCenteredBigTextLine(3, "CONFIRMED");

  wait1Msec(750);

  eraseDisplay();

  if ( menuOption == MANUAL)
    _nMaxLiftEncoderValue = SCISSOR_NO_LIMIT;
  else if (menuOption == LAB)
    _nMaxLiftEncoderValue = SCISSOR_LAB_BENCH_HEIGHT;
  else if ( menuOption == FULL )
    _nMaxLiftEncoderValue = SCISSOR_FULL_UP;
  else
    _nMaxLiftEncoderValue = SCISSOR_FULL_DOWN;
}

/////////////////////////////////////////////////////////////////////

int ButtonInput()
{
  int power = 0;

  // going down?
  if ( nNxtButtonPressed == kRightButton )
  {
    power = -100;
  }
  // going up?
  else if ( nNxtButtonPressed == kLeftButton )
  {
    power = 100;
  }
  // stopped?
  else if ( nNxtButtonPressed == kNoButton )
  {
    power = 0;
  }

  return power;
}

/////////////////////////////////////////////////////////////////////

void SetScissor()
{
#ifdef SCISSOR_TEST_BUTTON_INPUT
  int pwr = ButtonInput();
#else
  int pwr = ScaleJoystickInput(JOY_SCISSOR_CONTROL);
  if ( joy2TopHat_PRESSED == false )
    pwr = 0;
#endif

  MoveScissor(pwr);
}
