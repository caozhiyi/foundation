// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)

#ifndef COMMON_TIMER_INTEGER_TIMER_CONTAINER
#define COMMON_TIMER_INTEGER_TIMER_CONTAINER

#include <list>
#include <memory>
#include <unordered_map>

#include "../util/bitmap.h"
#include "timer_interface.h"

namespace fdan {

// integer timer. 
// only can set integer time, as 20ms, 452ms, 50sec, 5minute ...
// can't set 1500 ms(1sec + 500ms) ...

// timer wheel container, include a sub timer.
// if set timeout time is little than accuracy, will be added to sub timer wheel.
// It inherits from the timer interface, 
// so can set another timer wheel container to sub timer to support more timer set.
// More timer define see timer.h file.
class TimerIntegerContainer: 
    public Timer {

public:
    TimerIntegerContainer(std::shared_ptr<TimerIntegerContainer> sub_timer, TIME_UNIT unit, TIME_UNIT size);
    virtual ~TimerIntegerContainer();

    virtual bool AddTimer(std::weak_ptr<TimerSolt> t, uint32_t time, bool always = false);
    virtual bool RmTimer(std::weak_ptr<TimerSolt> t);

    // get min next time out time
    // return
    // >= 0 : the next time
    //  < 0 : has no timer
    virtual int32_t MinTime();
    // return the timer wheel current time
    virtual int32_t CurrentTimer();
    // timer wheel run time 
    // return carry
    virtual uint32_t TimerRun(uint32_t step);

    virtual bool Empty();

    // get current timer wheel timeout time
    virtual int32_t LocalMinTime();
    virtual bool InnerAddTimer(std::shared_ptr<TimerSolt> ptr, uint32_t time);
    virtual bool SubTimerTryRun(std::shared_ptr<TimerSolt> ptr) { return true; }

protected:
    TIME_UNIT _time_unit;
    uint32_t  _size;
    uint32_t  _timer_max;

    uint32_t _cur_time;
    Bitmap   _bitmap;
    std::shared_ptr<TimerIntegerContainer> _sub_timer;
    std::unordered_map<uint32_t, std::list<std::weak_ptr<TimerSolt>>> _timer_wheel;
};

}

#endif