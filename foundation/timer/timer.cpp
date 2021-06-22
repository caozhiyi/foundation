
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
    auto sec_sub = std::make_shared<TimerContainer>(nullptr, TU_MILLISECOND, TU_SECOND);
    return std::unique_ptr<Timer>(new TimerContainer(sec_sub, TU_SECOND, TU_MINUTE));
}

std::unique_ptr<Timer> MakeTimer1Hour() {
    auto sec_sub = std::make_shared<TimerContainer>(nullptr, TU_MILLISECOND, TU_SECOND);
    auto min_sub = std::make_shared<TimerContainer>(sec_sub, TU_SECOND, TU_MINUTE);
    return std::unique_ptr<Timer>(new TimerContainer(min_sub, TU_MINUTE, TU_HOUR));
}

}