// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#include <cmath>
#include "foundation/util/bitmap.h"

namespace fdan {

static const uint32_t kStepSize = sizeof(int64_t) * 8;
static const uint64_t kSetpBase = 1;

Bitmap::Bitmap():
  vec_bitmap_(0) {}

Bitmap::~Bitmap() {}

bool Bitmap::Init(uint32_t size) {
  uint32_t vec_size = size / kStepSize;
  // too large size
  if (vec_size > sizeof(vec_bitmap_) * 8) {
    return false;
  }
  if (size % kStepSize > 0) {
    vec_size++;
  }
  bitmap_.resize(vec_size);
  for (std::size_t i = 0; i < bitmap_.size(); i++) {
    bitmap_[i] = 0;
  }
  return true;
}

bool Bitmap::Insert(uint32_t index) {
  if (index > bitmap_.size() * kStepSize) {
    return false;
  }

  // get index in vector
  uint32_t bitmap_index = index / kStepSize;
  // get index in uint64_t
  uint32_t bit_index = index % kStepSize;

  bitmap_[bitmap_index] |= kSetpBase << bit_index;
  vec_bitmap_ |= kSetpBase << bitmap_index;

  return true;
}

bool Bitmap::Remove(uint32_t index) {
  if (index > bitmap_.size() * kStepSize) {
    return false;
  }

  // get index in vector
  uint32_t bitmap_index = index / kStepSize;
  // get index in uint64_t
  uint32_t bit_index = index % kStepSize;

  bitmap_[bitmap_index] &= ~(kSetpBase << bit_index);
  if (bitmap_[bitmap_index] == 0) {
    vec_bitmap_ &= ~(kSetpBase << bitmap_index);
  }
  return true;
}

int32_t Bitmap::GetMinAfter(uint32_t index) {
  // get next bit.
  if (index >= bitmap_.size() * kStepSize || Empty()) {
    return -1;
  }

  // get index in vector
  uint32_t bitmap_index = index / kStepSize;
  // filter smaller bitmap index
  uint32_t ret = bitmap_index * kStepSize;

  // find current uint64_t have next 1?
  if (bitmap_[bitmap_index] != 0) {
    int64_t curbitmap_ = bitmap_[bitmap_index];
    int32_t cur_step = index - ret;
    curbitmap_ = curbitmap_ >> cur_step;

    // don't have next 1
    if (curbitmap_ == 0) {
      ret += kStepSize;

    // find next 1
    } else {
      ret += cur_step;
      ret += (uint32_t)std::log2f(static_cast<float>(curbitmap_ & (-curbitmap_)));
      return ret;
    }

  } else {
    ret += kStepSize;
  }

  // find next used vector index
  int32_t tempvec_bitmap_ = vec_bitmap_ >> bitmap_index;
  if (tempvec_bitmap_ == 0) {
    return -1;
  }

  uint32_t next_vec_index =
    (uint32_t)std::log2f(static_cast<float>(tempvec_bitmap_ & (-tempvec_bitmap_) + 1));
  uint32_t target_vec_index = next_vec_index + bitmap_index;
  if (target_vec_index == bitmap_index) {
    return -1;
  }

  int64_t curbitmap_ = bitmap_[target_vec_index];
  ret += (next_vec_index - 1) * kStepSize;
  ret += (uint32_t)std::log2f(static_cast<float>(curbitmap_ & (-curbitmap_) + 1));

  return ret;
}

bool Bitmap::Empty() {
  return vec_bitmap_ == 0;
}

void Bitmap::Clear() {
  while (vec_bitmap_ != 0) {
    int32_t next_vec_index = (int32_t)std::log2f(static_cast<float>(vec_bitmap_ & (-(int32_t)vec_bitmap_) + 1));
    bitmap_[next_vec_index] = 0;
    vec_bitmap_ = vec_bitmap_ & (vec_bitmap_ - 1);
  }
}

}  // namespace fdan
