#include <gtest/gtest.h>
#include "foundation/util/bitmap.h"

TEST(bitmap_utest, init) {
    fdan::Bitmap bm;
    EXPECT_TRUE(bm.Init(64));
    EXPECT_TRUE(bm.Init(100));
}

TEST(bitmap_utest, insert) {
    fdan::Bitmap bm;

    EXPECT_TRUE(bm.Init(100));

    EXPECT_TRUE(bm.Insert(0));
    EXPECT_TRUE(bm.Insert(10));
    EXPECT_TRUE(bm.Insert(65));
    EXPECT_FALSE(bm.Insert(200));
}

TEST(bitmap_utest, remove) {
    fdan::Bitmap bm;

    EXPECT_TRUE(bm.Init(100));

    EXPECT_TRUE(bm.Insert(50));
    EXPECT_TRUE(bm.Insert(100));

    EXPECT_TRUE(bm.Remove(100));
    EXPECT_TRUE(bm.Remove(20));
}

TEST(bitmap_utest, minafter) {
    fdan::Bitmap bm;
    EXPECT_TRUE(bm.Init(1000));

    EXPECT_TRUE(bm.Insert(20));
    EXPECT_TRUE(bm.Insert(400));
    EXPECT_TRUE(bm.Insert(60));
    EXPECT_TRUE(bm.Insert(800));

    EXPECT_EQ(20, bm.GetMinAfter(8));
    EXPECT_TRUE(bm.Remove(20));
    EXPECT_EQ(60, bm.GetMinAfter(8));
    EXPECT_EQ(400, bm.GetMinAfter(61));
    EXPECT_TRUE(bm.Remove(400));
    EXPECT_EQ(800, bm.GetMinAfter(61));
}

TEST(bitmap_utest, empty) {
    fdan::Bitmap bm;
    EXPECT_TRUE(bm.Init(1000));

    EXPECT_TRUE(bm.Insert(20));
    EXPECT_TRUE(bm.Insert(400));
    EXPECT_TRUE(bm.Insert(60));
    EXPECT_TRUE(bm.Insert(800));

    EXPECT_EQ(20, bm.GetMinAfter(8));
    EXPECT_TRUE(bm.Remove(20));
    EXPECT_EQ(60, bm.GetMinAfter(8));

    EXPECT_EQ(400, bm.GetMinAfter(61));
    EXPECT_TRUE(bm.Remove(400));
    EXPECT_EQ(800, bm.GetMinAfter(61));

    EXPECT_TRUE(bm.Remove(800));
    EXPECT_TRUE(bm.Remove(60));
    EXPECT_TRUE(bm.Empty());
}