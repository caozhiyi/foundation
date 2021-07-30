// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#include <algorithm>
#include "foundation/timer/timer_solt.h"
#include "foundation/timer/timer_container.h"

namespace fdan {

TimerContainer::TimerContainer(std::shared_ptr<TimerContainer> sub_timer,
  TIME_UNIT unit, TIME_UNIT max):
  time_unit_(unit),
  size_(max / unit),
  timer_max_(max),
  sub_timer_(sub_timer),
  cur_time_(0) {
  bitmap_.Init(size_);
}

TimerContainer::~TimerContainer() {}

bool TimerContainer::AddTimer(std::weak_ptr<TimerSolt> t,
  uint32_t time, bool always) {
  if (time >= timer_max_) {
    return false;
  }

  auto ptr = t.lock();
  if (!ptr) {
    return false;
  }

  if (time < time_unit_ && sub_timer_) {
    return sub_timer_->AddTimer(t, time, always);
  }

  // set current timer unit interval
  if (!ptr->IsInTimer()) {
    ptr->SetInterval(time);
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

  uint16_t cur_index = 0;
  uint16_t type = 0;
  ptr->GetCurIndex(cur_index, type);
  if (type == TimeUnit2TimeType(time_unit_)) {
    ptr->RmInTimer();
    bitmap_.Remove(cur_index);
    uint32_t left_time = ptr->GetLeftInterval();
    auto timer_map = timer_wheel_.find(cur_index);
    if (timer_map == timer_wheel_.end()) {
      return true;
    }
    // don't have sub timer
    if (!sub_timer_) {
      left_time = 0;
    }
    auto timer_list = timer_map->second.find(left_time);
    if (timer_list == timer_map->second.end()) {
      return false;
    }
    for (auto timer = timer_list->second.begin(); timer != timer_list->second.end(); ++timer) {
      auto target = timer->lock();
      if (target == ptr) {
        timer_list->second.erase(timer);
        return true;
      }
    }
    return true;
  }

  if (sub_timer_) {
    return sub_timer_->RmTimer(t);
  }
  return false;
}

int32_t TimerContainer::MinTime() {
  uint32_t past_time = 0;
  int32_t sub_time = 0;
  int32_t local_time = LocalMinTime();

  if (sub_timer_) {
    sub_time = sub_timer_->MinTime();
    past_time += sub_timer_->CurrentTimer();

    if (sub_time > 0) {
      local_time = local_time - past_time;
      if (local_time > 0) {
        return std::min(local_time, sub_time);
      }
      return sub_time;

    } else if (local_time > 0) {
      return local_time - past_time;
    }
  }

  return local_time;
}

int32_t TimerContainer::CurrentTimer() {
  uint32_t ret = cur_time_ * time_unit_;
  if (sub_timer_) {
    ret += sub_timer_->CurrentTimer();
  }
  return ret;
}

uint32_t TimerContainer::TimerRun(uint32_t time) {
  uint32_t time_pass = time / time_unit_;
  uint32_t left_time = time % time_unit_;
  uint32_t time_pass_with_unit = time_pass * time_unit_;
  bool do_timer = time_pass > 0;

  if (left_time > 0) {
    uint32_t sub_run_step = sub_timer_->TimerRun(left_time);
    if (sub_run_step > 0) {
      do_timer = true;
    }
    cur_time_ += sub_run_step;
  }

  if (!do_timer) {
    uint32_t run_setp = 0;
    if (cur_time_ >= size_) {
      cur_time_ -= size_;
      run_setp++;
    }
    return run_setp;
  }

  std::vector<std::weak_ptr<TimerSolt>> run_timer_solts;
  std::vector<std::weak_ptr<TimerSolt>> sub_timer_solts;

  uint32_t prev_time = cur_time_;
  cur_time_ += time_pass;
  while (1) {
    int32_t next_time = bitmap_.GetMinAfter(prev_time);
    if (next_time < 0) {
      break;
    }
    if (next_time > cur_time_) {
      break;
    }
    GetIndexTimer(run_timer_solts, sub_timer_solts, next_time, left_time);
    prev_time = next_time + 1;
  }

  uint32_t run_setp = 0;
  if (cur_time_ >= size_) {
    cur_time_ -= size_;
    run_setp++;
  }

  if (run_setp > 0) {
    prev_time = 0;
    while (1) {
      int32_t next_time = bitmap_.GetMinAfter(prev_time);
      if (next_time < 0) {
        break;
      }
      if (next_time > cur_time_) {
        break;
      }
      GetIndexTimer(run_timer_solts, sub_timer_solts, next_time, left_time);
      prev_time = next_time + 1;
    }
  }

  // timer call back
  DoTimer(run_timer_solts, sub_timer_solts);

  return run_setp;
}

bool TimerContainer::Empty() {
  return bitmap_.Empty();
}

void TimerContainer::Clear() {
  bitmap_.Clear();
  timer_wheel_.clear();
  if (sub_timer_) {
    sub_timer_->Clear();
  }
}

int32_t TimerContainer::LocalMinTime() {
  int32_t next_time = bitmap_.GetMinAfter(cur_time_);
  if (next_time >= 0) {
    return (next_time - cur_time_) * time_unit_
      + GetIndexLeftInterval(next_time);
  }

  if (cur_time_ > 0) {
    next_time = bitmap_.GetMinAfter(0);

    if (next_time >= 0) {
      return (next_time + size_ - cur_time_) * time_unit_
        + GetIndexLeftInterval(next_time);
    }
  }
  return NO_TIMER;
}

bool TimerContainer::InnerAddTimer(std::shared_ptr<TimerSolt> ptr,
  uint32_t time) {
  if (time > timer_max_) {
    return false;
  }

  uint16_t cur_index = time / time_unit_ + cur_time_;
  if (cur_index >= size_) {
    cur_index -= size_;
  }
  uint32_t left_time = time % time_unit_;
  // don't have sub timer
  if (!sub_timer_) {
    left_time = 0;
  }
  ptr->SetCurIndex(cur_index, TimeUnit2TimeType(time_unit_));
  ptr->TimePass(time - left_time);

  timer_wheel_[cur_index][left_time].push_back(ptr);
  return bitmap_.Insert(cur_index);
}

uint16_t TimerContainer::TimeUnit2TimeType(TIME_UNIT tu) {
  switch (tu) {
  case fdan::TU_MILLISECOND:
    return TIT_MILLISECOND;
  case fdan::TU_SECOND:
    return TIT_SECOND;
  case fdan::TU_MINUTE:
    return TIT_MINUTE;
  default:
    throw "invalid time unit";
  }
}

uint32_t TimerContainer::GetIndexLeftInterval(uint16_t index) {
  uint32_t left_interval = 0;
  auto timer_map = timer_wheel_.find(index);
  if (timer_map != timer_wheel_.end() && !timer_map->second.empty()) {
    left_interval = timer_map->second.begin()->first;
  }
  return left_interval;
}

void TimerContainer::GetIndexTimer(std::vector<std::weak_ptr<TimerSolt>>& run_timer_solts,
  std::vector<std::weak_ptr<TimerSolt>>& sub_timer_solts, uint32_t index, uint32_t time_pass) {
  auto bucket_iter = timer_wheel_.find(index);
  if (bucket_iter == timer_wheel_.end()) {
    return;
  }
  auto& bucket = bucket_iter->second;


  for (auto timer_list = bucket.begin(); timer_list != bucket.end(); timer_list++) {
    if (timer_list->first == 0) {
      run_timer_solts.insert(run_timer_solts.end(),
        timer_list->second.begin(), timer_list->second.end());
      continue;
    }

    if (timer_list->first <= time_pass) {
      run_timer_solts.insert(run_timer_solts.end(),
        timer_list->second.begin(), timer_list->second.end());
      continue;
    }

    for (auto timer = timer_list->second.begin(); timer != timer_list->second.end(); timer++) {
      auto target = timer->lock();
      sub_timer_solts.push_back(target);
    }
  }

  bitmap_.Remove(cur_time_);
  timer_wheel_.erase(bucket_iter);
}

void TimerContainer::DoTimer(std::vector<std::weak_ptr<TimerSolt>>& run_timer_solts,
  std::vector<std::weak_ptr<TimerSolt>>& sub_timer_solts) {
  // timer call back
  for (auto iter = run_timer_solts.begin(); iter != run_timer_solts.end(); iter++) {
    auto ptr = iter->lock();
    if (!ptr) {
      continue;
    }
    ptr->OnTimer();
    ptr->RmInTimer();

    // add timer again
    if (ptr->IsAlways()) {
      ptr->ResetTime();
      auto root_timer = root_timer_.lock();
      if (root_timer) {
        root_timer->AddTimer(ptr, ptr->GetTotalInterval(), ptr->IsAlways());

      } else {
        AddTimer(ptr, ptr->GetTotalInterval(), ptr->IsAlways());
      }
    }
  }

  // add sub timer
  if (!sub_timer_) {
    return;
  }
  for (auto iter = sub_timer_solts.begin(); iter != sub_timer_solts.end(); iter++) {
    auto ptr = iter->lock();
    if (!ptr) {
      continue;
    }
    sub_timer_->InnerAddTimer(ptr, ptr->GetLeftInterval());
  }
}

}  // namespace fdan
