// Backend for the menu system.

#ifndef MENU_H
#define MENU_H

#pragma systemFile

#include "menuCommon.h"
#include "commandHandlers.h"
#include "menuGUI.h"

#ifndef PROGRAM_SIZE
#define PROGRAM_SIZE  256
#endif

#define MS_TIME_BETWEEN_COMMANDS    100

command currentProgram[PROGRAM_SIZE];

void initProgram(command &toParse, long len)
{
  memset(toParse, NOT_DEFINED, sizeof(command)*len);
}

bool executeCommand(command &toExecute)
{
  switch(toExecute.currentCommand)
  {
    case DRIVE:
      driveHandler(toExecute);
    break;
    case END:
      endHandler(toExecute);
    break;
    case WAIT:
      waitHandler(toExecute);
    break;
    case PT_TURN:
      turnHandler(toExecute);
    break;
    case SWING:
      swingHandler(toExecute);
    break;
    case ARMS:
      armsHandler(toExecute);
    break;
    case LIFT:
      liftHandler(toExecute);
    break;
    case BALL:
      ballHandler(toExecute);
    break;
    default:
      return false;
  }

  return true;
}

bool parse(long len)
{
  long i = 0;

  while(currentProgram[i].currentCommand != END || i < len)
  {
    if(currentProgram[i].currentCommand == NOT_DEFINED || !executeCommand(currentProgram[i]))
      return false;

    i++;
    wait1Msec(MS_TIME_BETWEEN_COMMANDS);
  }

  return true;
}

bool initMenuAndParse()
{
  initProgram(currentProgram, PROGRAM_SIZE);
  initGUI(currentProgram);

  return true;
}

#endif // MENU_H
