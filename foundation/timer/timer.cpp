
// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)

#include "timer.h"
#include "timer_container.h"

namespace fdan {

std::unique_ptr<Timer> MakeTimer1Sec() {
    return std::unique_ptr<Timer>(new TimerContainer(nullptr, TU_MILLISECOND, TU_SECOND));
}

std::unique_ptr<Timer> MakeTimer1Min() {
    auto sub = MakeTimer1Sec();
    return std::unique_ptr<Timer>(new TimerContainer(std::move(sub), TU_SECOND, TU_MINUTE));
}

std::unique_ptr<Timer> MakeTimer1Hour() {
    auto sub = MakeTimer1Min();
    return std::unique_ptr<Timer>(new TimerContainer(std::move(sub), TU_MINUTE, TU_HOUR));
}

}