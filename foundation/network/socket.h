// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_NETWORK_SOCKET_H_
#define FOUNDATION_NETWORK_SOCKET_H_

#include <cstdint>

namespace fdan {

int32_t SocketNoblocking(uint64_t sock);

int32_t ReusePort(uint64_t sock);

// check socket connect
bool CheckConnect(const uint64_t sock);

}  // namespace fdan

#endif  // FOUNDATION_NETWORK_SOCKET_H_
