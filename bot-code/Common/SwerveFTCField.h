//
// FTCField.h
//
// Definitions related to the FTC playing field. Be sure to #include this file *before*
// you #include "joystickDriver.h" as it overrides some defaults that are found therein.
//
// Current features here include a user interface executed during waitForStart() that allows
// the team color and side of the field to be manually changed.

typedef enum
    {
    TEAM_COLOR_BLUE,
    TEAM_COLOR_RED,
    } TEAM_COLOR;

typedef enum
    {
    STARTING_SIDE_LEFT,
    STARTING_SIDE_RIGHT
    } STARTING_SIDE;

//-------------------------------------------------------------------------------------
// Figure out if the team color is being forced, and, if so, to which color.
//-------------------------------------------------------------------------------------

#ifndef DEFAULT_TEAM_COLOR
#define DEFAULT_TEAM_COLOR  TEAM_COLOR_BLUE
#endif

STARTING_SIDE startingSide = STARTING_SIDE_LEFT;
TEAM_COLOR    teamColor    = DEFAULT_TEAM_COLOR;

// Define here so this file is self-contained. However, a better solution
// is to actually play the music; see Music.h.
#ifndef PlayMusic
#define PlayMusic(music,tempo,beat)
#endif

#define PlayTeamColorFeedback()                                             \
    {                                                                       \
    if (TEAM_COLOR_BLUE==teamColor)                                         \
        PlayMusic(musicBluesRiff, 100, 3*NOTE_EIGHTH);   /* in 6/8 time! */ \
    else                                                                    \
        PlayMusic(musicRedRedWine, 180, NOTE_QUARTER);                      \
    }

//--------------------------------------------------------------------------------------
// Team selection and field side selection
//--------------------------------------------------------------------------------------

// Alter some of the default behavior found in joystickDriver.h
#define OrangeButtonWaitDoFeedback()        false
#define OrangeButtonWaitTimeThreshold()     0x7FFFFFFF

// Define here so this file is self-contained. However, a better solution
// is to actually put the message on the display (:-); see display.h.
#ifndef DisplayMessageTemporarily
#define DisplayMessageTemporarily(duration, message)
#endif

// We can press the left or right arrow to indicate the side of the field from
// which we are starting. Once the side is determined, contining to
// press the same direction of arrow will toggle the team color.

#define ToggleTeamColor()                                   \
    {                                                       \
    if (TEAM_COLOR_BLUE==teamColor)                         \
        {                                                   \
        teamColor = TEAM_COLOR_RED;                         \
        DisplayMessageTemporarily(750, "red team");         \
        }                                                   \
    else                                                    \
        {                                                   \
        teamColor = TEAM_COLOR_BLUE;                        \
        DisplayMessageTemporarily(750, "blue team");        \
        }                                                   \
    PlayTeamColorFeedback();                                \
    }

#define WaitNoArrowPressed()    { while (kNoButton != nNxtButtonPressed) {} }

#define WaitForStartLoop()                                  \
    {                                                       \
    if (kLeftButton == nNxtButtonPressed)                   \
        {                                                   \
        if (STARTING_SIDE_LEFT==startingSide)               \
            {                                               \
            ToggleTeamColor();                              \
            }                                               \
        else                                                \
            {                                               \
            startingSide = STARTING_SIDE_LEFT;              \
            PlayMusicNoWait(musicHappy, 400, NOTE_QUARTER); \
            DisplayMessageTemporarily(750, "left side");    \
            }                                               \
        WaitNoArrowPressed();                               \
        }                                                   \
    else if (kRightButton == nNxtButtonPressed)             \
        {                                                   \
        if (STARTING_SIDE_RIGHT==startingSide)              \
            {                                               \
            ToggleTeamColor();                              \
            }                                               \
        else                                                \
            {                                               \
            startingSide = STARTING_SIDE_RIGHT;             \
            PlayMusicNoWait(musicSad, 400, NOTE_QUARTER);   \
            DisplayMessageTemporarily(750, "right side");   \
            }                                               \
        WaitNoArrowPressed();                               \
        }                                                   \
    }
