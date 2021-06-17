#ifndef UTEST_COMMON_TIMER_COMMON
#define UTEST_COMMON_TIMER_COMMON

#include "common/timer/timer_solt.h"

class TimerSoltIns: public quicx::TimerSolt {
public:
    void OnTimer() {}
};

#endif