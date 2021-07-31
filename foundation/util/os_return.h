// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_UTIL_OS_RETURN_H_
#define FOUNDATION_UTIL_OS_RETURN_H_

#include <cstdint>

namespace fdan {

template <typename T>
struct SysCallResult {
  T return_value;
  int32_t err;
};

using SysCallInt32Result = SysCallResult<int32_t>;
using SysCallInt64Result = SysCallResult<int64_t>;

}  // namespace fdan

#endif  // FOUNDATION_UTIL_OS_RETURN_H_
