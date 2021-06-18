#ifndef UTEST_COMMON_TIMER_COMMON
#define UTEST_COMMON_TIMER_COMMON

#include "foundation/timer/timer_solt.h"

class TimerSoltIns: public fdan::TimerSolt {
public:
    void OnTimer() {}
};

#endif