////////////////////////////////////////////////////////////////////////
//
// See TeamConfig.h for controller setup
//

////////////////////////////////////////////////////////////////////////
//
// TIMER USAGE
//
//   none
//

#include "JoystickDriver.c"
#include "GlobalDefines.h"

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
typedef enum
{
  CRATE_OPEN_DOWN,
  CRATE_OPEN_BACK,
  CRATE_OPEN_FRONT,
  CRATE_OPEN_UP,
  NUM_SCENARIOS

} TScenario;

string sScenarioName[NUM_SCENARIOS];

#define END_OF_SCENARIO -1

int tScenarioCrateOpenDown[] =
  {
    BTN_CRATE_LIFT_TO_GRAB,
    BTN_CRATE_GRIP_OPEN,
    BTN_CRATE_GRIP_HOLD,
    BTN_CRATE_LIFT_TO_ROTATE,
    BTN_CRATE_ROTATE_FWD,
    BTN_CRATE_ROTATE_FWD,
    BTN_BALL_DISPENSE,
    BTN_CRATE_LIFT_TO_CHIMNEY,
    BTN_CRATE_GRIP_OPEN,
    BTN_CRATE_LIFT_TO_GRAB,
    END_OF_SCENARIO
  };

int tScenarioCrateOpenBack[] =
  {
    BTN_CRATE_LIFT_TO_GRAB,
    BTN_CRATE_GRIP_OPEN,
    BTN_CRATE_GRIP_HOLD,
    BTN_CRATE_LIFT_TO_ROTATE,
    BTN_CRATE_ROTATE_FWD,
    BTN_BALL_DISPENSE,
    BTN_CRATE_LIFT_TO_CHIMNEY,
    BTN_CRATE_GRIP_OPEN,
    BTN_CRATE_LIFT_TO_GRAB,
    END_OF_SCENARIO
  };

int tScenarioCrateOpenFront[] =
  {
    BTN_CRATE_LIFT_TO_GRAB,
    BTN_CRATE_GRIP_OPEN,
    BTN_CRATE_GRIP_HOLD,
    BTN_CRATE_LIFT_TO_ROTATE,
    BTN_CRATE_ROTATE_REV,
    BTN_BALL_DISPENSE,
    BTN_CRATE_LIFT_TO_CHIMNEY,
    BTN_CRATE_GRIP_OPEN,
    BTN_CRATE_LIFT_TO_GRAB,
    END_OF_SCENARIO
  };

int tScenarioCrateOpenUp[] =
  {
    BTN_CRATE_LIFT_TO_GRAB,
    BTN_CRATE_GRIP_OPEN,
    BTN_CRATE_GRIP_HOLD,
    BTN_BALL_DISPENSE,
    BTN_CRATE_LIFT_TO_CHIMNEY,
    BTN_CRATE_GRIP_OPEN,
    BTN_CRATE_LIFT_TO_ROTATE,
    BTN_CRATE_GRIP_HOLD,
    BTN_CRATE_LIFT_TO_CHIMNEY,
    BTN_CRATE_LIFT_TO_GRAB,
    BTN_CRATE_GRIP_OPEN,
    END_OF_SCENARIO
  };
/*
int tScenarioCrateOpenUp[] =
  {
    BTN_CRATE_LIFT_TO_GRAB,
    BTN_CRATE_GRIP_OPEN,
    BTN_CRATE_GRIP_HOLD,
    BTN_CRATE_LIFT_TO_ROTATE,
    BTN_CRATE_ROTATE_FWD,
    BTN_CRATE_ROTATE_FWD,
    BTN_CRATE_GRIP_OPEN,
    BTN_CRATE_LIFT_TO_GRAB,
    BTN_CRATE_GRIP_HOLD,
    BTN_CRATE_LIFT_TO_ROTATE,
    BTN_CRATE_ROTATE_FWD,
    BTN_CRATE_ROTATE_FWD,
    BTN_BALL_DISPENSE,
    BTN_CRATE_LIFT_TO_CHIMNEY,
    BTN_CRATE_GRIP_OPEN,
    BTN_CRATE_LIFT_TO_GRAB,
    END_OF_SCENARIO
  };
*/
string sBtnDescription[11];

bool DoScenario(TScenario tScenario);
int WaitForJoystickButtonPress();
void InitDescriptions();
int GetExpectedButton(TScenario scenario, int currStep);
void DisplayScenarioResult(bool bShowResult);
void DisplayScenarioMsg(string sMsg);

task main()
{
  bDisplayDiagnostics = false;
  eraseDisplay();

  InitDescriptions();

  while (true)
  {
    TScenario tScenario = (TScenario)random[NUM_SCENARIOS-1];
    while ( DoScenario(tScenario) == false )
    { }

    wait1Msec(5);
  }
}

TScenario tCurrScenario;
int nCurrStep = 0;
int nCurrBtn = -1;
bool bStepCorrect = false;
string sCurrScenarioName = "";

bool DoScenario(TScenario tScenario)
{
  bool bSuccess = true;
  int numErrors = 0;

  tCurrScenario = tScenario;
  sCurrScenarioName = sScenarioName[tScenario];
  nCurrStep = 0;
  nCurrBtn = -1;

  DisplayScenarioResult(false);
  PlaySound(soundUpwardTones);

  while ( GetExpectedButton(tScenario, nCurrStep) != END_OF_SCENARIO )
  {
    nCurrBtn = WaitForJoystickButtonPress();
    DisplayScenarioResult(false);

    if ( nCurrBtn != GetExpectedButton(tScenario, nCurrStep) )
    {
      bSuccess = false;
      numErrors++;
      bStepCorrect = false;
	    DisplayScenarioResult(true);
		  PlaySound(soundException);
		  wait1Msec(750);
    }
    else
    {
      bStepCorrect = true;
	    DisplayScenarioResult(true);
	    PlaySound(soundBlip);
		  wait1Msec(750);
      nCurrStep++;
    }

  }

  PlaySound(soundDownwardTones);

  string s;
  sprintf(s, "DONE - %d errors", numErrors);
  DisplayScenarioMsg(s);
  wait1Msec(3000);
  eraseDisplay();

  return bSuccess;
}

int WaitForJoystickButtonPress()
{
  int btn = -1;
  while (btn == -1)
  {
	  getJoystickSettings(joystick);

	  if ( joy1Btn(BTN_CRATE_GRIP_OPEN) )
	    btn = BTN_CRATE_GRIP_OPEN;

	  else if ( joy1Btn(BTN_CRATE_GRIP_HOLD) )
	    btn = BTN_CRATE_GRIP_HOLD;

	  else if ( joy1Btn(BTN_CRATE_ROTATE_FWD) )
	    btn = BTN_CRATE_ROTATE_FWD;

	  else if ( joy1Btn(BTN_CRATE_ROTATE_REV) )
	    btn = BTN_CRATE_ROTATE_REV;

	  else if ( joy1Btn(BTN_CRATE_LIFT_RESET) )
	    btn = BTN_CRATE_LIFT_RESET;

	  else if ( joy1Btn(BTN_BALL_DISPENSE) )
	    btn = BTN_BALL_DISPENSE;
/*
	  else if ( joy1Btn(BTN_CRATE_LIFT_TO_BALL) )
	    btn = BTN_CRATE_LIFT_TO_BALL;
*/
	  else if ( joy1Btn(BTN_CRATE_LIFT_TO_GRAB) )
	    btn = BTN_CRATE_LIFT_TO_GRAB;

	  else if ( joy1Btn(BTN_CRATE_LIFT_TO_CHIMNEY) )
	    btn = BTN_CRATE_LIFT_TO_CHIMNEY;

	  else if ( joy1Btn(BTN_CRATE_LIFT_TO_ROTATE) )
	    btn = BTN_CRATE_LIFT_TO_ROTATE;
  }

  return btn;
}

void ClearDisplay()
{
  eraseDisplay();

  nxtDisplayTextLine(0, sCurrScenarioName);
  nxtDisplayTextLine(1, "-----------------");
  nxtDisplayTextLine(2, "");
  nxtDisplayTextLine(3, "");
  nxtDisplayTextLine(4, "");
  nxtDisplayTextLine(6, "");
  nxtDisplayTextLine(7, "");
}

void DisplayScenarioMsg(string sMsg)
{
  ClearDisplay();

    nxtDisplayTextLine(3, sMsg);
}

void DisplayScenarioResult(bool bShowResult)
{
  ClearDisplay();

  string s;

  if ( bShowResult )
  {
	  s = (bStepCorrect) ? "Correct" : "WRONG";
    nxtDisplayTextLine(2, "Step %d - %s", (nCurrStep + 1), s);

    s = ( nCurrBtn != END_OF_SCENARIO ) ?  sBtnDescription[nCurrBtn] : "";
	  nxtDisplayTextLine(4, "Hit: %s", s);

	  int expectedBtn = GetExpectedButton(tCurrScenario, nCurrStep);
	  s = sBtnDescription[expectedBtn];
	  nxtDisplayTextLine(6, "Expect: %s", s); 	}


  nxtDisplayTextLine(7, "");
}

void InitDescriptions()
{
  sScenarioName[CRATE_OPEN_DOWN]  = "Open Down";
  sScenarioName[CRATE_OPEN_BACK]  = "Open Back";
  sScenarioName[CRATE_OPEN_FRONT] = "Open Front";
  sScenarioName[CRATE_OPEN_UP]    = "Open Up";

  sBtnDescription[BTN_CRATE_LIFT_TO_GRAB]    = "To GRAB";
  //sBtnDescription[BTN_CRATE_LIFT_TO_BALL]    = "To BALL";
  sBtnDescription[BTN_CRATE_LIFT_TO_ROTATE]  = "To ROTATE";
  sBtnDescription[BTN_CRATE_LIFT_TO_CHIMNEY] = "To CHIMNEY";

  sBtnDescription[BTN_CRATE_GRIP_OPEN] = "Open";
  sBtnDescription[BTN_CRATE_GRIP_HOLD] = "Grab";

  sBtnDescription[BTN_CRATE_ROTATE_FWD] = "FWD";
  sBtnDescription[BTN_CRATE_ROTATE_REV] = "REV";

  sBtnDescription[BTN_BALL_DISPENSE] = "Release Ball";

}

int GetExpectedButton(TScenario scenario, int currStep)
{
  int btn = -1;

  if ( scenario == CRATE_OPEN_DOWN )
    btn = tScenarioCrateOpenDown[currStep];

 else if ( scenario == CRATE_OPEN_BACK )
    btn = tScenarioCrateOpenBack[currStep];

 else if ( scenario == CRATE_OPEN_FRONT )
    btn = tScenarioCrateOpenFront[currStep];

 else if ( scenario == CRATE_OPEN_UP )
    btn = tScenarioCrateOpenUp[currStep];

 return btn;
}
