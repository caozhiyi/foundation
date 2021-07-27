// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_ALLOTER_POOL_ALLOTER_H_
#define FOUNDATION_ALLOTER_POOL_ALLOTER_H_

#ifdef F_POOL_THREAD_SAFE
#include <mutex>
#endif
#include <memory>
#include <vector>
#include <cstdint>

#include "foundation/alloter/alloter_interface.h"

namespace fdan {

static const uint32_t kDefaultMaxBytes = 256;
static const uint32_t kDefaultNumberOfFreeLists = kDefaultMaxBytes / kAlign;
static const uint32_t kDefaultNumberAddNodes = 20;

class PoolAlloter : public Alloter {
 public:
  PoolAlloter();
  ~PoolAlloter();

  void* Malloc(uint32_t size);
  void* MallocAlign(uint32_t size);
  void* MallocZero(uint32_t size);

  void Free(void* &data, uint32_t len);

 private:
  uint32_t FreeListIndex(uint32_t size, uint32_t align = kAlign) {
    return (size + align - 1) / align - 1;
  }

  void* ReFill(uint32_t size, uint32_t num = kDefaultNumberAddNodes);
  void* ChunkAlloc(uint32_t size, uint32_t& nums);

 private:
  union MemNode {
    MemNode* next;
    char     data[1];
};

#ifdef F_POOL_THREAD_SAFE
    std::mutex mutex_;
#endif
  char*  pool_start_;
  char*  pool_end_;
  std::vector<MemNode*> free_list_;
  std::vector<char*>    malloc_vec_;
  std::shared_ptr<Alloter> alloter_;
};

std::shared_ptr<Alloter> MakePoolAlloterPtr();

}  // namespace fdan

#endif  // FOUNDATION_ALLOTER_POOL_ALLOTER_H_
