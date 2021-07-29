// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_NETWORK_IO_HANDLE_H_
#define FOUNDATION_NETWORK_IO_HANDLE_H_

#include <cstdint>
#include "foundation/util/os_return.h"
#include "foundation/network/address.h"

namespace fdan {
namespace net {

struct Iovec {
#ifdef __win__
  size_t  iov_len;   // size of buffer
  void*   iov_base;  // starting address of buffer
#else
  void*   iov_base;  // starting address of buffer
  size_t  iov_len;   // size of buffer
#endif
  Iovec(void* base, size_t len): iov_base(base), iov_len(len) {}
};

SysCallInt64Result TcpSocket(bool ipv4 = false);

SysCallInt32Result Bind(int64_t sockfd, const Address& addr);

SysCallInt32Result Listen(int64_t sockfd, uint32_t len = 0);

SysCallInt32Result Connect(int64_t sockfd, const Address& addr);

SysCallInt32Result Close(int64_t sockfd);

SysCallInt64Result Accept(int64_t sockfd, Address& address);

SysCallInt32Result Write(int64_t sockfd, const char *data, uint32_t len);

SysCallInt32Result Writev(int64_t sockfd, Iovec *vec, uint32_t vec_len);

SysCallInt32Result Recv(int64_t sockfd, char *data,
  uint32_t len, uint16_t flag);

SysCallInt32Result Readv(int64_t sockfd, Iovec *vec, uint32_t vec_len);

}  // namespace net
}  // namespace fdan

#endif  // FOUNDATION_NETWORK_IO_HANDLE_H_
