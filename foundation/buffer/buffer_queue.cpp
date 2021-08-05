// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#include "foundation/alloter/pool_block.h"
#include "foundation/buffer/buffer_queue.h"
#include "foundation/buffer/buffer_block.h"
#include "foundation/alloter/alloter_interface.h"

namespace fdan {

BufferQueue::BufferQueue(const std::shared_ptr<BlockMemoryPool>& block_pool,
  const std::shared_ptr<AlloterWrap>& alloter, uint16_t block_vec_default_size):
  can_read_length_(0),
  block_vec_default_size_(block_vec_default_size),
  block_alloter_(block_pool),
  alloter_(alloter) {}

BufferQueue::~BufferQueue() {
  buffer_list_.Clear();
}

uint32_t BufferQueue::ReadNotMovePt(char* res, uint32_t len) {
  if (buffer_list_.Size() == 0 || !res) {
    return 0;
  }

  std::shared_ptr<BufferBlock> temp = buffer_list_.GetHead();
  uint32_t read_len = 0;
  while (temp && read_len < len) {
    read_len += temp->ReadNotMovePt(res + read_len, len - read_len);
    if (temp == buffer_write_) {
      break;
    }
    temp = temp->GetNext();
  }
  return read_len;
}

uint32_t BufferQueue::Read(std::shared_ptr<Buffer> buffer, uint32_t len) {
  if (!buffer) {
    return 0;
  }

  if (len == 0) {
    len = GetCanReadLength();
  }

  if (len == 0) {
    return 0;
  }

  std::shared_ptr<BufferQueue> buffer_queue =
    std::dynamic_pointer_cast<BufferQueue>(buffer);
  if (!buffer_queue->buffer_write_) {
    buffer_queue->Append();
    buffer_queue->buffer_write_ = buffer_queue->buffer_list_.GetTail();
  }

  uint32_t can_write_size = buffer_queue->buffer_write_->GetCanWriteLength();
  uint32_t total_read_len = 0;
  uint32_t cur_read_len = 0;

  auto buffer_read = buffer_list_.GetHead();
  while (buffer_read) {
    cur_read_len = buffer_read->Read(buffer_queue->buffer_write_,
      can_write_size);
    total_read_len += cur_read_len;

    // current write block is full
    if (cur_read_len >= can_write_size) {
      if (total_read_len >= len) {
        break;
      }

      buffer_queue->Append();
      buffer_queue->buffer_write_ = buffer_queue->buffer_list_.GetTail();
      can_write_size = buffer_queue->buffer_write_->GetCanWriteLength();

    // current read block is empty
    } else {
      can_write_size -= cur_read_len;
      if (buffer_read == buffer_write_) {
        if (buffer_write_->GetNext()) {
          buffer_write_ = buffer_write_->GetNext();

        } else {
          Reset();
          break;
        }
      }
      buffer_list_.PopFront();
      buffer_read = buffer_list_.GetHead();
    }

    if (total_read_len >= len) {
      break;
    }
  }
  can_read_length_ -= total_read_len;
  buffer_queue->can_read_length_ += total_read_len;
  return total_read_len;
}

uint32_t BufferQueue::Write(std::shared_ptr<Buffer> buffer, uint32_t len) {
  if (!buffer) {
    return 0;
  }

  if (len == 0) {
    len = buffer->GetCanReadLength();
  }
  if (len == 0) {
    return 0;
  }

  std::shared_ptr<BufferQueue> buffer_queue =
    std::dynamic_pointer_cast<BufferQueue>(buffer);
  auto from_list = buffer_queue->buffer_list_;
  auto from_buffer = from_list.GetHead();

  uint32_t should_write_size = from_list.GetHead()->GetCanReadLength();
  uint32_t total_write_len = 0;
  uint32_t cur_write_len = 0;

  while (1) {
    if (!buffer_write_) {
      Append();
      buffer_write_ = buffer_list_.GetTail();
    }

    cur_write_len = buffer_write_->Write(from_buffer, should_write_size);
    total_write_len += cur_write_len;

    // current read block is empty
    if (cur_write_len >= should_write_size) {
      if (from_buffer == buffer_queue->buffer_write_) {
        if (buffer_queue->buffer_write_->GetNext()) {
          buffer_queue->buffer_write_ = buffer_queue->buffer_write_->GetNext();

        } else {
          buffer_queue->Reset();
          break;
        }
      }
      from_list.PopFront();
      from_buffer = from_list.GetHead();
      should_write_size = from_buffer->GetCanReadLength();

    // current write block is full
    } else {
      if (total_write_len >= len) {
        break;
      }
      should_write_size -= cur_write_len;
      buffer_write_ = buffer_write_->GetNext();
    }

    if (total_write_len >= len) {
      break;
    }
  }
  can_read_length_ += total_write_len;
  buffer_queue->can_read_length_ -= total_write_len;
  return total_write_len;
}

uint32_t BufferQueue::Read(char* res, uint32_t len) {
  if (buffer_list_.Size() == 0 || !res || len == 0) {
    return 0;
  }

  auto buffer_read = buffer_list_.GetHead();
  uint32_t total_read_len = 0;
  while (buffer_read) {
    total_read_len += buffer_read->Read(res + total_read_len,
      len - total_read_len);
    if (total_read_len >= len) {
      break;
    }
    if (buffer_read == buffer_write_) {
      if (buffer_write_->GetNext()) {
        buffer_write_ = buffer_write_->GetNext();

      } else {
        Reset();
        break;
      }
    }
    buffer_list_.PopFront();
    buffer_read = buffer_list_.GetHead();
    if (buffer_read->GetCanReadLength() == 0) {
      break;
    }
  }
  can_read_length_ -= total_read_len;
  return total_read_len;
}

uint32_t BufferQueue::Write(const char* str, uint32_t len) {
  if (!str || len == 0) {
    return 0;
  }

  uint32_t write_len = 0;

  while (1) {
    if (!buffer_write_) {
      Append();
      buffer_write_ = buffer_list_.GetTail();
    }

    write_len += buffer_write_->Write(str + write_len, len - write_len);

    if (write_len >= len) {
      break;
    }
    buffer_write_ = buffer_write_->GetNext();
  }
  can_read_length_ += write_len;
  return write_len;
}

void BufferQueue::Clear() {
  can_read_length_ = 0;
  Reset();
}

int32_t BufferQueue::MoveReadPt(int32_t len) {
  int32_t total_read_len = 0;
  auto buffer_read = buffer_list_.GetHead();

  if (len >= 0) {
    while (buffer_read) {
      total_read_len += buffer_read->MoveReadPt(len - total_read_len);

      if (total_read_len >= len) {
        break;
      }

      if (buffer_read == buffer_write_) {
        if (buffer_write_->GetNext()) {
          buffer_write_ = buffer_write_->GetNext();

        } else {
          Reset();
          break;
        }
      }
      buffer_list_.PopFront();
      buffer_read = buffer_list_.GetHead();
    }

  } else {
    total_read_len += buffer_read->MoveReadPt(len);
  }

  can_read_length_ -= total_read_len;
  return total_read_len;
}

int32_t BufferQueue::MoveWritePt(int32_t len) {
  int32_t total_write_len = 0;
  if (len >= 0) {
    while (buffer_write_) {
      total_write_len += buffer_write_->MoveWritePt(len - total_write_len);
      if (buffer_write_ == buffer_list_.GetTail() || len <= total_write_len) {
        break;
      }
      buffer_write_ = buffer_write_->GetNext();
    }

  } else {
    while (buffer_write_) {
      total_write_len += buffer_write_->MoveWritePt(len + total_write_len);
      if (buffer_write_ == buffer_list_.GetHead() || -len <= total_write_len) {
        break;
      }
      buffer_write_ = buffer_write_->GetPrev();
    }
  }
  can_read_length_ += total_write_len;
  return total_write_len;
}

uint32_t BufferQueue::ReadUntil(char* res, uint32_t len) {
  if (GetCanReadLength() < len) {
    return 0;

  } else {
    return Read(res, len);
  }
}

uint32_t BufferQueue::ReadUntil(char* res, uint32_t len, const char* find,
  uint32_t find_len, uint32_t& need_len) {
  uint32_t size = FindStr(find, find_len);
  if (size) {
    if (size <= len) {
      return Read(res, size);

    } else {
      need_len = size;
      return 0;
    }
  }
  return 0;
}

uint32_t BufferQueue::GetCanWriteLength() {
  if (!buffer_write_) {
    return 0;
  }

  std::shared_ptr<BufferBlock> temp = buffer_write_;
  uint32_t total_len = 0;
  while (temp) {
    total_len += temp->GetCanWriteLength();
    if (temp == buffer_list_.GetTail()) {
      break;
    }
    temp = temp->GetNext();
  }
  return total_len;
}

uint32_t BufferQueue::GetCanReadLength() {
  return can_read_length_;
}

uint32_t BufferQueue::GetFreeMemoryBlock(std::vector<net::Iovec>& block_vec,
  uint32_t size) {
  void* mem_1 = nullptr;
  void* mem_2 = nullptr;
  uint32_t mem_len_1 = 0;
  uint32_t mem_len_2 = 0;

  block_vec.reserve(block_vec_default_size_);
  std::shared_ptr<BufferBlock> temp = buffer_write_;
  uint32_t cur_len = 0;
  if (size > 0) {
    while (cur_len < size) {
      if (temp == nullptr) {
        Append();
        temp = buffer_list_.GetTail();
      }

      temp->GetFreeMemoryBlock(mem_1, mem_len_1, mem_2, mem_len_2);
      if (mem_len_1 > 0) {
        block_vec.emplace_back(net::Iovec(mem_1, mem_len_1));
        cur_len += mem_len_1;
      }
      if (mem_len_2 > 0) {
        block_vec.emplace_back(net::Iovec(mem_2, mem_len_2));
        cur_len += mem_len_2;
      }
      temp = temp->GetNext();
    }

  } else {
    // add one block
    if (!temp) {
      Append();
      temp = buffer_list_.GetTail();
    }

    while (temp) {
      temp->GetFreeMemoryBlock(mem_1, mem_len_1, mem_2, mem_len_2);
      if (mem_len_1 > 0) {
        block_vec.emplace_back(net::Iovec(mem_1, mem_len_1));
        cur_len += mem_len_1;
      }
      if (mem_len_2 > 0) {
        block_vec.emplace_back(net::Iovec(mem_2, mem_len_2));
        cur_len += mem_len_2;
      }
      if (temp == buffer_list_.GetTail()) {
        break;
      }
      temp = temp->GetNext();
    }
  }
  return cur_len;
}

uint32_t BufferQueue::GetUseMemoryBlock(std::vector<net::Iovec>& block_vec,
  uint32_t max_size) {
  void* mem_1 = nullptr;
  void* mem_2 = nullptr;
  uint32_t mem_len_1 = 0;
  uint32_t mem_len_2 = 0;

  block_vec.reserve(block_vec_default_size_);
  std::shared_ptr<BufferBlock> temp = buffer_list_.GetHead();
  uint32_t cur_len = 0;
  while (temp) {
    temp->GetUseMemoryBlock(mem_1, mem_len_1, mem_2, mem_len_2);
    if (mem_len_1 > 0) {
      block_vec.emplace_back(net::Iovec(mem_1, mem_len_1));
      cur_len += mem_len_1;
    }
    if (mem_len_2 > 0) {
      block_vec.emplace_back(net::Iovec(mem_2, mem_len_2));
      cur_len += mem_len_2;
    }
    if (temp == buffer_write_) {
      break;
    }
    if (max_size > 0 && cur_len >= max_size) {
      break;
    }
    temp = temp->GetNext();
  }
  return cur_len;
}

uint32_t BufferQueue::FindStr(const char* s, uint32_t s_len) {
  if (buffer_list_.Size() == 0) {
    return 0;
  }

  std::shared_ptr<BufferBlock> temp = buffer_list_.GetHead();
  uint32_t cur_len = 0;
  uint32_t can_read = 0;
  while (temp) {
    can_read = temp->FindStr(s, s_len);
    if (can_read > 0) {
      cur_len += can_read;
      break;
    }
    if (temp == buffer_write_) {
      break;
    }
    cur_len += temp->GetCanReadLength();
    temp = temp->GetNext();
  }
  return cur_len;
}

std::shared_ptr<BlockMemoryPool> BufferQueue::GetBlockMemoryPool() {
  return block_alloter_;
}

void BufferQueue::Reset() {
  buffer_list_.Clear();
  buffer_write_.reset();
}

void BufferQueue::Append() {
  auto temp = alloter_->PoolNewSharePtr<BufferBlock>(block_alloter_);

  if (!buffer_write_) {
    buffer_write_ = temp;
  }

  buffer_list_.PushBack(temp);
}

}  // namespace fdan
