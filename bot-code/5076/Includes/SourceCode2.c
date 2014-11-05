typedef enum {
  DRIVE,
  TURN_RIGHT,
  TURN_LEFT,
  WAIT,
  SWING_LEFT,
  SWING_RIGHT,
} commandType;

typedef struct {
  commandType command;
  float argument;
} commandStruct;

task main() {
  commandStruct currentProgram[255];
  ubyte currentInstruction = 0;

  while(true) {

  }
}
