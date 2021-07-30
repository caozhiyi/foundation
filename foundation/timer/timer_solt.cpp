// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#include "foundation/timer/timer_solt.h"

namespace fdan {

TimerSolt::TimerSolt():
  total_interval_(0),
  left_interval_(0) {}

void TimerSolt::SetInterval(uint32_t interval) {
  total_interval_ = interval;
  left_interval_ = interval;
}

uint32_t TimerSolt::GetTotalInterval() {
  total_interval_ &= ~(TSF_ALWAYS | TSF_IN_TIMER);
  return total_interval_;
}

uint32_t TimerSolt::GetLeftInterval() {
  return left_interval_;
}

void TimerSolt::ResetTime() {
  left_interval_ = total_interval_;
  cur_index_ = 0;
}

uint32_t TimerSolt::TimePass(uint32_t time) {
  left_interval_ -= time;
  return left_interval_;
}

void TimerSolt::SetInTimer() {
  total_interval_ |= TSF_IN_TIMER;
}

bool TimerSolt::IsInTimer() {
  return total_interval_ & TSF_IN_TIMER;
}

void TimerSolt::RmInTimer() {
  total_interval_ &= ~TSF_IN_TIMER;
}

void TimerSolt::SetAlways() {
  total_interval_ |= TSF_ALWAYS;
}

bool TimerSolt::IsAlways() {
  return total_interval_ & TSF_ALWAYS;
}

void TimerSolt::RmAlways() {
  total_interval_ &= ~TSF_ALWAYS;
}

void TimerSolt::SetCurIndex(uint16_t index, uint16_t type) {
  cur_index_ = index | type;
}

void TimerSolt::GetCurIndex(uint16_t& index, uint16_t& type) {
  index = cur_index_ & ~TIT_MUSK;
  type = cur_index_ & TIT_MUSK;
}

}  // namespace fdan
