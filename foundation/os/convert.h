// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_OS_CONVERT_H_
#define FOUNDATION_OS_CONVERT_H_

#include <cstdint>

namespace fdan {

void Localtime(const uint64_t* time, void* out_tm);

char* ErrnoInfo(uint32_t err);

}  // namespace fdan

#endif  // FOUNDATION_OS_CONVERT_H_
