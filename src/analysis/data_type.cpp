#include "analysis/data_type.h"

#include "analysis/type_index.h"

namespace Lox {

template <typename T>
TypeIndex DataType<T>::typeIndex() const {
  return TypeId<T>;
}

template <typename T>
std::string_view DataType<T>::typeName() const {
  return TypeName<T>;
}

#define EXPAND_TYPE(type) template class DataType<type>;

#define EXPAND_TYPE_BOOST(r, seq) EXPAND_TYPE(BOOST_PP_SEQ_ELEM(0, seq))

BOOST_PP_SEQ_FOR_EACH_PRODUCT(EXPAND_TYPE_BOOST,
                              ((i32)(i64)(u32)(u64)(f32)(f64)))
#undef EXPAND_TYPE
#undef EXPAND_TYPE_BOOST

}  // namespace Lox
