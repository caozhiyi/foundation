// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_BUFFER_BUFFER_QUEUE_H_
#define FOUNDATION_BUFFER_BUFFER_QUEUE_H_

#include <vector>
#include <memory>

#include "foundation/structure/list.h"
#include "foundation/network/net_handle.h"
#include "foundation/buffer/buffer_interface.h"
#include "foundation/alloter/alloter_interface.h"

namespace fdan {

class AlloterWrap;
class BufferBlock;
class BlockMemoryPool;
class BufferQueue:
  public InnerBuffer {
 public:
  BufferQueue(const std::shared_ptr<BlockMemoryPool>& block_pool,
    const std::shared_ptr<AlloterWrap>& alloter, uint16_t block_vec_default_size = 8);
  virtual ~BufferQueue();

  // read to res buf but don't change the read point
  // return read size
  virtual uint32_t ReadNotMovePt(char* res, uint32_t len);

  virtual uint32_t Read(std::shared_ptr<InnerBuffer> buffer, uint32_t len = 0);
  virtual uint32_t Write(std::shared_ptr<InnerBuffer> buffer, uint32_t len = 0);

  virtual uint32_t Read(char* res, uint32_t len);
  virtual uint32_t Write(const char* data, uint32_t len);

  // clear all data
  virtual void Clear();

  // move read point
  virtual int32_t MoveReadPt(int32_t len);
  // move write point
  virtual int32_t MoveWritePt(int32_t len);

  // do not read when buffer less than len.
  // return len when read otherwise return 0
  virtual uint32_t ReadUntil(char* res, uint32_t len);

  // do not read when can't find specified character.
  // return read bytes when read otherwise return 0
  // when find specified character but res length is too short,
  // return 0 and the last param return need length
  virtual uint32_t ReadUntil(char* res, uint32_t len, const char* find,
    uint32_t find_len, uint32_t& need_len);

  virtual uint32_t GetCanWriteLength();
  virtual uint32_t GetCanReadLength();

  // get free memory block,
  // block_vec: memory block vector.
  // size: count block_vec's memory, bigger than size.
  // if size = 0, return existing free memory block.
  // return size of free memory.
  virtual uint32_t GetFreeMemoryBlock(std::vector<net::Iovec>& block_vec,
    uint32_t size = 0);

  // get use memory block,
  // block_vec: memory block vector.
  // return size of use memory.
  // if size = 0, return all used memory block.
  virtual uint32_t GetUseMemoryBlock(std::vector<net::Iovec>& block_vec,
    uint32_t max_size = 0);

  // return can read bytes
  virtual uint32_t FindStr(const char* s, uint32_t s_len);

  // return block memory pool
  virtual std::shared_ptr<BlockMemoryPool> GetBlockMemoryPool();

 protected:
  virtual void Reset();
  virtual void Append();

 protected:
  uint32_t can_read_length_;
  uint16_t block_vec_default_size_;

  List<BufferBlock>            buffer_list_;
  std::shared_ptr<BufferBlock> buffer_write_;

  std::shared_ptr<BlockMemoryPool> block_alloter_;
  std::shared_ptr<AlloterWrap>     alloter_;
};

}  // namespace fdan

#endif  // FOUNDATION_BUFFER_BUFFER_QUEUE_H_
