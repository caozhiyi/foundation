// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_TIMER_TIMER_SOLT_H_
#define FOUNDATION_TIMER_TIMER_SOLT_H_

#include <cstdint>
#include "foundation/timer/timer_interface.h"

namespace fdan {

enum TIME_INDEX_TYPE: uint16_t {
  TIT_MILLISECOND = 0x01 << 10,
  TIT_SECOND      = 0x02 << 10,
  TIT_MINUTE      = 0x04 << 10,
  TIT_MUSK        = 0x07 << 10,
};

// Inherit this class to add to timer.
// don't call any function in this class,
// they internal used by timer.
class TimerSolt {
 public:
  TimerSolt();
  ~TimerSolt() {}

  enum TIMER_SOLT_FLAG: uint32_t {
    TSF_IN_TIMER = (uint32_t)1 << 30,
    TSF_ALWAYS   = (uint32_t)1 << 31,
  };

  // timer out call back
  virtual void OnTimer() = 0;

  void SetInterval(uint32_t interval);
  uint32_t GetTotalInterval();
  uint32_t GetLeftInterval();

  void ResetTime();
  uint32_t TimePass(uint32_t time);

  void SetInTimer();
  bool IsInTimer();
  void RmInTimer();

  void SetAlways();
  bool IsAlways();
  void RmAlways();

  void SetCurIndex(uint16_t index, uint16_t type);
  void GetCurIndex(uint16_t& index, uint16_t& type);

 private:
  friend class TimerContainer;
  uint32_t total_interval_;
  uint32_t left_interval_;

  uint16_t cur_index_;
};

}  // namespace fdan

#endif  // FOUNDATION_TIMER_TIMER_SOLT_H_
