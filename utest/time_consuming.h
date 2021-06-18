#ifndef FOUNDATION_UTEST_TIME_CONSUMING
#define FOUNDATION_UTEST_TIME_CONSUMING

#include <chrono>
#include <string>

namespace fdan {

class TimeConsuming {
public:
    TimeConsuming(std::string name);
    ~TimeConsuming();

private:
    std::string _name;
    std::chrono::system_clock::time_point _start_time;
};

}

#endif