#include "analysis/data_type.h"

#include <type_traits>

#include "analysis/type_index.h"

namespace Lox {
template <typename T>
DataType<T>::DataType() : DataTypeBase(sizeof(T)) {}

template <typename T>
TypeIndex DataType<T>::typeIndex() const {
  return TypeId<T>;
}

template <typename T>
std::string_view DataType<T>::typeName() const {
  return TypeName<T>;
}

template <typename T>
bool DataType<T>::isSigned() const {
  return std::is_signed_v<T>;
}

#define EXPAND_TYPE(type) template class DataType<type>;

#define EXPAND_TYPE_BOOST(r, seq) EXPAND_TYPE(BOOST_PP_SEQ_ELEM(0, seq))

BOOST_PP_SEQ_FOR_EACH_PRODUCT(EXPAND_TYPE_BOOST,
                              ((i32)(i64)(u32)(u64)(f32)(f64)(bool)));

}  // namespace Lox
