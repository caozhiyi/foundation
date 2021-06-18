#include <gtest/gtest.h>

#include "common.h"
#include "foundation/timer/timer_interface.h"

TEST(timersolt_utest, index1) {
    TimerSoltIns t;
    t.SetIndex(25 * fdan::MILLISECOND);

    EXPECT_EQ(25, t.GetIndex(fdan::TC_1MS));
}

TEST(timersolt_utest, index2) {
    TimerSoltIns t;
    t.SetIndex(180 * fdan::MILLISECOND);

    EXPECT_EQ(30, t.GetIndex(fdan::TC_1MS));
    EXPECT_EQ(3, t.GetIndex(fdan::TC_50MS));
}

TEST(timersolt_utest, index3) {
    TimerSoltIns t;
    t.SetIndex(6 * fdan::SECOND + 180 * fdan::MILLISECOND);

    EXPECT_EQ(30, t.GetIndex(fdan::TC_1MS));
    EXPECT_EQ(3, t.GetIndex(fdan::TC_50MS));
    EXPECT_EQ(6, t.GetIndex(fdan::TC_1SEC));
}

TEST(timersolt_utest, index4) {
    TimerSoltIns t;
    t.SetIndex(10 * fdan::MINUTE + 6 * fdan::SECOND + 180 * fdan::MILLISECOND);

    EXPECT_EQ(30, t.GetIndex(fdan::TC_1MS));
    EXPECT_EQ(3, t.GetIndex(fdan::TC_50MS));
    EXPECT_EQ(6, t.GetIndex(fdan::TC_1SEC));
    EXPECT_EQ(10, t.GetIndex(fdan::TC_1MIN));
}

TEST(timersolt_utest, always1) {
    TimerSoltIns t;
    t.SetIndex(10 * fdan::MINUTE + 6 * fdan::SECOND + 180 * fdan::MILLISECOND);
    t.SetAlways(fdan::TC_1MS);

    EXPECT_EQ(30, t.GetIndex(fdan::TC_1MS));
    EXPECT_EQ(3, t.GetIndex(fdan::TC_50MS));
    EXPECT_EQ(6, t.GetIndex(fdan::TC_1SEC));
    EXPECT_EQ(10, t.GetIndex(fdan::TC_1MIN));

    EXPECT_TRUE(t.IsAlways(fdan::TC_1MS));
    EXPECT_FALSE(t.IsAlways(fdan::TC_1SEC));
}

TEST(timersolt_utest, always2) {
    TimerSoltIns t;
    t.SetIndex(10 * fdan::MINUTE + 6 * fdan::SECOND + 180 * fdan::MILLISECOND);
    t.SetAlways(fdan::TC_1MIN);

    EXPECT_EQ(30, t.GetIndex(fdan::TC_1MS));
    EXPECT_EQ(3, t.GetIndex(fdan::TC_50MS));
    EXPECT_EQ(6, t.GetIndex(fdan::TC_1SEC));
    EXPECT_EQ(10, t.GetIndex(fdan::TC_1MIN));

    EXPECT_TRUE(t.IsAlways(fdan::TC_1MIN));
    EXPECT_FALSE(t.IsAlways(fdan::TC_1SEC));

    t.CancelAlways(fdan::TC_1MIN);
    EXPECT_FALSE(t.IsAlways(fdan::TC_1MIN));
}

TEST(timersolt_utest, settimer) {
    TimerSoltIns t;
    t.SetIndex(10 * fdan::MINUTE + 6 * fdan::SECOND + 180 * fdan::MILLISECOND);
    t.SetAlways(fdan::TC_1MIN);
    t.SetTimer();

    EXPECT_EQ(30, t.GetIndex(fdan::TC_1MS));
    EXPECT_EQ(3, t.GetIndex(fdan::TC_50MS));
    EXPECT_EQ(6, t.GetIndex(fdan::TC_1SEC));
    EXPECT_EQ(10, t.GetIndex(fdan::TC_1MIN));

    EXPECT_TRUE(t.IsInTimer());

    t.RmTimer();
    EXPECT_FALSE(t.IsInTimer());
}

TEST(timersolt_utest, clear) {
    TimerSoltIns t;
    t.SetIndex(10 * fdan::MINUTE + 6 * fdan::SECOND + 180 * fdan::MILLISECOND);
    t.SetAlways(fdan::TC_1MIN);
    t.SetTimer();
    t.Clear();

    EXPECT_EQ(0, t.GetIndex(fdan::TC_1MS));
    EXPECT_EQ(0, t.GetIndex(fdan::TC_50MS));
    EXPECT_EQ(0, t.GetIndex(fdan::TC_1SEC));
    EXPECT_EQ(0, t.GetIndex(fdan::TC_1MIN));
    EXPECT_FALSE(t.IsAlways(fdan::TC_1MIN));
    EXPECT_FALSE(t.IsInTimer());
}