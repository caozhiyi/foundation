#include <gtest/gtest.h>

#include "common.h"
#include "common/timer/timer.h"

TEST(timer1ms_utest, addtimer1) {
    auto solt = std::make_shared<TimerSoltIns>();
    auto timer = quicx::MakeTimer50Ms();

    EXPECT_TRUE(timer->AddTimer(solt, 30 * quicx::MILLISECOND));
}

TEST(timer1ms_utest, addtimer2) {
    auto solt1 = std::make_shared<TimerSoltIns>();
    auto solt2 = std::make_shared<TimerSoltIns>();
    auto solt3 = std::make_shared<TimerSoltIns>();
    auto solt4 = std::make_shared<TimerSoltIns>();
    auto timer = quicx::MakeTimer50Ms();

    EXPECT_TRUE(timer->AddTimer(solt1, 10 * quicx::MILLISECOND, true));
    EXPECT_TRUE(timer->AddTimer(solt2, 30 * quicx::MILLISECOND));
    EXPECT_TRUE(timer->AddTimer(solt3, 40 * quicx::MILLISECOND));
    EXPECT_FALSE(timer->AddTimer(solt4, 50 * quicx::MILLISECOND));
}

TEST(timer1ms_utest, rmtimer) {
    auto solt1 = std::make_shared<TimerSoltIns>();
    auto solt2 = std::make_shared<TimerSoltIns>();
    auto solt3 = std::make_shared<TimerSoltIns>();
    auto solt4 = std::make_shared<TimerSoltIns>();
    auto timer = quicx::MakeTimer50Ms();

    EXPECT_TRUE(timer->AddTimer(solt1, 10 * quicx::MILLISECOND, true));
    EXPECT_TRUE(timer->AddTimer(solt2, 30 * quicx::MILLISECOND));
    EXPECT_TRUE(timer->AddTimer(solt3, 40 * quicx::MILLISECOND));
    EXPECT_FALSE(timer->AddTimer(solt4, 50 * quicx::MILLISECOND));
    
    EXPECT_TRUE(timer->RmTimer(solt1));
    EXPECT_TRUE(timer->RmTimer(solt2));
    EXPECT_TRUE(timer->RmTimer(solt3));
    EXPECT_TRUE(timer->RmTimer(solt4));
}

TEST(timer1ms_utest, mintime) {
    auto solt1 = std::make_shared<TimerSoltIns>();
    auto solt2 = std::make_shared<TimerSoltIns>();
    auto solt3 = std::make_shared<TimerSoltIns>();
    auto solt4 = std::make_shared<TimerSoltIns>();
    auto timer = quicx::MakeTimer50Ms();

    EXPECT_TRUE(timer->AddTimer(solt1, 10 * quicx::MILLISECOND, true));
    EXPECT_TRUE(timer->AddTimer(solt2, 30 * quicx::MILLISECOND));
    EXPECT_TRUE(timer->AddTimer(solt3, 40 * quicx::MILLISECOND));
    EXPECT_FALSE(timer->AddTimer(solt4, 50 * quicx::MILLISECOND));

    EXPECT_EQ(10, timer->MinTime());

    timer->RmTimer(solt1);
    EXPECT_EQ(30, timer->MinTime());

    timer->RmTimer(solt2);
    EXPECT_EQ(40, timer->MinTime());
}

TEST(timer1ms_utest, timerrun) {
    auto solt1 = std::make_shared<TimerSoltIns>();
    auto solt2 = std::make_shared<TimerSoltIns>();
    auto solt3 = std::make_shared<TimerSoltIns>();
    auto solt4 = std::make_shared<TimerSoltIns>();
    auto timer = quicx::MakeTimer50Ms();

    EXPECT_TRUE(timer->AddTimer(solt1, 20 * quicx::MILLISECOND, true));
    EXPECT_TRUE(timer->AddTimer(solt2, 30 * quicx::MILLISECOND));
    EXPECT_TRUE(timer->AddTimer(solt3, 36 * quicx::MILLISECOND));

    EXPECT_EQ(20, timer->MinTime());

    timer->TimerRun(20);
    EXPECT_EQ(10, timer->MinTime());

    timer->TimerRun(10);
    EXPECT_EQ(6, timer->MinTime());

    timer->TimerRun(6);
    EXPECT_EQ(4, timer->MinTime());

    timer->TimerRun(4);
    EXPECT_EQ(20, timer->MinTime());

    for (uint32_t i = 0; i < 20; i++) {
        timer->TimerRun(20);
        EXPECT_EQ(20, timer->MinTime());
    }
}