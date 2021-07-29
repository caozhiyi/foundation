// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#include <cstring>
#include "foundation/alloter/pool_block.h"
#include "foundation/buffer/buffer_block.h"

namespace fdan {

BufferBlock::BufferBlock(std::shared_ptr<BlockMemoryPool>& alloter):
  can_read_(false),
  alloter_(alloter) {
  buffer_start_ = reinterpret_cast<char*>(alloter->PoolLargeMalloc());
  total_size_ = alloter->GetBlockLength();
  buffer_end_ = buffer_start_ + total_size_;
  read_ = write_ = buffer_start_;
}

BufferBlock::~BufferBlock() {
  if (buffer_start_) {
    auto alloter = alloter_.lock();
    if (alloter) {
      void* m = reinterpret_cast<char*>(buffer_start_);
      alloter->PoolLargeFree(m);
    }
  }
}

uint32_t BufferBlock::ReadNotMovePt(char* res, uint32_t len) {
  return _Read(res, len, false);
}

uint32_t BufferBlock::Read(std::shared_ptr<InnerBuffer> buffer, uint32_t len) {
  if (!buffer_start_) {
    return 0;
  }

  if (!can_read_ && read_ == write_) {
    return 0;
  }

  if (len == 0) {
    len = GetCanReadLength();
  }

  void* data1 = nullptr, *data2 = nullptr;
  uint32_t len1 = 0, len2 = 0;

  GetUseMemoryBlock(data1, len1, data2, len2);
  uint32_t total_size = len1 + len2;
  uint32_t read_done_size = 0;

  // read all data
  if (len >= total_size) {
    read_done_size += buffer->Write(reinterpret_cast<char*>(data1), len1);
    read_done_size += buffer->Write(reinterpret_cast<char*>(data2), len2);

  // write part of data
  } else if (len < total_size && len >= len1) {
    read_done_size += buffer->Write(reinterpret_cast<char*>(data1), len1);
    read_done_size += buffer->Write(reinterpret_cast<char*>(data2), len - len1);

  // write part of data
  } else {
    read_done_size += buffer->Write(reinterpret_cast<char*>(data1), len);
  }

  MoveReadPt(read_done_size);
  return read_done_size;
}

uint32_t BufferBlock::Write(std::shared_ptr<InnerBuffer> buffer, uint32_t len) {
  if (len == 0) {
    len = buffer->GetCanReadLength();
  }

  if (!buffer_start_) {
    return 0;
  }

  if (!can_read_ && read_ == write_) {
    write_ = read_ = buffer_start_;
  }

  void* data1 = nullptr, *data2 = nullptr;
  uint32_t len1 = 0, len2 = 0;

  std::shared_ptr<BufferBlock> block_buffer =
    std::dynamic_pointer_cast<BufferBlock>(buffer);
  block_buffer->GetUseMemoryBlock(data1, len1, data2, len2);
  uint32_t total_size = len1 + len2;
  uint32_t write_done_size = 0;

  // write all data
  if (len >= total_size) {
    write_done_size += Write(reinterpret_cast<char*>(data1), len1);
    write_done_size += Write(reinterpret_cast<char*>(data2), len2);

  // write part of data
  } else if (len < total_size && len >= len1) {
    write_done_size += Write(reinterpret_cast<char*>(data1), len1);
    write_done_size += Write(reinterpret_cast<char*>(data2), len - len1);

  // write part of data
  } else {
    write_done_size += Write(reinterpret_cast<char*>(data1), len);
  }

  buffer->MoveReadPt(write_done_size);
  return write_done_size;
}

uint32_t BufferBlock::Read(char* res, uint32_t len) {
  if (res == nullptr) {
    return 0;
  }
  return _Read(res, len, true);
}

uint32_t BufferBlock::Write(const char* data, uint32_t len) {
  if (data == nullptr) {
    return 0;
  }
  return _Write(data, len);
}

void BufferBlock::Clear() {
  write_ = read_ = buffer_start_;
  can_read_ = false;
}

int32_t BufferBlock::MoveReadPt(int32_t len) {
  if (!buffer_start_) {
    return 0;
  }

  if (len > 0) {
    /*s-----------r-----w-------------e*/
    if (read_ < write_) {
      size_t size = write_ - read_;
      // res can load all
      if ((int32_t)size <= len) {
        Clear();
        return (int32_t)size;

      // only read len
      } else {
        read_ += len;
        return len;
      }

    /*s-----------w-----r-------------e*/
    /*s-----------wr------------------e*/
    } else {
      if (!can_read_ && read_ == write_) {
        return 0;
      }
      size_t size_start = write_ - buffer_start_;
      size_t size_end = buffer_end_ - read_;
      size_t size =  size_start + size_end;
      // res can load all
      if ((int32_t)size <= len) {
        Clear();
        return (int32_t)size;

      // only read len
      } else {
        if (len <= (int32_t)size_end) {
          read_ += len;
          return len;

        } else {
          size_t left = len - size_end;
          read_ = buffer_start_ + left;
          return len;
        }
      }
    }

  } else {
    len = -len;
    /*s-----------w-----r-------------e*/
    if (write_ < read_) {
      size_t size = read_ - write_;
      // reread all buffer
      if ((int32_t)size <= len) {
        read_ = write_;
        can_read_ = true;
        return (int32_t)size;

      // only reread part of buffer
      } else {
        read_ -= len;
        return len;
      }

    /*s-----------r-----w-------------e*/
    /*s-----------rw------------------e*/
    } else {
      if (can_read_ && read_ == write_) {
        return 0;
      }
      size_t size_start = read_ - buffer_start_;
      size_t size_end = buffer_end_ - write_;
      size_t size =  size_start + size_end;
      // reread all buffer
      if ((int32_t)size <= len) {
        read_ = write_;
        can_read_ = true;
        return (int32_t)size;

      // only reread part of buffer
      } else {
        if (len <= (int32_t)size_start) {
          read_ -= len;
          return len;

        } else {
          size_t left = len - size_start;
          read_ = buffer_end_ - left;
          return len;
        }
      }
    }
  }
}

int32_t BufferBlock::MoveWritePt(int32_t len) {
  if (!buffer_start_) {
    return 0;
  }

  if (len > 0) {
    /*s-----------w-----r-------------e*/
    if (write_ < read_) {
      size_t size = read_ - write_;
      // all buffer will be used
      if ((int32_t)size <= len) {
        write_ = read_;
        can_read_ = true;
        return (int32_t)size;

      // part of buffer will be used
      } else {
        write_ += len;
        return len;
      }

    /*s-----------r-----w-------------e*/
    /*s-----------rw------------------e*/
    } else {
      if (can_read_ && read_ == write_) {
        return 0;
      }
      size_t size_start = read_ - buffer_start_;
      size_t size_end = buffer_end_ - write_;
      size_t size =  size_start + size_end;

      // all buffer will be used
      if ((int32_t)size <= len) {
        write_ = read_;
        can_read_ = true;
        return (int32_t)size;

      // part of buffer will be used
      } else {
        if (len <= (int32_t)size_end) {
          write_ += len;
          return len;

        } else {
          size_t left = len - size_end;
          write_ = buffer_start_ + left;
          return len;
        }
      }
    }

  } else {
    len = -len;
    /*s-----------r-----w-------------e*/
    if (read_ < write_) {
      size_t size = write_ - read_;
      // rewrite all buffer
      if ((int32_t)size <= len) {
        Clear();
        return (int32_t)size;

      // only rewrite part of buffer
      } else {
        write_ -= len;
        return len;
      }

    /*s-----------w-----r-------------e*/
    /*s-----------wr------------------e*/
    } else {
      if (!can_read_ && read_ == write_) {
        return 0;
      }
      size_t size_start = write_ - buffer_start_;
      size_t size_end = buffer_end_ - read_;
      size_t size =  size_start + size_end;
      // rewrite all buffer
      if ((int32_t)size <= len) {
        Clear();
        return (int32_t)size;

      // only rewrite part of buffer
      } else {
        if (len <= (int32_t)size_start) {
          write_ -= len;
          return len;

        } else {
          size_t left = len - size_start;
          write_ = buffer_end_ - left;
          return len;
        }
      }
    }
  }
}

uint32_t BufferBlock::ReadUntil(char* res, uint32_t len) {
  if (GetCanReadLength() < len) {
    return 0;

  } else {
    return Read(res, len);
  }
}

uint32_t BufferBlock::ReadUntil(char* res, uint32_t len, const char* find,
  uint32_t find_len, uint32_t& need_len) {
  uint32_t size = FindStr(find, find_len);
  if (size) {
    if (size <= len) {
      return Read(res, len);

    } else {
      need_len = size;
      return 0;
    }
  }

  return 0;
}

uint32_t BufferBlock::GetCanWriteLength() {
  if (write_ > read_) {
    return (uint32_t)((buffer_end_ - write_) + (read_ - buffer_start_));

  } else if (write_ < read_) {
    return (uint32_t)((read_ - write_));

  } else {
    if (can_read_) {
      return 0;

    } else {
      return total_size_;
    }
  }
}

uint32_t BufferBlock::GetCanReadLength() {
  if (write_ > read_) {
    return (uint32_t)(write_ - read_);

  } else if (write_ < read_) {
    return (uint32_t)((buffer_end_ - read_) + (write_ - buffer_start_));

  } else {
    if (can_read_) {
      return total_size_;

    } else {
      return 0;
    }
  }
}

bool BufferBlock::GetFreeMemoryBlock(void*& res1, uint32_t& len1,
  void*& res2, uint32_t& len2) {
  res1 = res2 = nullptr;
  len1 = len2 = 0;

  if (write_ >= read_) {
    if (can_read_ && write_ == read_) {
      return false;
    }
    res1 = write_;
    len1 = (uint32_t)(buffer_end_ - write_);

    len2 = (uint32_t)(read_ - buffer_start_);
    if (len2 > 0) {
      res2 = buffer_start_;
    }
    return true;

  } else {
    res1 = write_;
    len1 = (uint32_t)(read_ - write_);
    return true;
  }
}

bool BufferBlock::GetUseMemoryBlock(void*& res1, uint32_t& len1,
  void*& res2, uint32_t& len2) {
  res1 = res2 = nullptr;
  len1 = len2 = 0;

  if (read_ >= write_) {
    if (!can_read_ && write_ == read_) {
      return false;
    }
    res1 = read_;
    len1 = (uint32_t)(buffer_end_ - read_);

    len2 = (uint32_t)(write_ - buffer_start_);
    if (len2 > 0) {
      res2 = buffer_start_;
    }
    return true;

  } else {
    res1 = read_;
    len1 = (uint32_t)(write_ - read_);
    return true;
  }
}

uint32_t BufferBlock::FindStr(const char* s, uint32_t s_len) {
  if (write_ > read_) {
    const char* find = _FindStrInMem(read_, s, uint32_t(write_ - read_), s_len);
    if (find) {
      return (uint32_t)(find - read_ + s_len);
    }
    return 0;

  } else if (write_ < read_) {
    const char* find = _FindStrInMem(read_, s,
      uint32_t(buffer_end_ - read_), s_len);
    if (find) {
      return uint32_t(find - read_ + s_len);
    }
    find = _FindStrInMem(buffer_start_, s,
      uint32_t(write_ - buffer_start_), s_len);
    if (find) {
      return uint32_t(find - buffer_start_ + s_len + buffer_end_ - read_);
    }
    return 0;

  } else {
    if (can_read_) {
      const char* find = _FindStrInMem(read_, s,
        uint32_t(buffer_end_ - read_), s_len);
      if (find) {
        return uint32_t(find - read_ + s_len);
      }
      find = _FindStrInMem(buffer_start_, s,
        uint32_t(write_ - buffer_start_), s_len);
      if (find) {
        return uint32_t(find - buffer_start_ + s_len + buffer_end_ - read_);
      }
      return 0;

    } else {
      return 0;
    }
  }
}

std::shared_ptr<BlockMemoryPool> BufferBlock::GetBlockMemoryPool() {
  return alloter_.lock();
}

const char* BufferBlock::_FindStrInMem(const char* buffer, const char* ch,
  uint32_t buffer_len, uint32_t ch_len) const {
  if (!buffer) {
    return nullptr;
  }

  const char* buffer_end = buffer + buffer_len;
  const char* buff = buffer;
  const char* find = nullptr;
  size_t finded = 0;
  while (true) {
    find = (char*)memchr(buff, *ch, buffer_len - finded);
    if (!find || find > buffer_end - ch_len) {
      break;
    }

    if (memcmp(find, ch, ch_len) == 0) {
      return find;
    }
    finded += find - buff + 1;
    if (buffer_len - finded < ch_len) {
      break;
    }
    buff = ++find;
  }
  return nullptr;
}

uint32_t BufferBlock::_Read(char* res, uint32_t len, bool move_pt) {
  /*s-----------r-----w-------------e*/
  if (read_ < write_) {
    size_t size = write_ - read_;
    // res can load all
    if (size <= len) {
      memcpy(res, read_, size);
      if (move_pt) {
        Clear();
      }
      return (uint32_t)size;

    // only read len
    } else {
      memcpy(res, read_, len);
      if (move_pt) {
        read_ += len;
      }
      return len;
    }

  /*s-----------w-----r-------------e*/
  /*s----------------wr-------------e*/
  } else {
    if (!can_read_ && read_ == write_) {
      return 0;
    }
    size_t size_start = write_ - buffer_start_;
    size_t size_end = buffer_end_ - read_;
    size_t size =  size_start + size_end;
    // res can load all
    if (size <= len) {
      memcpy(res, read_, size_end);
      memcpy(res + size_end, buffer_start_, size_start);
      if (move_pt) {
        // reset point
        Clear();
      }
      return (uint32_t)size;

    } else {
      if (len <= size_end) {
        memcpy(res, read_, len);
        if (move_pt) {
          read_ += len;
        }
        return len;

      } else {
        size_t left = len - size_end;
        memcpy(res, read_, size_end);
        memcpy(res + size_end, buffer_start_, left);
        if (move_pt) {
          read_ = buffer_start_ + left;
        }
        return len;
      }
    }
  }
}

uint32_t BufferBlock::_Write(const char* data, uint32_t len) {
  /*s-----------w-----r-------------e*/
  if (write_ < read_) {
    size_t size = read_ - write_;
    // can save all data
    if (len <= size) {
      memcpy(write_, data, len);
      write_ += len;
      if (write_ == read_) {
        can_read_ = true;
      }
      return len;

    // can save a part of data
    } else {
      memcpy(write_, data, size);
      write_ += size;
      can_read_ = true;

      return (uint32_t)size;
    }

  /*s-----------r-----w-------------e*/
  /*s-----------rw------------------e*/
  } else {
    if (can_read_ && read_ == write_) {
      return 0;
    }

    size_t size_start = read_ - buffer_start_;
    size_t size_end = buffer_end_ - write_;
    size_t size =  size_start + size_end;

    // all buffer will be used
    if (size <= len) {
      memcpy(write_, data, size_end);
      memcpy(buffer_start_, data + size_end, size_start);

      write_ = read_;
      can_read_ = true;
      return (int32_t)size;

    // part of buffer will be used
    } else {
      if (len <= size_end) {
        memcpy(write_, data, len);
        write_ += len;
        return len;

      } else {
        size_t left = len - size_end;

        memcpy(write_, data, size_end);
        memcpy(buffer_start_, data + size_end, left);

        write_ = buffer_start_ + left;
        return len;
      }
    }
  }
}

}  // namespace fdan
