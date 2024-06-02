#pragma once
#include <string_view>

#include "boost/preprocessor.hpp"
#include "common/type_define.h"

namespace Lox {

enum class TypeIndex : int32_t { none = 0, i32, i64, u32, u64, f32, f64 };

template <typename T>
constexpr auto TypeId = TypeIndex::none;

template <typename T>
constexpr std::string_view TypeName = "none";

#define EXPAND_TYPE(type)                        \
  template <>                                    \
  constexpr auto TypeId<type> = TypeIndex::type; \
  template <>                                    \
  constexpr std::string_view TypeName<type> = BOOST_PP_STRINGIZE(type);

#define EXPAND_TYPE_BOOST(r, seq) EXPAND_TYPE(BOOST_PP_SEQ_ELEM(0, seq))

BOOST_PP_SEQ_FOR_EACH_PRODUCT(EXPAND_TYPE_BOOST,
                              ((i32)(i64)(u32)(u64)(f32)(f64)))

#undef EXPAND_TYPE
#undef EXPAND_TYPE_BOOST
}  // namespace Lox
