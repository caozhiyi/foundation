#include <iostream>
#include <gtest/gtest.h>
#include "foundation/network/address.h"

TEST(address_utest, asstrng) {
    fdan::Address addr(fdan::AT_IPV4, "127.0.0.1", 8080);
    EXPECT_EQ(addr.AsString(), "127.0.0.1:8080");
}

TEST(address_utest, compare) {
    fdan::Address addr1(fdan::AT_IPV4, "127.0.0.1", 8080);
    fdan::Address addr2(fdan::AT_IPV4, "127.0.0.1", 8080);
    fdan::Address addr3(fdan::AT_IPV4, "127.0.0.2", 8080);
    EXPECT_TRUE(addr1 == addr2);
    EXPECT_FALSE(addr1 == addr3);
}

TEST(address_utest, iostream) {
    fdan::Address addr(fdan::AT_IPV4, "127.0.0.1", 8080);
    std::cout << addr << std::endl;
}