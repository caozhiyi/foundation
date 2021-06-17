#include <chrono>
#include <gtest/gtest.h>
#include "../time_consuming.h"
#include "common/util/os_info.h"

TEST(os_info_utest, IsBigEndian_time) {
    quicx::TimeConsuming tc("IsBigEndian_time");
    for (uint32_t i = 0; i < 1000000; i++) {
        quicx::IsBigEndian();
    }
}

TEST(os_info_utest, IsBigEndian) {
    bool is = quicx::IsBigEndian();
}
