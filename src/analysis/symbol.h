#pragma once

#include "analysis/data_type.h"
#include "parser/expr.h"

namespace Lox {

enum class SymbolType {
  None,
  Variable,
  Function,
};

struct Symbol {
  SymbolType symbol_type;
  std::string name;
  DataTypePtr type;
};

}  // namespace Lox
