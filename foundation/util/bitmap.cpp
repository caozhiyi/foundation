// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)

#include <cmath>
#include "bitmap.h"

namespace fdan {

static const uint32_t __step_size = sizeof(int64_t) * 8;
static const uint64_t __bit_base = 1;

Bitmap::Bitmap() {

}

Bitmap::~Bitmap() {

}

bool Bitmap::Init(uint32_t size) {
    uint32_t vec_size = size / __step_size;
    if (size % __step_size > 0) {
        vec_size++;
    }
    _bitmap.resize(vec_size);
    for (std::size_t i = 0; i < _bitmap.size(); i++) {
        _bitmap[i] = 0;
    }
    return true;
}

bool Bitmap::Insert(uint32_t index) {
    if (index > _bitmap.size() * __step_size) {
        return false;
    }
    
    // get index in vector
    uint32_t bitmap_index = index / __step_size;
    // get index in uint64_t
    uint32_t bit_index = index % __step_size;

    _bitmap[bitmap_index] |= __bit_base << bit_index;

    return true;
}

bool Bitmap::Remove(uint32_t index) {
    if (index > _bitmap.size() * __step_size) {
        return false;
    }

    // get index in vector
    uint32_t bitmap_index = index / __step_size;
    // get index in uint64_t
    uint32_t bit_index = index % __step_size;

    _bitmap[bitmap_index] &= ~(__bit_base << bit_index);

    return true;
}

int32_t Bitmap::GetMinAfter(uint32_t index) {
    // get next bit.
    if (index != 0) {
        index++;
    }
    if (index > _bitmap.size() * __step_size || Empty()) {
        return -1;
    }

    // get index in vector
    uint32_t bitmap_index = index / __step_size;
    // get index in uint64_t
    uint32_t bit_index = index % __step_size;
    // filter smaller bitmap index
    uint32_t ret = bitmap_index * __step_size;

    for (std::size_t i = bitmap_index; i < _bitmap.size(); i++) {
        if (index > (i + 1) * __step_size) {
            ret += __step_size;

        } else {
            if (_bitmap[i] == 0) {
                ret += __step_size;
                continue;
            }
            
            // target index in current bitmap uint64_t 
            if (index > ret) {
                int64_t cur_bitmap = _bitmap[i];
                int32_t cur_step = index - ret;
                cur_bitmap = cur_bitmap >> cur_step;
                if (cur_bitmap == 0) {
                    ret += __step_size;
                    continue;
                }
                ret += cur_step;
                ret += (uint32_t)std::log2f(float(cur_bitmap & (-cur_bitmap)));
                return ret;

            // find next 
            } else {
                int64_t cur_bitmap = _bitmap[i];
                if (cur_bitmap == 0) {
                    ret += __step_size;
                    continue;
                }
                ret += (uint32_t)std::log2f(float(cur_bitmap & (-cur_bitmap)));
                return ret;
            }
        }
    }
    // shouldn't be here
    return -1;
}

bool Bitmap::Empty() {
    for (std::size_t i = 0; i < _bitmap.size(); i++) {
        if (_bitmap[i] != 0) {
            return false;
        }
    }
    return true;
}

}