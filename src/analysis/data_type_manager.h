#pragma once
#include "analysis/data_type.h"
#include "common/auto_registration_factory.h"
#include "common/maybe.h"

namespace Lox {
using DataTypeAutoRegistrationFactory =
    AutoRegistrationFactory<DataTypeBase, std::string>;

class DataTypeManager {
 public:
  Maybe<DataTypeBase> get(const std::string& name) const;
  bool push(const std::string& name, const DataTypePtr& type);

 private:
  const DataTypePtr getFromData(const std::string& name) const;

  bool hasType(const std::string& name) const {
    auto iter = data_.find(name);
    return iter != data_.end();
  }

  std::unordered_map<std::string, DataTypePtr> data_;
  DataTypeAutoRegistrationFactory& factory{
      DataTypeAutoRegistrationFactory::Get()};
};

}  // namespace Lox
