// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#include <winsock2.h>
#include <WS2tcpip.h>

#include "foundation/network/net_handle.h"

namespace fdan {
namespace net {

SysCallInt64Result TcpSocket(bool ipv4) {
  int32_t af = AF_INET6;
  if (ipv4) {
    af = AF_INET;
  }
  int64_t sock = socket(af, SOCK_STREAM, 0);
  if (sock == INVALID_SOCKET) {
    return {sock, (int32_t)GetLastError()};
  }

  // both ipv6 and ipv4
  int32_t opt = 0;
  if (setsockopt((SOCKET)sock, IPPROTO_IPV6,
    IPV6_V6ONLY, (char*)&opt, sizeof(opt)) != 0) {
    return { sock, (int32_t)GetLastError() };
  }

  return {sock, 0};
}

SysCallInt32Result Bind(int64_t sockfd, const Address& address) {
  int32_t ret = -1;
  if (address.GetType() == AT_IPV4) {
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(address.GetAddrPort());
    inet_pton(AF_INET, address.GetIp().c_str(), &addr.sin_addr);
    ret = bind((SOCKET)sockfd, (sockaddr*)&addr, sizeof(addr));

  } else {
    SOCKADDR_IN6 addr;
    addr.sin6_flowinfo = 0;
    addr.sin6_scope_id = 0;
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(address.GetAddrPort());
    inet_pton(AF_INET6, address.GetIp().c_str(), &addr.sin6_addr);
    ret = bind((SOCKET)sockfd, (sockaddr*)&addr, sizeof(addr));
  }

  if (SOCKET_ERROR == ret) {
    return {ret, (int32_t)GetLastError()};
  }
  return {ret, 0};
}

SysCallInt32Result Listen(int64_t sockfd, uint32_t len) {
  if (len == 0) {
    len = SOMAXCONN;
  }

  int32_t ret = listen((SOCKET)sockfd, SOMAXCONN);
  if (SOCKET_ERROR == ret) {
    return {ret, (int32_t)GetLastError()};
  }

  return {ret, 0};
}

SysCallInt32Result Connect(int64_t sockfd, const Address& address) {
  return {0, 0};
}

SysCallInt32Result Close(int64_t sockfd) {
  int32_t ret = closesocket((SOCKET)sockfd);
  if (ret != 0) {
    return {ret, (int32_t)GetLastError()};
  }
  return {ret, 0};
}

SysCallInt64Result Accept(int64_t sockfd, Address& address) {
  struct sockaddr_storage client_addr;
  socklen_t addr_size = sizeof(client_addr);
  int64_t ret = accept((SOCKET)sockfd, (sockaddr*)&client_addr, &addr_size);
  if (ret < 0) {
    return { ret, (int32_t)GetLastError() };
  }

  struct sockaddr* addr_pt = (struct sockaddr*)&client_addr;

  void *addr = nullptr;
  switch (addr_pt->sa_family) {
  case AF_INET:
    addr = &((struct sockaddr_in *)addr_pt)->sin_addr;
    address.SetAddrPort(ntohs(((struct sockaddr_in *)addr_pt)->sin_port));
    address.SetType(AT_IPV4);
    break;
  case AF_INET6:
    addr = &((struct sockaddr_in6 *)addr_pt)->sin6_addr;
    address.SetAddrPort((((struct sockaddr_in6 *)addr_pt)->sin6_port));
    address.SetType(AT_IPV6);
    break;
  default:
    return { -1, (int32_t)GetLastError() };
  }

  char str_addr[INET6_ADDRSTRLEN] = { 0 };
  inet_ntop(AF_INET6, addr, str_addr, sizeof(str_addr));
  address.SetIp(str_addr);

  return { ret, 0 };
}

SysCallInt32Result Write(int64_t sockfd, const char *data, uint32_t len) {
  int32_t ret = send((SOCKET)sockfd, data, len, 0);
  if (ret < 0) {
    return { ret, (int32_t)GetLastError() };
  }
  return { ret, 0 };
}

SysCallInt32Result Writev(int64_t sockfd, Iovec *vec, uint32_t vec_len) {
  DWORD flags = 0;
  DWORD send_bytes = 0;
  int32_t ret = WSASend((SOCKET)sockfd, (LPWSABUF)vec,
    (DWORD)vec_len, &send_bytes, flags, nullptr, nullptr);

  if (ret == SOCKET_ERROR) {
    return { (int32_t)send_bytes, WSAGetLastError() };
  }

  return { (int32_t)send_bytes, 0 };
}

SysCallInt32Result Recv(int64_t sockfd, char *data,
  uint32_t len, uint16_t flag) {
  int32_t ret = recv((SOCKET)sockfd, data, len, 0);
  if (ret < 0) {
    return { ret, (int32_t)GetLastError() };
  }
  return { ret, 0 };
}

SysCallInt32Result Readv(int64_t sockfd, Iovec *vec, uint32_t vec_len) {
  DWORD flags = 0;
  DWORD send_bytes = 0;
  int32_t ret = WSARecv((SOCKET)sockfd, (LPWSABUF)vec,
    (DWORD)vec_len, &send_bytes, &flags, nullptr, nullptr);
  if (ret == SOCKET_ERROR) {
    return { (int32_t)send_bytes, WSAGetLastError() };
  }
  return { (int32_t)send_bytes, 0 };
}

}  // namespace net
}  // namespace fdan
