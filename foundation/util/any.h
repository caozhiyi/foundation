// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_UTIL_ANY_H_
#define FOUNDATION_UTIL_ANY_H_

#include <utility>
#include <typeinfo>
#include <algorithm>

namespace fdan {

class Any {
 public:
  Any(): content_(0) {}
  template<typename ValueType>
  explicit Any(const ValueType & value):
    content_(new CHolder<ValueType>(value)) {}
  explicit Any(const Any & other):
    content_(other.content_ ? other.content_->Clone() : 0) {}
  explicit Any(Any&& other): content_(other.content_) {
    other.content_ = 0;
  }
  ~Any()  {
    delete content_;
  }

 public:  // modifiers
  Any& Swap(Any & rhs)  {
    std::swap(content_, rhs.content_);
    return *this;
  }
  template<typename ValueType>
  Any& operator=(const ValueType & rhs) {
    Any(rhs).Swap(*this);
    return *this;
  }
  Any& operator=(Any rhs) {
    Any(rhs).Swap(*this);
    return *this;
  }
  // move assignement
  Any& operator=(Any&& rhs)  {
    rhs.Swap(*this);
    Any().Swap(rhs);
    return *this;
  }

 public:  // queries
  bool Empty() const  {
    return !content_;
  }
  void Clear()  {
    Any().Swap(*this);
  }
  const std::type_info& Type() const  {
    return content_ ? content_->Type() : typeid(void);
  }
  class CPlaceHolder {
   public:
    virtual ~CPlaceHolder() {
    }
    // queries
    virtual const std::type_info& Type() const  = 0;
    virtual CPlaceHolder * Clone() const = 0;
  };
  template<typename ValueType>
  class CHolder : public CPlaceHolder {
   public:
    CHolder(const ValueType& value) : held_(value) {}
    CHolder(ValueType&& value) : held_(static_cast<ValueType&&>(value)) {}
    // queries
    virtual const std::type_info& Type() const  {
      return typeid(ValueType);
    }
    virtual CPlaceHolder * Clone() const {
      return new CHolder(held_);
    }

   public:
    ValueType held_;

   private:
    CHolder & operator=(const CHolder&) {}
  };

 private:  // representation
  template<typename ValueType>
  friend ValueType* any_cast(Any *);
  CPlaceHolder* content_;
};

template<typename ValueType>
ValueType* any_cast(Any * operand) {
  if (operand && operand->Type() == typeid(ValueType)) {
    return &static_cast<Any::CHolder<ValueType>*>(operand->content_)->held_;
  }
  return nullptr;
}

template<typename ValueType>
const ValueType * any_cast(const Any * operand)  {
  return any_cast<ValueType>(const_cast<Any *>(operand));
}

template<typename ValueType>
ValueType any_cast(Any & operand) {
  ValueType * result = any_cast<ValueType>(&operand);
  if (!result) {
    throw "bad_any_cast: failed conversion using any_cast";
  }
  return static_cast<ValueType>(*result);
}

template<typename ValueType>
inline ValueType any_cast(const Any& operand) {
  return any_cast<const ValueType&>(const_cast<Any&>(operand));
}

template<typename ValueType>
  inline ValueType any_cast(Any&& operand) {
  return any_cast<ValueType>(operand);
}

}  // namespace fdan

#endif  // FOUNDATION_UTIL_ANY_H_
