// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_TIMER_TIMER_INTERFACE_H_
#define FOUNDATION_TIMER_TIMER_INTERFACE_H_

#include <memory>
#include <cstdint>

namespace fdan {

// time unit
enum TIME_UNIT: uint32_t {
  TU_MILLISECOND = 1,
  TU_SECOND    = TU_MILLISECOND * 1000,
  TU_MINUTE    = TU_SECOND * 60,
  TU_HOUR    = TU_MINUTE * 60,
};

enum TIMER_CODE {
  NO_TIMER = -1  // don't have timer
};

class TimerSolt;

// timer interface, timer inherits from this.
class Timer {
 public:
  Timer() {}
  ~Timer() {}

  virtual bool AddTimer(std::weak_ptr<TimerSolt> t,
    uint32_t time, bool always = false) = 0;
  virtual bool RmTimer(std::weak_ptr<TimerSolt> t) = 0;

  // get min next time out time
  // return:
  // >= 0  : the next time
  //  < 0  : has no timer
  virtual int32_t MinTime() = 0;

  // return the timer wheel current time
  virtual int32_t CurrentTimer() = 0;

  // timer wheel run time
  // return carry
  virtual uint32_t TimerRun(uint32_t time) = 0;

  virtual bool Empty() = 0;
};

}  // namespace fdan

#endif  // FOUNDATION_TIMER_TIMER_INTERFACE_H_
