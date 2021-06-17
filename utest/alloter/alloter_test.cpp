#include <mutex>
#include <thread>
#include <gtest/gtest.h>
#include "common/alloter/pool_alloter.h"

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
    quicx::AlloterWrap alloter(std::shared_ptr<quicx::Alloter>(new quicx::PoolAlloter()));
    AlloterTestClass* at = alloter.PoolNew<AlloterTestClass>(100);
    ASSERT_EQ(100, at->_data);
    alloter.PoolDelete<AlloterTestClass>(at);
    ASSERT_EQ(0, __alloter_test_value);
}


TEST(alloter_utest, warp2) {
    quicx::AlloterWrap alloter(std::shared_ptr<quicx::Alloter>(new quicx::PoolAlloter()));
    {
        auto at = alloter.PoolNewSharePtr<AlloterTestClass>(100);
        ASSERT_EQ(100, at->_data);
    }
    ASSERT_EQ(0, __alloter_test_value);
}

TEST(alloter_utest, warp3) {
    quicx::AlloterWrap alloter(std::shared_ptr<quicx::Alloter>(new quicx::PoolAlloter()));
    auto data = alloter.PoolMalloc<char>(100);
    alloter.PoolFree<char>(data, 100);
    ASSERT_EQ(nullptr, data);
}


TEST(alloter_utest, warp4) {
    quicx::AlloterWrap alloter(std::shared_ptr<quicx::Alloter>(new quicx::PoolAlloter()));
    {
        auto data = alloter.PoolMallocSharePtr<char>(100);
    }
}