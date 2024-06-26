#include "analysis/data_type_manager.h"

#include "analysis/data_type.h"
#include "boost/config/helper_macros.hpp"

namespace Lox {

#define REGISTER_TYPE(type)                                            \
  static DataTypeAutoRegistrationFactory::RegisterType<DataType<type>> \
      REGISTER_VAR_NAME(BOOST_STRINGIZE(type));

#define REGISTER_TYPE_BOOST(r, seq) REGISTER_TYPE(BOOST_PP_SEQ_ELEM(0, seq))

BOOST_PP_SEQ_FOR_EACH_PRODUCT(REGISTER_TYPE_BOOST,
                              ((i32)(i64)(u32)(u64)(f32)(f64)));

Maybe<DataTypeBase> DataTypeManager::get(const std::string& name) const {
  auto obj = basic_type_.getShared(name);
  if (obj) {
    return obj;
  }

  auto type = getFromData(name);
  if (type) return type;

  return NewErr("not found type: {}", name);
}

bool DataTypeManager::push(const std::string& name, const DataTypePtr& type) {
  if (basic_type_.has_obj(name)) return false;
  if (hasType(name)) return false;
  data_.emplace(name, type);
  return true;
}

const DataTypePtr DataTypeManager::getFromData(const std::string& name) const {
  auto iter = data_.find(name);
  if (iter == data_.end()) return nullptr;
  return iter->second;
}

}  // namespace Lox
