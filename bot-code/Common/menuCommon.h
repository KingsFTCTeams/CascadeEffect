#ifndef MENU_COMMON_H
#define MENU_COMMON_H

#pragma systemFile

typedef enum
{
  NOT_DEFINED = 0,

  DRIVE,
  END,
  WAIT,
  PT_TURN,
  SWING,
  ARMS,
  LIFT,
  BALL,

  LAST = BALL
} commandType;

typedef union
{
  int   iArg;
  float fArg;
} commandArgument;

typedef struct
{
  commandType     currentCommand;
  commandArgument currentArgument;
} command;

#endif // MENU_COMMON_H
