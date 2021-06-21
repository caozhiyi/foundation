// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)

#include "timer_solt.h"

namespace fdan {

TimerSolt::TimerSolt(): 
    _ms_interval(0),
    _second_interval(0),
    _minute_interval(0),
    _ms_index(0),
    _second_index(0),
    _minute_index(0) {

} 

void TimerSolt::SetInterval(TIME_UNIT tu, uint32_t interval, uint16_t current_time) {
    switch (tu)
    {
    case TU_MILLISECOND:
        _ms_interval = (uint16_t)(interval % TU_SECOND);
        _ms_index = _ms_interval + current_time;
        if (_ms_index >= TU_SECOND) {
            _ms_index -= TU_SECOND;
        }
        break;
    case TU_SECOND:
        _second_interval = (uint16_t)(interval % 60);
        _second_index = _second_interval + current_time;
        if (_second_index >= 60) {
            _ms_index -= 60;
        }
        break;
    case TU_MINUTE:
        _second_interval = (uint16_t)(interval % 60);
        _second_index = _second_interval + current_time;
        if (_second_index >= 60) {
            _ms_index -= 60;
        }
        break;
    default:
        break;
    }
}

uint16_t TimerSolt::GetInterval(TIME_UNIT tu) {
    switch (tu)
    {
    case TU_MILLISECOND:
        return _ms_interval & ~(TSF_IN_TIMER | TSF_ALWAYS);
    case TU_SECOND:
        return _second_interval;
    case TU_MINUTE:
        return _minute_interval;
    default:
        break;
    }
    return 0;
}

uint16_t TimerSolt::GetBitmapIndex(TIME_UNIT tu) {
    switch (tu)
    {
    case TU_MILLISECOND:
        return _ms_index;
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
    _ms_interval |= TSF_IN_TIMER;
}

bool TimerSolt::IsInTimer() {
    return _ms_interval & TSF_IN_TIMER;
}

void TimerSolt::RmInTimer() {
    _ms_interval &= ~TSF_IN_TIMER;
}

void TimerSolt::SetAlways() {
    _ms_interval |= TSF_ALWAYS;
}

bool TimerSolt::IsAlways() {
    return _ms_interval & TSF_ALWAYS;
}

void TimerSolt::RmAlways() {
    _ms_interval &= ~TSF_ALWAYS;
}

bool TimerSolt::ShouldAddTimer(TIME_UNIT tu) {
    switch (tu)
    {
    case TU_MILLISECOND:
        return _second_index == 0 && _minute_index == 0;
    case TU_SECOND:
        return _minute_index == 0;
    case TU_MINUTE:
        return true;
    default:
        break;
    }
    return false;
}

}