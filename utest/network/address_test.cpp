#include <iostream>
#include <gtest/gtest.h>
#include "common/network/address.h"

TEST(address_utest, asstrng) {
    quicx::Address addr(quicx::AT_IPV4, "127.0.0.1", 8080);
    EXPECT_EQ(addr.AsString(), "127.0.0.1:8080");
}

TEST(address_utest, compare) {
    quicx::Address addr1(quicx::AT_IPV4, "127.0.0.1", 8080);
    quicx::Address addr2(quicx::AT_IPV4, "127.0.0.1", 8080);
    quicx::Address addr3(quicx::AT_IPV4, "127.0.0.2", 8080);
    EXPECT_TRUE(addr1 == addr2);
    EXPECT_FALSE(addr1 == addr3);
}

TEST(address_utest, iostream) {
    quicx::Address addr(quicx::AT_IPV4, "127.0.0.1", 8080);
    std::cout << addr << std::endl;
}