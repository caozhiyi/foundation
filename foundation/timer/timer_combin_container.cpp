// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)

#include <algorithm>
#include "timer_solt.h"
#include "timer_combin_container.h"

namespace fdan {

TimerCombinContainer::TimerCombinContainer(std::shared_ptr<TimerCombinContainer> sub_timer, TIME_UNIT unit, TIME_UNIT size):
    TimerIntegerContainer(sub_timer, unit, size),
    _root_timer(nullptr) {

}

TimerCombinContainer::~TimerCombinContainer() {

}

uint32_t TimerCombinContainer::TimerRun(uint32_t time) {
    _cur_time += time / _time_unit;
    time = time % _time_unit;

    if (time > 0) {
        uint32_t sub_run_step = _sub_timer->TimerRun(time);
        _cur_time += sub_run_step;
    }

    uint32_t run_setp = 0;
    if (_cur_time >= _size) {
        _cur_time -= _size;
        run_setp++;
    }

    auto bucket_iter = _timer_wheel.find(_cur_time);
    if (bucket_iter == _timer_wheel.end()) {
        return run_setp;
    }
    auto& bucket = bucket_iter->second;
    std::vector<std::shared_ptr<TimerSolt>> run_timer_solts;

    for (auto iter = bucket.begin(); iter != bucket.end(); iter++) {
        auto ptr = (iter)->lock();
        if (ptr && ptr->IsInTimer()) {
            run_timer_solts.push_back(ptr);
        }
        // remove from current bucket
        _bitmap.Remove(ptr->GetBitmapIndex(_time_unit));
    }

    _timer_wheel.erase(bucket_iter);

    for (auto iter = run_timer_solts.begin(); iter != run_timer_solts.end(); iter++) {
        auto ptr = *iter;
        if (ptr->ShouldAddSubTimer(_time_unit)) {
            if (!SubTimerTryRun(ptr)) {
                _sub_timer->InnerAddTimer(ptr, ptr->GetInterval());
            }

        } else {
            ptr->OnTimer();
            if (ptr->IsAlways()) {
                if (_root_timer) {
                    _root_timer->AddTimer(ptr, ptr->GetInterval(), ptr->IsAlways());

                } else {
                    AddTimer(ptr, ptr->GetInterval(), ptr->IsAlways());
                }
            }
        }
    }
    return run_setp;
}

int32_t TimerCombinContainer::LocalMinTime() {
    int32_t next_time = _bitmap.GetMinAfter(_cur_time);
    if (next_time >= 0) {
        if (next_time <= _cur_time) {
            return (next_time - _cur_time + _size) * _time_unit;

        } else {
            return (next_time - _cur_time) * _time_unit;
        }
    }

    if (_cur_time > 0) {
        next_time = _bitmap.GetMinAfter(0);
        if (next_time >= 0) {
            return (next_time + _size - _cur_time) * _time_unit;
        }
    }
    return NO_TIMER;
}

bool TimerCombinContainer::InnerAddTimer(std::shared_ptr<TimerSolt> ptr, uint32_t time) {
   
    ptr->SetInterval(_time_unit, time, _cur_time);
    // get current timer index
    uint16_t index = ptr->GetBitmapIndex(_time_unit);

    // should add to sub timer
    if (index == _cur_time && _sub_timer) {
        return _sub_timer->InnerAddTimer(ptr, time);
    }

    _timer_wheel[index].push_back(ptr);
    return _bitmap.Insert(index);
}

bool TimerCombinContainer::SubTimerTryRun(std::shared_ptr<TimerSolt> ptr) {
    // need timeout now
    if (_sub_timer) {
        if (_sub_timer->SubTimerTryRun(ptr)) {
            return true;
        }
    }
    if (!Empty() && ptr->GetBitmapIndex(_time_unit) == _cur_time) {
        ptr->OnTimer();
        return true;
    }
    return false;
}

}