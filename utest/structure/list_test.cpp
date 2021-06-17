#include <iostream>
#include <gtest/gtest.h>
#include "common/structure/list.h"
#include "common/structure/list_solt.h"

static uint32_t __test_shared_count = 0;

class TestListItem:
    public quicx::ListSolt<TestListItem> {
public:
    TestListItem(){
        __test_shared_count++;
    }
    ~TestListItem() {
        __test_shared_count--;
    }

};

TEST(list_utest, add1) {
    quicx::List<TestListItem> list;
    {
        auto item1 = std::make_shared<TestListItem>();
        list.PushFront(item1);
        EXPECT_EQ(__test_shared_count, 1);

        auto item2 = std::make_shared<TestListItem>();
        list.PushFront(item2);
        EXPECT_EQ(__test_shared_count, 2);
    }

    list.PopFront();
    EXPECT_EQ(__test_shared_count, 1);

    list.PopFront();
    EXPECT_EQ(__test_shared_count, 0);
}

TEST(list_utest, add2) {
    quicx::List<TestListItem> list;
    {
        auto item1 = std::make_shared<TestListItem>();
        list.PushFront(item1);
        EXPECT_EQ(__test_shared_count, 1);

        auto item2 = std::make_shared<TestListItem>();
        list.PushFront(item2);
        EXPECT_EQ(__test_shared_count, 2);
    }

    list.Clear();
    EXPECT_EQ(__test_shared_count, 0);
}

TEST(list_utest, add3) {
    quicx::List<TestListItem> list;
    {
        auto item1 = std::make_shared<TestListItem>();
        list.PushFront(item1);
        EXPECT_EQ(__test_shared_count, 1);

        auto item2 = std::make_shared<TestListItem>();
        list.PushFront(item2);
        EXPECT_EQ(__test_shared_count, 2);

        auto item3 = std::make_shared<TestListItem>();
        list.PushBack(item3);
        EXPECT_EQ(__test_shared_count, 3);
    }

    list.PopFront();
    EXPECT_EQ(__test_shared_count, 2);

    list.PopBack();
    EXPECT_EQ(__test_shared_count, 1);

    list.PopBack();
    EXPECT_EQ(__test_shared_count, 0);
}