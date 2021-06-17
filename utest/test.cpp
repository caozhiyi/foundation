#include <gtest/gtest.h>

#ifndef __linux__
#pragma comment(lib,"gtest_maind.lib")
#pragma comment(lib,"gtestd.lib")
#endif // __linux__

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}