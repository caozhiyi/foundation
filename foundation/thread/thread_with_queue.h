// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_THREAD_THREAD_WITH_QUEUE_H_
#define FOUNDATION_THREAD_THREAD_WITH_QUEUE_H_

#include <utility>
#include "foundation/thread/thread.h"
#include "foundation/structure/thread_safe_block_queue.h"

namespace fdan {

template<typename T>
class ThreadWithQueue:
  public Thread {
 public:
  ThreadWithQueue() {}
  virtual ~ThreadWithQueue() {}

  uint32_t GetQueueSize() {
    return queue_.Size();
  }

  void Push(const T& t) {
    queue_.Push(t);
  }

  T Pop() {
    return std::move(queue_.Pop());
  }

  // TO DO
  virtual void Run() = 0;

 protected:
  ThreadWithQueue(const ThreadWithQueue&) = delete;
  ThreadWithQueue& operator=(const ThreadWithQueue&) = delete;

 private:
  ThreadSafeBlockQueue<T> queue_;
};

}  // namespace fdan

#endif  // FOUNDATION_THREAD_THREAD_WITH_QUEUE_H_
