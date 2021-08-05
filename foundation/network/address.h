// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_NETWORK_ADDRESS_H_
#define FOUNDATION_NETWORK_ADDRESS_H_

#include <string>
#include <cstdint>

namespace fdan {

enum AddressType {
  AT_IPV4  = 0x1,
  AT_IPV6  = 0x2,
};

class Address {
 public:
  Address();
  explicit Address(AddressType at);
  explicit Address(AddressType at, const std::string& ip, uint16_t port);
  explicit Address(const Address& addr);
  ~Address();

  void SetType(AddressType at) { address_type_ = at; }
  AddressType GetType() const { return address_type_; }

  void SetIp(const std::string& ip);
  const std::string& GetIp() const { return ip_; }

  void SetAddrPort(uint16_t port) { port_ = port; }
  uint16_t GetAddrPort() const { return port_; }

  const std::string AsString();

  friend std::ostream& operator<< (std::ostream &out, Address &addr);
  friend bool operator==(const Address &addr1, const Address &addr2);

  static bool IsIpv4(const std::string& ip);
 private:
  std::string ToIpv6(const std::string& ip);
  std::string ToIpv4(const std::string& ip);

 protected:
  AddressType address_type_;
  std::string ip_;
  uint16_t    port_;
};

}  // namespace fdan

#endif  // FOUNDATION_NETWORK_ADDRESS_H_
