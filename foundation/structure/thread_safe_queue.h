// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_STRUCTURE_THREAD_SAFE_QUEUE_H_
#define FOUNDATION_STRUCTURE_THREAD_SAFE_QUEUE_H_

#include <mutex>
#include <queue>
#include <utility>

namespace fdan {

template<typename T>
class ThreadSafeQueue {
 public:
  ThreadSafeQueue() {}
  ~ThreadSafeQueue() {}

  void Push(const T& element) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(element);
  }

  bool Pop(T& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (queue_.empty()) {
      return false;
    }
    value = std::move(queue_.front());
    queue_.pop();
    return true;
  }

  void Clear() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (!queue_.empty()) {
      queue_.pop();
    }
  }

  size_t Size() {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.size();
  }

  bool Empty() {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.empty();
  }

 private:
  std::mutex    mutex_;
  std::queue<T> queue_;
};

}  // namespace fdan

#endif  // FOUNDATION_STRUCTURE_THREAD_SAFE_QUEUE_H_
