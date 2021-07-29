// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#include <cstring>
#include <cstdlib>

#include "foundation/alloter/pool_alloter.h"
#include "foundation/alloter/normal_alloter.h"

namespace fdan {

PoolAlloter::PoolAlloter():
  pool_start_(nullptr),
  pool_end_(nullptr) {
  free_list_.resize(kDefaultNumberOfFreeLists);
  memset(&(*free_list_.begin()), 0,
    sizeof(void*) * kDefaultNumberOfFreeLists);
  alloter_ = MakeNormalAlloterPtr();
}

PoolAlloter::~PoolAlloter() {
  for (auto iter = malloc_vec_.begin(); iter != malloc_vec_.end(); ++iter) {
    if (*iter) {
      void* data = reinterpret_cast<void*>(*iter);
      alloter_->Free(data);
    }
  }
}

void* PoolAlloter::Malloc(uint32_t size) {
  if (size > kDefaultMaxBytes) {
    void* ret = alloter_->Malloc(size);
    return ret;
}
#ifdef F_POOL_THREAD_SAFE
  std::lock_guard<std::mutex> lock(mutex_);
#endif
  MemNode** my_free = &(free_list_[FreeListIndex(size)]);
  MemNode* result = *my_free;
  if (result == nullptr) {
    void* bytes = ReFill(Align(size));
    return bytes;
  }

  *my_free = result->next;
  return result;
}

void* PoolAlloter::MallocAlign(uint32_t size) {
  return Malloc(Align(size));
}

void* PoolAlloter::MallocZero(uint32_t size) {
  void* ret = Malloc(size);
  if (ret) {
    memset(ret, 0, size);
  }
  return ret;
}

void PoolAlloter::Free(void* &data, uint32_t len) {
  if (!data) {
    return;
  }

  if (len > kDefaultMaxBytes) {
    alloter_->Free(data);
    data = nullptr;
    return;
  }

#ifdef F_POOL_THREAD_SAFE
  std::lock_guard<std::mutex> lock(mutex_);
#endif
  MemNode* node = reinterpret_cast<MemNode*>(data);
  MemNode** my_free = &(free_list_[FreeListIndex(len)]);

  node->next = *my_free;
  *my_free = node;
  data = nullptr;
}

void* PoolAlloter::ReFill(uint32_t size, uint32_t num) {
  uint32_t nums = num;

  char* chunk = reinterpret_cast<char*>(ChunkAlloc(size, nums));

  MemNode* volatile* my_free;
  MemNode* res, *current, *next;
  if (1 == nums) {
    return chunk;
  }

  res = reinterpret_cast<MemNode*>(chunk);

  my_free = &(free_list_[FreeListIndex(size)]);

  *my_free = next = reinterpret_cast<MemNode*>(chunk + size);
  for (uint32_t i = 1;; i++) {
    current = next;
    next = reinterpret_cast<MemNode*>(reinterpret_cast<char*>(next) + size);
    if (nums - 1 == i) {
      current->next = nullptr;
      break;

    } else {
      current->next = next;
    }
  }
  return res;
}

void* PoolAlloter::ChunkAlloc(uint32_t size, uint32_t& nums) {
  char* res;
  uint32_t need_bytes = size * nums;
  uint32_t left_bytes = uint32_t(pool_end_ - pool_start_);

  // pool is enough
  if (left_bytes >= need_bytes) {
    res = pool_start_;
    pool_start_ += need_bytes;
    return res;

  } else if (left_bytes >= size) {
    nums = left_bytes / size;
    need_bytes = size * nums;
    res = pool_start_;
    pool_start_ += need_bytes;
    return res;
  }

  uint32_t bytes_to_get = size * nums;

  if (left_bytes > 0) {
    MemNode* my_free = free_list_[FreeListIndex(left_bytes)];
    (reinterpret_cast<MemNode*>(pool_start_))->next = my_free;
    free_list_[FreeListIndex(size)] = reinterpret_cast<MemNode*>(pool_start_);
  }

  pool_start_ = reinterpret_cast<char*>(alloter_->Malloc(bytes_to_get));

  malloc_vec_.push_back(pool_start_);
  pool_end_ = pool_start_ + bytes_to_get;
  return ChunkAlloc(size, nums);
}

std::shared_ptr<Alloter> MakePoolAlloterPtr() {
  return std::make_shared<fdan::PoolAlloter>();
}

}  // namespace fdan
