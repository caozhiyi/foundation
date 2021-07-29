// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#include <ostream>
#include "foundation/network/address.h"

namespace fdan {

Address::Address():
  Address(AT_IPV6) {}

Address::Address(AddressType at):
  address_type_(at),
  ip_(""),
  port_(0) {}

Address::Address(AddressType at, const std::string& ip, uint16_t port):
  address_type_(at),
  ip_(ip),
  port_(port) {}

Address::Address(const Address& addr):
  address_type_(addr.address_type_),
  ip_(addr.ip_),
  port_(addr.port_) {}

Address::~Address() {}

void Address::SetIp(const std::string& ip) {
  if (address_type_ == AT_IPV6) {
    ip_ = ToIpv6(ip);

  } else {
    ip_ = ToIpv4(ip);
  }
}

const std::string Address::AsString() {
  if (address_type_ == AT_IPV6) {
    return "[" + ip_ + "]:" + std::to_string(port_);

  } else {
    return ip_ + ":" + std::to_string(port_);
  }
}

std::ostream& operator<< (std::ostream &out, Address &addr) {
  const std::string str = addr.AsString();
  out.write(str.c_str(), str.length());
  return out;
}

bool operator==(const Address &addr1, const Address &addr2) {
  return addr1.ip_ == addr2.ip_ &&
    addr1.port_ == addr2.port_ && addr1.port_ != 0;
}

bool Address::IsIpv4(const std::string& ip) {
  if (ip.find(':') == std::string::npos) {
    return true;
  }

  return false;
}

std::string Address::ToIpv6(const std::string& ip) {
  if (!IsIpv4(ip)) {
    return ip;
  }

  std::string ret("::FFFF:");
  ret.append(ip);
  return ret;
}

std::string Address::ToIpv4(const std::string& ip) {
  if (IsIpv4(ip)) {
    return ip;
  }
  std::size_t pos = ip.rfind(':');

  return std::string(&ip[pos], ip.length() - pos);
}

}  // namespace fdan
