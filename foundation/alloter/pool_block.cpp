// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#include <cstdlib>
#include <algorithm>
#include "foundation/alloter/pool_block.h"

namespace fdan {

static const uint16_t kMaxBlockNum = 10;

BlockMemoryPool::BlockMemoryPool(uint32_t large_sz, uint32_t add_num):
  number_large_add_nodes_(add_num),
  large_size_(large_sz) {}

BlockMemoryPool::~BlockMemoryPool() {
#ifdef F_POOL_THREAD_SAFE
  std::lock_guard<std::mutex> lock(mutex_);
#endif
  // free all memory
  for (auto iter = free_mem_vec_.begin(); iter != free_mem_vec_.end(); ++iter) {
    free(*iter);
  }
  free_mem_vec_.clear();
}

void* BlockMemoryPool::PoolLargeMalloc() {
#ifdef F_POOL_THREAD_SAFE
  std::lock_guard<std::mutex> lock(mutex_);
#endif
  if (free_mem_vec_.empty()) {
    Expansion();
  }

  void* ret = free_mem_vec_.back();
  free_mem_vec_.pop_back();
  return ret;
}

void BlockMemoryPool::PoolLargeFree(void* &m) {
  bool release = false;
  {
#ifdef F_POOL_THREAD_SAFE
    std::lock_guard<std::mutex> lock(mutex_);
#endif
    free_mem_vec_.push_back(m);

    if (free_mem_vec_.size() > kMaxBlockNum) {
      release = true;
    }
  }

  // release some block.
  if (release) {
    ReleaseHalf();
  }
}

uint32_t BlockMemoryPool::GetSize() {
#ifdef F_POOL_THREAD_SAFE
  std::lock_guard<std::mutex> lock(mutex_);
#endif
  return (uint32_t)free_mem_vec_.size();
}

uint32_t BlockMemoryPool::GetBlockLength() {
  return large_size_;
}

void BlockMemoryPool::ReleaseHalf() {
#ifdef F_POOL_THREAD_SAFE
  std::lock_guard<std::mutex> lock(mutex_);
#endif
  size_t size = free_mem_vec_.size();
  size_t hale = size / 2;
  for (auto iter = free_mem_vec_.begin(); iter != free_mem_vec_.end();) {
    void* mem = *iter;

    iter = free_mem_vec_.erase(iter);
    free(mem);

    size--;
    if (iter == free_mem_vec_.end() || size <= hale) {
      break;
    }
  }
}

void BlockMemoryPool::Expansion(uint32_t num) {
  if (num == 0) {
    num = number_large_add_nodes_;
  }

  for (uint32_t i = 0; i < num; ++i) {
    void* mem = malloc(large_size_);
    // not memset!
    free_mem_vec_.push_back(mem);
  }
}

std::shared_ptr<BlockMemoryPool> MakeBlockMemoryPoolPtr(uint32_t large_sz,
  uint32_t add_num) {
  return std::make_shared<BlockMemoryPool>(large_sz, add_num);
}

}  // namespace fdan
