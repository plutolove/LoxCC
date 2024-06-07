#pragma once
#include "analysis/data_type_manager.h"
#include "analysis/scope_symbol_table.h"
#include "parser/visitor_helper.h"

namespace Lox {

class Analysis : public AstVisitorHelper {
 public:
  Analysis() {}

 private:
  DataTypeManager data_type_manager{};
  ScopeSymbolTable scope_symbol_table{};
};

}  // namespace Lox
