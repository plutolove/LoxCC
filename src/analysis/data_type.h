#pragma once

#include <future>

#include "analysis/type_index.h"
#include "common/auto_registration_factory.h"

namespace Lox {

class DataTypeBase {
 public:
  virtual ~DataTypeBase() {}
  DataTypeBase(size_t size) : size(size) {}
  virtual TypeIndex typeIndex() const = 0;
  virtual std::string_view typeName() const = 0;
  virtual bool isSigned() const = 0;
  size_t getSize() const { return size; }

 private:
  size_t size{0};
};

template <typename T>
class DataType : public DataTypeBase {
 public:
  using DataTypeBase::DataTypeBase;
  using ValueType = std::decay_t<T>;

  DataType();
  virtual TypeIndex typeIndex() const override;
  virtual std::string_view typeName() const override;
  virtual bool isSigned() const override;
};

using DataTypePtr = std::shared_ptr<DataTypeBase>;

#define EXTERN_EXPAND_TYPE(type) extern template class DataType<type>;

#define EXTERN_EXPAND_TYPE_BOOST(r, seq) \
  EXTERN_EXPAND_TYPE(BOOST_PP_SEQ_ELEM(0, seq))

BOOST_PP_SEQ_FOR_EACH_PRODUCT(EXTERN_EXPAND_TYPE_BOOST,
                              ((i32)(i64)(u32)(u64)(f32)(f64)(bool)));

static inline DataTypePtr CommonType(const DataTypePtr& lhs,
                                     const DataTypePtr& rhs) {
  if (lhs->typeIndex() == lhs->typeIndex()) {
    return lhs;
  }
  if (lhs->getSize() != rhs->getSize()) {
    return lhs->getSize() > rhs->getSize() ? lhs : rhs;
  }
  if (lhs->isSigned()) return rhs;
  return lhs;
}

}  // namespace Lox
