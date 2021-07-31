// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#include <random>
#include "foundation/util/random.h"

namespace fdan {

std::random_device RangeRandom::random_;
std::mt19937 RangeRandom::engine_(_random());

RangeRandom::RangeRandom(int32_t min, int32_t max):
    uniform_(min, max) {}

RangeRandom::~RangeRandom() {}

int32_t RangeRandom::Random() {
    return uniform_(engine_);
}

}  // namespace fdan
