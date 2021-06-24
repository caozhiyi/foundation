// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)

#include <algorithm>
#include "timer_solt.h"
#include "timer_integer_container.h"

namespace fdan {

TimerIntegerContainer::TimerIntegerContainer(std::shared_ptr<TimerIntegerContainer> sub_timer, TIME_UNIT unit, TIME_UNIT size):
    _time_unit(unit),
    _size(size / unit),
    _timer_max(size),
    _sub_timer(sub_timer),
    _cur_time(0) {

    _bitmap.Init(_size);
}

TimerIntegerContainer::~TimerIntegerContainer() {

}

bool TimerIntegerContainer::AddTimer(std::weak_ptr<TimerSolt> t, uint32_t time, bool always) {
    if (time >= _timer_max) {
        return false;
    }

    auto ptr = t.lock();
    if (!ptr) {
        return false;
    }

    if (time < _time_unit && _sub_timer) {
        return _sub_timer->AddTimer(t, time, always);
    }

    // set current timer unit interval
    if (!ptr->IsInTimer()) {
        ptr->SetInTimer();
        if (always) {
            ptr->SetAlways();
        }

    } else {
        return false;
    }

    return InnerAddTimer(ptr, time);
}

bool TimerIntegerContainer::RmTimer(std::weak_ptr<TimerSolt> t) {
    auto ptr = t.lock();
    if (!ptr) {
        return false;
    }
    ptr->RmInTimer();
    _bitmap.Remove(ptr->GetBitmapIndex(_time_unit));
    if (_sub_timer) {
        _sub_timer->RmTimer(t);
    }
    return true;
}

int32_t TimerIntegerContainer::MinTime() {
    uint32_t past_time = 0;
    int32_t sub_time = 0;
    int32_t local_time = LocalMinTime();

    if (_sub_timer) {
        sub_time = _sub_timer->MinTime();
        past_time += _sub_timer->CurrentTimer();

        if (sub_time > 0) {
            local_time = local_time - past_time;
            if (local_time > 0) {
                return std::min(local_time, sub_time);
            }
            return sub_time;

        } else if (local_time > 0){
            return local_time - past_time;
        }
    }

    return local_time;
}

int32_t TimerIntegerContainer::CurrentTimer() {
    uint32_t ret = _cur_time * _time_unit;
    if (_sub_timer) {
        ret += _sub_timer->CurrentTimer();
    }
    return ret;
}

uint32_t TimerIntegerContainer::TimerRun(uint32_t time) {
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
        
        ptr->OnTimer();
        if (ptr->IsAlways()) {
            InnerAddTimer(ptr, ptr->GetInterval());
        }
    }
    return run_setp;
}

bool TimerIntegerContainer::Empty() {
    return _bitmap.Empty();
}

int32_t TimerIntegerContainer::LocalMinTime() {
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

bool TimerIntegerContainer::InnerAddTimer(std::shared_ptr<TimerSolt> ptr, uint32_t time) {
   
    ptr->SetInterval(_time_unit, time, _cur_time);
    // get current timer index
    uint16_t index = ptr->GetBitmapIndex(_time_unit);

    _timer_wheel[index].push_back(ptr);
    return _bitmap.Insert(index);
}


}