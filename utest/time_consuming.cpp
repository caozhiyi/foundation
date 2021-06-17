#include <iostream>
#include "time_consuming.h"

namespace quicx {

TimeConsuming::TimeConsuming(std::string name) : _name(name),
    _start_time(std::chrono::system_clock::now()) {
}

TimeConsuming::~TimeConsuming() {
    std::chrono::milliseconds time_span = std::chrono::duration_cast<std::chrono::milliseconds>  \
        (std::chrono::system_clock::now() - _start_time);

    std::cout << _name << " used " << time_span.count() << " ms." << std::endl;
}

}