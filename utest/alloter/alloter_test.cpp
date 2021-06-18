#include <mutex>
#include <thread>
#include <gtest/gtest.h>
#include "foundation/alloter/pool_alloter.h"

uint32_t __alloter_test_value = 0;
class AlloterTestClass {
public:
    AlloterTestClass(uint64_t v) : _data(v) {
        __alloter_test_value++;
    }
    ~AlloterTestClass() {
         __alloter_test_value--;
    }

    uint64_t _data;
};

TEST(alloter_utest, warp1) {
    fdan::AlloterWrap alloter(std::shared_ptr<fdan::Alloter>(new fdan::PoolAlloter()));
    AlloterTestClass* at = alloter.PoolNew<AlloterTestClass>(100);
    ASSERT_EQ(100, at->_data);
    alloter.PoolDelete<AlloterTestClass>(at);
    ASSERT_EQ(0, __alloter_test_value);
}


TEST(alloter_utest, warp2) {
    fdan::AlloterWrap alloter(std::shared_ptr<fdan::Alloter>(new fdan::PoolAlloter()));
    {
        auto at = alloter.PoolNewSharePtr<AlloterTestClass>(100);
        ASSERT_EQ(100, at->_data);
    }
    ASSERT_EQ(0, __alloter_test_value);
}

TEST(alloter_utest, warp3) {
    fdan::AlloterWrap alloter(std::shared_ptr<fdan::Alloter>(new fdan::PoolAlloter()));
    auto data = alloter.PoolMalloc<char>(100);
    alloter.PoolFree<char>(data, 100);
    ASSERT_EQ(nullptr, data);
}


TEST(alloter_utest, warp4) {
    fdan::AlloterWrap alloter(std::shared_ptr<fdan::Alloter>(new fdan::PoolAlloter()));
    {
        auto data = alloter.PoolMallocSharePtr<char>(100);
    }
}