#include <gtest/gtest.h>

#include "common.h"
#include "common/timer/timer.h"

TEST(timercontainer_utest, addtimer1) {
    auto solt = std::make_shared<TimerSoltIns>();
    auto timer = quicx::MakeTimer1Hour();

    EXPECT_TRUE(timer->AddTimer(solt, 30 * quicx::MILLISECOND));
}

TEST(timercontainer_utest, addtimer2) {
    auto solt1 = std::make_shared<TimerSoltIns>();
    auto solt2 = std::make_shared<TimerSoltIns>();
    auto solt3 = std::make_shared<TimerSoltIns>();
    auto solt4 = std::make_shared<TimerSoltIns>();
    auto timer = quicx::MakeTimer1Hour();

    EXPECT_TRUE(timer->AddTimer(solt1, 10 * quicx::MILLISECOND, true));
    EXPECT_TRUE(timer->AddTimer(solt2, 30 * quicx::SECOND));
    EXPECT_TRUE(timer->AddTimer(solt3, 40 * quicx::MINUTE));
    EXPECT_TRUE(timer->AddTimer(solt4, 50 * quicx::MINUTE));
}

TEST(timercontainer_utest, rmtimer) {
    auto solt1 = std::make_shared<TimerSoltIns>();
    auto solt2 = std::make_shared<TimerSoltIns>();
    auto solt3 = std::make_shared<TimerSoltIns>();
    auto solt4 = std::make_shared<TimerSoltIns>();
    auto timer = quicx::MakeTimer1Hour();

    EXPECT_TRUE(timer->AddTimer(solt1, 10 * quicx::MILLISECOND, true));
    EXPECT_TRUE(timer->AddTimer(solt2, 30 * quicx::SECOND));
    EXPECT_TRUE(timer->AddTimer(solt3, 40 * quicx::MINUTE));
    EXPECT_FALSE(timer->AddTimer(solt4, 60 * quicx::MINUTE));

    EXPECT_TRUE(timer->RmTimer(solt1));
    EXPECT_TRUE(timer->RmTimer(solt2));
    EXPECT_TRUE(timer->RmTimer(solt3));
    EXPECT_TRUE(timer->RmTimer(solt4));
}

TEST(timercontainer_utest, mintime) {
    auto solt1 = std::make_shared<TimerSoltIns>();
    auto solt2 = std::make_shared<TimerSoltIns>();
    auto solt3 = std::make_shared<TimerSoltIns>();
    auto solt4 = std::make_shared<TimerSoltIns>();
    auto timer = quicx::MakeTimer1Hour();

    EXPECT_TRUE(timer->AddTimer(solt1, 10 * quicx::MILLISECOND));
    EXPECT_TRUE(timer->AddTimer(solt2, 30 * quicx::SECOND));
    EXPECT_TRUE(timer->AddTimer(solt3, 40 * quicx::MINUTE));

    EXPECT_EQ(10, timer->MinTime());

    timer->RmTimer(solt1);
    EXPECT_EQ(30 * quicx::SECOND, timer->MinTime());

    timer->RmTimer(solt2);
    EXPECT_EQ(40 * quicx::MINUTE, timer->MinTime());
}

TEST(timercontainer_utest, timerrun1) {
    auto solt1 = std::make_shared<TimerSoltIns>();
    auto solt2 = std::make_shared<TimerSoltIns>();
    auto solt3 = std::make_shared<TimerSoltIns>();
    auto solt4 = std::make_shared<TimerSoltIns>();
    auto timer = quicx::MakeTimer1Hour();

    EXPECT_TRUE(timer->AddTimer(solt1, 20 * quicx::MILLISECOND));
    EXPECT_TRUE(timer->AddTimer(solt2, 30 * quicx::SECOND));
    EXPECT_TRUE(timer->AddTimer(solt3, 40 * quicx::MINUTE, true));

    EXPECT_EQ(20, timer->MinTime());

    timer->TimerRun(20);
    EXPECT_EQ(29980, timer->MinTime());

    timer->TimerRun(10);
    EXPECT_EQ(29970, timer->MinTime());

    timer->TimerRun(29970);
    EXPECT_EQ(2370000, timer->MinTime());

    timer->TimerRun(2370000);
    EXPECT_EQ(2400000, timer->MinTime());

    for (uint32_t i = 0; i < 20; i++) {
        timer->TimerRun(2400000);
        EXPECT_EQ(2400000, timer->MinTime());
    }
}

TEST(timercontainer_utest, timerrun2) {
    auto solt1 = std::make_shared<TimerSoltIns>();
    auto solt2 = std::make_shared<TimerSoltIns>();
    auto solt3 = std::make_shared<TimerSoltIns>();
    auto solt4 = std::make_shared<TimerSoltIns>();
    auto timer = quicx::MakeTimer1Hour();

    EXPECT_TRUE(timer->AddTimer(solt1, 20 * quicx::MILLISECOND));
    EXPECT_TRUE(timer->AddTimer(solt2, 31 * quicx::SECOND, true));
    EXPECT_TRUE(timer->AddTimer(solt3, 1 * quicx::MINUTE));

    EXPECT_EQ(20, timer->MinTime());

    timer->TimerRun(20);
    EXPECT_EQ(30980, timer->MinTime());

    timer->TimerRun(10);
    EXPECT_EQ(30970, timer->MinTime());

    timer->TimerRun(30970);
    EXPECT_EQ(29000, timer->MinTime());

    timer->TimerRun(29000);
    EXPECT_EQ(2000, timer->MinTime());

    timer->TimerRun(2000);
    EXPECT_EQ(31000, timer->MinTime());

    for (uint32_t i = 0; i < 20; i++) {
        timer->TimerRun(31000);
        EXPECT_EQ(31000, timer->MinTime());
    }
}

TEST(timercontainer_utest, timerrun3) {
    auto solt1 = std::make_shared<TimerSoltIns>();
    auto solt2 = std::make_shared<TimerSoltIns>();
    auto solt3 = std::make_shared<TimerSoltIns>();
    auto solt4 = std::make_shared<TimerSoltIns>();
    auto timer = quicx::MakeTimer1Hour();

    EXPECT_TRUE(timer->AddTimer(solt1, 20 * quicx::MILLISECOND));
    EXPECT_TRUE(timer->AddTimer(solt2, 31 * quicx::SECOND));

    EXPECT_EQ(20, timer->MinTime());

    timer->TimerRun(20);
    EXPECT_EQ(30980, timer->MinTime());

    timer->TimerRun(10);
    EXPECT_EQ(30970, timer->MinTime());

    EXPECT_TRUE(timer->AddTimer(solt1, 40 * quicx::MILLISECOND));
    EXPECT_EQ(40, timer->MinTime());

    timer->TimerRun(40);
    EXPECT_EQ(30930, timer->MinTime());

    timer->TimerRun(30930);
    EXPECT_EQ(-1, timer->MinTime());
}