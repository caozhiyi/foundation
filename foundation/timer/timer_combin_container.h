// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)

#ifndef COMMON_TIMER_TIMER_CONTAINER
#define COMMON_TIMER_TIMER_CONTAINER

#include <list>
#include <memory>
#include <unordered_map>

#include "timer_integer_container.h"

namespace fdan {

// combination timer
// can set any time. as 20ms, 452ms, 50sec, 5minute, 1500 ms(1sec + 500ms) ...

// timer wheel container, include a sub timer.
// if set timeout time is little than accuracy, will be added to sub timer wheel.
// It inherits from the timer interface, 
// so can set another timer wheel container to sub timer to support more timer set.
// More timer define see timer.h file.
class TimerCombinContainer: 
    public TimerIntegerContainer {

public:
    TimerCombinContainer(std::shared_ptr<TimerCombinContainer> sub_timer, TIME_UNIT unit, TIME_UNIT size);
    virtual ~TimerCombinContainer();
    // timer wheel run time 
    // return carry
    virtual uint32_t TimerRun(uint32_t step);

    void SetRootTimer(std::shared_ptr<TimerCombinContainer> root_timer) { _root_timer = root_timer; }
    
protected:
    // get current timer wheel timeout time
    int32_t LocalMinTime();
    bool InnerAddTimer(std::shared_ptr<TimerSolt> ptr, uint32_t time);
    bool SubTimerTryRun(std::shared_ptr<TimerSolt> ptr);
    uint32_t GetSubFutureTimer(uint32_t next_index);

private:
    std::shared_ptr<TimerCombinContainer> _root_timer;
};

}

#endif