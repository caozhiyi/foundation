// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_TIMER_TIMER_H_
#define FOUNDATION_TIMER_TIMER_H_

#include <memory>
#include "foundation/timer/timer_interface.h"

namespace fdan {

std::shared_ptr<Timer> MakeTimer1Sec();

std::shared_ptr<Timer> MakeTimer1Min();

std::shared_ptr<Timer> MakeTimer1Hour();

}  // namespace fdan

#endif  // FOUNDATION_TIMER_TIMER_H_
