#pragma once

#include "analysis/type_index.h"
namespace Lox {

class DataTypeBase {
 public:
  virtual ~DataTypeBase() {}
  virtual TypeIndex typeIndex() const = 0;
  virtual std::string_view typeName() const = 0;
};

template <typename T>
class DataType : public DataTypeBase {
 public:
  using ValueType = std::decay_t<T>;
  virtual TypeIndex typeIndex() const override;
  virtual std::string_view typeName() const override;
};

#define EXTERN_EXPAND_TYPE(type) extern template class DataType<type>;

#define EXTERN_EXPAND_TYPE_BOOST(r, seq) \
  EXTERN_EXPAND_TYPE(BOOST_PP_SEQ_ELEM(0, seq))

BOOST_PP_SEQ_FOR_EACH_PRODUCT(EXTERN_EXPAND_TYPE_BOOST,
                              ((i32)(i64)(u32)(u64)(f32)(f64)))
#undef EXTERN_EXPAND_TYPE
#undef EXTERN_EXPAND_TYPE_BOOST

}  // namespace Lox
