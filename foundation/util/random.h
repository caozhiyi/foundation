// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_UTIL_RANDOM_H_
#define FOUNDATION_UTIL_RANDOM_H_

#include <random>
#include <cstdint>

namespace fdan {

class RangeRandom {
 public:
  RangeRandom(int32_t min, int32_t max);
  ~RangeRandom();

  int32_t Random();

 private:
  static std::random_device random_;
  static std::mt19937       engine_;
  std::uniform_int_distribution<int32_t> uniform_;
};

}  // namespace fdan

#endif  // FOUNDATION_UTIL_RANDOM_H_
