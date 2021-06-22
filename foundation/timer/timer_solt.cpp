// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)

#include "timer_solt.h"

namespace fdan {

TimerSolt::TimerSolt():
    _ms_index(0),
    _second_index(0),
    _minute_index(0) {

} 

void TimerSolt::SetInterval(TIME_UNIT tc, uint32_t interval, uint16_t current_time) {
    _interval = interval;

    switch (tc)
    {
    case fdan::TU_MILLISECOND:
        _ms_index = (uint16_t)(interval % TU_SECOND) + current_time;
        if (_ms_index >= TU_SECOND) {
            _ms_index -= TU_SECOND;
        }
        break;
    case fdan::TU_SECOND:
        interval /= TU_SECOND;
        _second_index = (uint8_t)(interval % 60) + current_time;
        if (_second_index >= 60) {
            _second_index -= 60;
        }
        break;
    case fdan::TU_MINUTE:
        interval /= TU_MINUTE;
        _minute_index = (uint8_t)(interval % 60) + current_time;
        if (_minute_index >= 60) {
            _minute_index -= 60;
        }
        break;
    default:
        break;
    }
}

uint32_t TimerSolt::GetInterval() {
    return _interval;
}

uint16_t TimerSolt::GetBitmapIndex(TIME_UNIT tu) {
    switch (tu)
    {
    case TU_MILLISECOND:
        return _ms_index & ~(TSF_IN_TIMER | TSF_ALWAYS);
    case TU_SECOND:
        return _second_index;
    case TU_MINUTE:
        return _minute_index;
    default:
        break;
    }
    return 0;
}

void TimerSolt::SetInTimer() {
    _ms_index |= TSF_IN_TIMER;
}

bool TimerSolt::IsInTimer() {
    return _ms_index & TSF_IN_TIMER;
}

void TimerSolt::RmInTimer() {
    _ms_index &= ~TSF_IN_TIMER;
}

void TimerSolt::SetAlways() {
    _ms_index |= TSF_ALWAYS;
}

bool TimerSolt::IsAlways() {
    return _ms_index & TSF_ALWAYS;
}

void TimerSolt::RmAlways() {
    _ms_index &= ~TSF_ALWAYS;
}

bool TimerSolt::ShouldAddSubTimer(TIME_UNIT tu) {
    switch (tu)
    {
    case TU_MILLISECOND:
        return false;
    case TU_SECOND:
        return _interval % TU_SECOND > 0;
    case TU_MINUTE:
        return _interval % TU_MINUTE > 0;
    default:
        break;
    }
    return false;
}

}