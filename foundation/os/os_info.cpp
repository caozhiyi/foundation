// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#include <thread>
#include "foundation/os/os_info.h"

namespace fdan {

uint32_t GetCpuNum() {
  return std::thread::hardware_concurrency();
}

}  // namespace fdan
