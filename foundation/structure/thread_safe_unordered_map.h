// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_STRUCTURE_THREAD_SAFE_UNORDERED_MAP_H_
#define FOUNDATION_STRUCTURE_THREAD_SAFE_UNORDERED_MAP_H_

#include <mutex>
#include <utility>
#include <unordered_map>

namespace fdan {

template<typename K, typename V>
class ThreadSafeUnorderedMap {
 public:
  ThreadSafeUnorderedMap() {}
  ~ThreadSafeUnorderedMap() {}

  V& operator[] (const K& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    return unordered_map_[key];
  }

  bool Find(const K& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    return unordered_map_.find(key) != unordered_map_.end();
  }

  void Insert(const std::pair<K, V>& item) {
    std::lock_guard<std::mutex> lock(mutex_);
    unordered_map_.insert(item);
  }

  void Erase(const K& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    unordered_map_.erase(key);
  }

  void Clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    unordered_map_.clear();
  }

  size_t Size() {
    std::lock_guard<std::mutex> lock(mutex_);
    return unordered_map_.size();
  }

  bool Empty() {
    std::lock_guard<std::mutex> lock(mutex_);
    return unordered_map_.empty();
  }

  std::unordered_map<K, V>& GetMap() {
    return unordered_map_;
  }

 private:
  std::unordered_map<K, V> unordered_map_;
  std::mutex         mutex_;
};

}  // namespace fdan

#endif  // FOUNDATION_STRUCTURE_THREAD_SAFE_UNORDERED_MAP_H_
