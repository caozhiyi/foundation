// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)

#include <algorithm>
#include "timer_solt.h"
#include "timer_container.h"

namespace fdan {

TimerContainer::TimerContainer(std::shared_ptr<TimerContainer> sub_timer, TIME_UNIT unit, TIME_UNIT max):
    _time_unit(unit),
    _size(max / unit),
    _timer_max(max),
    _sub_timer(sub_timer),
    _cur_time(0) {

    _bitmap.Init(_size);
}

TimerContainer::~TimerContainer() {

}

bool TimerContainer::AddTimer(std::weak_ptr<TimerSolt> t, uint32_t time, bool always) {
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

bool TimerContainer::RmTimer(std::weak_ptr<TimerSolt> t) {
    auto ptr = t.lock();
    if (!ptr) {
        return false;
    }
    ptr->RmInTimer();
    return true;
}

int32_t TimerContainer::MinTime() {
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

int32_t TimerContainer::CurrentTimer() {
    uint32_t ret = _cur_time * _time_unit;
    if (_sub_timer) {
        ret += _sub_timer->CurrentTimer();
    }
    return ret;
}

uint32_t TimerContainer::TimerRun(uint32_t time) {
    uint32_t time_pass = time / _time_unit;
    uint32_t time_pass_unit = time_pass * _time_unit;
    _cur_time += time_pass;

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
    }

    _bitmap.Remove(_cur_time);
    _timer_wheel.erase(bucket_iter);

    for (auto iter = run_timer_solts.begin(); iter != run_timer_solts.end(); iter++) {
        auto ptr = *iter;

        uint32_t left_interval = ptr->TimePass(time_pass_unit);
        if (left_interval > 0) {
            InnerAddTimer(ptr, left_interval);

        } else {
            ptr->OnTimer();
            // add timer again
            if (ptr->IsAlways()) {
                auto root_timer = _root_timer.lock();
                if (root_timer) {
                    root_timer->AddTimer(ptr, ptr->GetTotalInterval(), ptr->IsAlways());
                } else {
                    AddTimer(ptr, ptr->GetTotalInterval(), ptr->IsAlways());
                }
            }
        }
    }
    return run_setp;
}

bool TimerContainer::Empty() {
    return _bitmap.Empty();
}

int32_t TimerContainer::LocalMinTime() {
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

bool TimerContainer::InnerAddTimer(std::shared_ptr<TimerSolt> ptr, uint32_t time) {
    if (time > _timer_max) {
        return false;
    }
    
    time /= _time_unit; 
    // get current timer index
    uint32_t index = time + _cur_time;

    _timer_wheel[index].push_back(ptr);
    return _bitmap.Insert(index);
}


}