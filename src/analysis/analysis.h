#pragma once
#include "analysis/data_type_manager.h"
#include "parser/visitor_helper.h"

namespace Lox {

class Analysis : public AstVisitorHelper {
 public:
  Analysis() {}

 private:
  DataTypeManager data_type_manager;
};

}  // namespace Lox
