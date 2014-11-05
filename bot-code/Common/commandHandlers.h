// Add handlers to command here.

#pragma systemFile

#ifndef COMMAND_HANDLERS_H
#define COMMAND_HANDLERS_H

void driveHandler(command &currentCommand)
{
  DriveXSquaresWithPwr(currentCommand.currentArgument.fArg, currentCommand.currentArgument.iArg);
}

void endHandler(command &currentCommand)
{
  SetAbortAutonomous(true);
}

void waitHandler(command &currentCommand)
{
  wait1Msec(currentCommand.currentArgument.iArg);
}

void turnHandler(command &currentCommand)
{
  Turn(currentCommand.currentArgument.iArg);
}

void swingHandler(command &currentCommand)
{

}

void armsHandler(command &currentCommand)
{

}

void liftHandler(command &currentCommand)
{

}

void ballHandler(command &currentCommand)
{

}

#endif // COMMAND_HANDLERS_H
