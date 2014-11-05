MILLI msLastStallCheck = 0;

#define CheckForMotorStall(msNow)                                                                                                                   \
    {                                                                                                                                               \
    BOOL fFirstTime = (0 == msLastStallCheck);                                                                                                      \
    /* We have a throttle on how often we check for stall due to a worry about reading the motor encoders too frequently */                         \
    const MILLI msCheckInterval = 100;                                                                                                              \
    if (fFirstTime || msNow > msLastStallCheck + msCheckInterval)                                                                                   \
        {                                                                                                                                           \
        /* Find out where the motors are right now */                                                                                               \
        ENCOD encLeft, encRight; BOOL fResult = true;                                                                                               \
        ReadEncoders(fResult, encLeft, encRight);                                                                                                   \
        if (fResult)                                                                                                                                \
            {                                                                                                                                       \
            /* A motor is ok if it has no power or if it's moved (or a couple of other things) */                                                   \
            BOOL fLeftOK  = fFirstTime || !MtrOf(motorLeft).fStallCheck  || MtrOf(motorLeft).power  == 0 || motorLeft.encStallPrev  != encLeft;     \
            BOOL fRightOK = fFirstTime || !MtrOf(motorRight).fStallCheck || MtrOf(motorRight).power == 0 || motorRight.encStallPrev != encRight;    \
            const int msAbortStall = 750;                                                                                           \
            /**/                                                                                                                    \
            if (fLeftOK)                                                                                                            \
                {                                                                                                                   \
                motorLeft.msLastOkStall = msNow;                                                                                    \
                }                                                                                                                   \
            else if (msNow - motorLeft.msLastOkStall > msAbortStall)                                                                \
                {                                                                                                                   \
                AbortDueToStall("left", msNow - motorLeft.msLastOkStall);                                                           \
                }                                                                                                                   \
            /**/                                                                                                                    \
            if (fRightOK)                                                                                                           \
                {                                                                                                                   \
                motorRight.msLastOkStall = msNow;                                                                                   \
                }                                                                                                                   \
            else if (msNow - motorRight.msLastOkStall > msAbortStall)                                                               \
                {                                                                                                                   \
                AbortDueToStall("right", msNow - motorRight.msLastOkStall);                                                         \
                }                                                                                                                   \
            /**/                                                                                                                    \
            motorLeft.encStallPrev  = encLeft;                                                                                      \
            motorRight.encStallPrev = encRight;                                                                                     \
            msLastStallCheck = msNow;                                                                                               \
            }                                                                                                                       \
        }                                                                                                                           \
    }
