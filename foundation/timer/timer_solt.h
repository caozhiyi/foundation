// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)

#ifndef COMMON_TIMER_TIMER_SOLT
#define COMMON_TIMER_TIMER_SOLT

#include <cstdint>
#include "timer_interface.h"

namespace fdan {

// Inherit this class to add to timer.
// don't call any function in this class, 
// they internal used by timer.
class TimerSolt {
public:
    TimerSolt();
    ~TimerSolt() {}

private:
    enum TIMER_SOLT_FLAG {
        TSF_IN_TIMER = 1 << 14,
        TSF_ALWAYS   = 1 << 15,
    };

    // timer out call back
    virtual void OnTimer() = 0;

    void SetInterval(TIME_UNIT tc, uint32_t interval, uint16_t current_time);
    uint16_t GetInterval(TIME_UNIT tc);
    uint16_t GetBitmapIndex(TIME_UNIT tc);

    void SetInTimer();
    bool IsInTimer();
    void RmInTimer();

    void SetAlways();
    bool IsAlways();
    void RmAlways();

    bool ShouldAddTimer(TIME_UNIT tc);

private:
    friend class TimerContainer;
    friend class Timer1ms;

    uint16_t _ms_interval;
    uint8_t  _second_interval;
    uint8_t  _minute_interval;

    uint16_t _ms_index;
    uint8_t  _second_index;
    uint8_t  _minute_index;
};

}

#endif