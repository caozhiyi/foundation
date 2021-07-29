// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_STRUCTURE_LIST_H_
#define FOUNDATION_STRUCTURE_LIST_H_

#include <memory>
#include <cstdint>
#include "foundation/structure/list_solt.h"

namespace fdan {

template<typename T>
class List {
 public:
  List(): size_(0) {}
  ~List() {}

  uint32_t Size() { return size_; }

  std::shared_ptr<T> GetHead() { return head_; }
  std::shared_ptr<T> GetTail() { return tail_; }

  void Clear() {
    size_ = 0;

    head_.reset();
    tail_.reset();
  }

  void PushBack(std::shared_ptr<T> v) {
    if (!v) {
      return;
    }

    if (!tail_) {
      tail_ = v;
      head_ = v;

    } else {
      tail_->SetNext(v);
      v->SetPrev(tail_);
      tail_ = v;
    }
    size_++;
  }

  std::shared_ptr<T> PopBack() {
    if (!tail_) {
      return nullptr;
    }

    auto ret = tail_;
    tail_ = tail_->GetPrev();
    if (!tail_) {
      head_.reset();

    } else {
      tail_->SetNext(nullptr);
    }
    size_--;

    return ret;
  }

  void PushFront(std::shared_ptr<T> v) {
    if (!v) {
      return;
    }

    if (!head_) {
      tail_ = v;
      head_ = v;

    } else {
      head_->SetPrev(v);
      v->SetNext(head_);
      head_ = v;
    }
    size_++;
  }

  std::shared_ptr<T> PopFront() {
    if (!head_) {
      return nullptr;
    }

    auto ret = head_;
    head_ = head_->GetNext();
    if (!head_) {
      tail_.reset();
    }

    size_--;

    return ret;
  }

 private:
  uint32_t size_;
  std::shared_ptr<T> head_;
  std::shared_ptr<T> tail_;
};

}  // namespace fdan

#endif  // FOUNDATION_STRUCTURE_LIST_H_
