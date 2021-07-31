// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_UTIL_SINGLETON_H_
#define FOUNDATION_UTIL_SINGLETON_H_

namespace fdan {

template<typename T>
class Singleton {
 public:
  static T& Instance() {
    static T instance;
    return instance;
  }

 protected:
  Singleton(const Singleton&) {}
  Singleton& operator = (const Singleton&) {}
  Singleton() {}
  virtual ~Singleton() {}
};

}  // namespace fdan

#endif  // FOUNDATION_UTIL_SINGLETON_H_
