
// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)

#include "timer.h"
#include "timer_combin_container.h"
#include "timer_integer_container.h"

namespace fdan {

std::unique_ptr<Timer> MakeIntegerTimer1Sec() {
    return std::unique_ptr<Timer>(new TimerIntegerContainer(nullptr, TU_MILLISECOND, TU_SECOND));
}

std::unique_ptr<Timer> MakeIntegerTimer1Min() {
    auto sec_sub = std::make_shared<TimerIntegerContainer>(nullptr, TU_MILLISECOND, TU_SECOND);
    return std::unique_ptr<Timer>(new TimerIntegerContainer(sec_sub, TU_SECOND, TU_MINUTE));
}

std::unique_ptr<Timer> MakeIntegerTimer1Hour() {
    auto sec_sub = std::make_shared<TimerIntegerContainer>(nullptr, TU_MILLISECOND, TU_SECOND);
    auto min_sub = std::make_shared<TimerIntegerContainer>(sec_sub, TU_SECOND, TU_MINUTE);
    return std::unique_ptr<Timer>(new TimerIntegerContainer(min_sub, TU_MINUTE, TU_HOUR));
}


std::shared_ptr<Timer> MakeCombinTimer1Sec() {
    return std::make_shared<TimerCombinContainer>(nullptr, TU_MILLISECOND, TU_SECOND);
}

std::shared_ptr<Timer> MakeCombinTimer1Min() {
    auto sec_sub = std::make_shared<TimerCombinContainer>(nullptr, TU_MILLISECOND, TU_SECOND);
    auto timer = std::make_shared<TimerCombinContainer>(sec_sub, TU_SECOND, TU_MINUTE);
    sec_sub->SetRootTimer(timer);
    return timer;
}

std::shared_ptr<Timer> MakeCombinTimer1Hour() {
    auto sec_sub = std::make_shared<TimerCombinContainer>(nullptr, TU_MILLISECOND, TU_SECOND);
    auto min_sub = std::make_shared<TimerCombinContainer>(sec_sub, TU_SECOND, TU_MINUTE);
    auto timer = std::make_shared<TimerCombinContainer>(min_sub, TU_MINUTE, TU_HOUR);
    sec_sub->SetRootTimer(timer);
    min_sub->SetRootTimer(timer);

    return timer;
}

}