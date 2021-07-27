// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_ALLOTER_POOL_BLOCK_H_
#define FOUNDATION_ALLOTER_POOL_BLOCK_H_

#ifdef F_POOL_THREAD_SAFE
#include <mutex>
#endif
#include <vector>
#include <memory>
#include <cstdint>

namespace fdan {

// all memory must return memory pool before destroy.
class BlockMemoryPool {
 public:
  // bulk memory size.
  // every time add nodes num
  BlockMemoryPool(uint32_t large_sz, uint32_t add_num);
  virtual ~BlockMemoryPool();

  // for bulk memory.
  // return one bulk memory node
  virtual void* PoolLargeMalloc();
  virtual void PoolLargeFree(void* &m);

  // return bulk memory list size
  virtual uint32_t GetSize();
  // return length of bulk memory
  virtual uint32_t GetBlockLength();

  // release half memory
  virtual void ReleaseHalf();
  virtual void Expansion(uint32_t num = 0);

 private:
#ifdef F_POOL_THREAD_SAFE
  std::mutex          mutex_;
#endif
  uint32_t            number_large_add_nodes_;  // every time add nodes num
  uint32_t            large_size_;              // bulk memory size
  std::vector<void*>  free_mem_vec_;            // free bulk memory list
};

std::shared_ptr<BlockMemoryPool> MakeBlockMemoryPoolPtr(uint32_t large_sz,
  uint32_t add_num);

}  // namespace fdan

#endif  // FOUNDATION_ALLOTER_POOL_BLOCK_H_
