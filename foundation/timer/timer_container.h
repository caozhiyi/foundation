// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_TIMER_TIMER_CONTAINER_H_
#define FOUNDATION_TIMER_TIMER_CONTAINER_H_

#include <map>
#include <list>
#include <memory>
#include <vector>
#include <unordered_map>

#include "foundation/util/bitmap.h"
#include "foundation/timer/timer_interface.h"

namespace fdan {

// timer wheel container, include a sub timer.
// if set timeout time is little than accuracy, will be added to sub timer wheel.
// It inherits from the timer interface, 
// so can set another timer wheel container to sub timer to support more timer set.
// More timer define see timer.h file.
class TimerContainer:
  public Timer {
 public:
  TimerContainer(std::shared_ptr<TimerContainer> sub_timer,
    TIME_UNIT unit, TIME_UNIT max);
  ~TimerContainer();

  bool AddTimer(std::weak_ptr<TimerSolt> t, uint32_t time, bool always = false);
  bool RmTimer(std::weak_ptr<TimerSolt> t);

  // get min next time out time
  // return
  // >= 0 : the next time
  //  < 0 : has no timer
  int32_t MinTime();
  // return the timer wheel current time
  int32_t CurrentTimer();
  // timer wheel run time
  // return carry
  uint32_t TimerRun(uint32_t step);

  bool Empty();
  void Clear();

  // get current timer wheel timeout time
  int32_t LocalMinTime();
  bool InnerAddTimer(std::shared_ptr<TimerSolt> ptr, uint32_t time);

  void SetRootTimer(std::shared_ptr<TimerContainer> timer)
    { root_timer_ = timer; }

 protected:
  uint16_t TimeUnit2TimeType(TIME_UNIT tu);
  uint32_t GetIndexLeftInterval(uint16_t index);
  void GetIndexTimer(std::vector<std::weak_ptr<TimerSolt>>& run_timer_solts,
    std::vector<std::weak_ptr<TimerSolt>>& sub_timer_solts,
    uint32_t index, uint32_t time_pass);
  void DoTimer(std::vector<std::weak_ptr<TimerSolt>>& run_timer_solts,
    std::vector<std::weak_ptr<TimerSolt>>& sub_timer_solts);

 protected:
  TIME_UNIT time_unit_;
  uint32_t  size_;
  uint32_t  timer_max_;

  uint32_t cur_time_;
  Bitmap   bitmap_;
  std::weak_ptr<TimerContainer>   root_timer_;
  std::shared_ptr<TimerContainer> sub_timer_;
  std::unordered_map<uint32_t, std::map<uint32_t, std::list<std::weak_ptr<TimerSolt>>>> timer_wheel_;
};

}  // namespace fdan

#endif  // FOUNDATION_TIMER_TIMER_CONTAINER_H_
