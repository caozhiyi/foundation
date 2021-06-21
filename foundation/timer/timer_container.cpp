// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)

#include <algorithm>
#include "timer_solt.h"
#include "timer_container.h"

namespace fdan {

TimerContainer::TimerContainer(std::unique_ptr<Timer> t, TIME_UNIT unit, TIME_UNIT size):
    _time_unit(unit),
    _size(size / unit),
    _timer_max(size),
    _sub_timer(std::move(t)),
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
        return _sub_timer->AddTimer(t, time % _time_unit, always);
    }

    // set current timer unit interval
    ptr->SetInterval(_time_unit, time / _time_unit, _cur_time);
    if (!ptr->IsInTimer()) {
        ptr->SetInTimer();
        if (always) {
            ptr->SetAlways();
        }
    }

    // get current timer index
    uint16_t index = ptr->GetBitmapIndex(_time_unit);

    // should add to sub timer
    if (index == _cur_time && _sub_timer) {
        return _sub_timer->AddTimer(t, time % _time_unit, always);
    }

    _timer_wheel[index].push_back(t);
    return _bitmap.Insert(index);
}

bool TimerContainer::RmTimer(std::weak_ptr<TimerSolt> t) {
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
        // timer call back
        (*iter)->OnTimer();

        if ((*iter)->IsAlways() && (*iter)->ShouldAddTimer(_time_unit)) {
            // add to timer again
            (*iter)->SetInterval(_time_unit, (*iter)->GetInterval(_time_unit), _cur_time);
            uint16_t bit_index = (*iter)->GetBitmapIndex(_time_unit);
            _timer_wheel[bit_index].push_back((*iter));
            _bitmap.Insert(bit_index);
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
        return (next_time - _cur_time) * _time_unit;
    }

    if (_cur_time > 0) {
        next_time = _bitmap.GetMinAfter(0);
        if (next_time >= 0) {
            return (next_time + _size - _cur_time) * _time_unit;
        }
    }

    return NO_TIMER;
}

}