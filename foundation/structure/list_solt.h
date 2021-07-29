// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_STRUCTURE_LIST_SOLT_H_
#define FOUNDATION_STRUCTURE_LIST_SOLT_H_

#include <memory>

namespace fdan {

template<typename T>
class ListSolt {
 public:
  ListSolt() {}
  virtual ~ListSolt() {}

  void SetNext(std::shared_ptr<T> v) { next_ = v; }
  std::shared_ptr<T> GetNext() { return next_; }

  void SetPrev(std::shared_ptr<T> v) { prev_ = v; }
  std::shared_ptr<T> GetPrev() { return prev_.lock(); }

 protected:
  std::weak_ptr<T>   prev_;
  std::shared_ptr<T> next_;
};

}  // namespace fdan

#endif  // FOUNDATION_STRUCTURE_LIST_SOLT_H_
