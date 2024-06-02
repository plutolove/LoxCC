#pragma once

#include <initializer_list>
#include <vector>

#include "analysis/data_type.h"
#include "parser/expr.h"

namespace Lox {

enum class SymbolType {
  None,
  Variable,
  Function,
};

struct Symbol {
  Symbol(const std::string& name, const DataTypePtr& type)
      : symbol_type(SymbolType::Variable), name(name), type(type) {}

  Symbol(const std::string& name, const DataTypePtr& type,
         const std::vector<DataTypePtr>& args_type)
      : symbol_type(SymbolType::Function),
        name(name),
        type(type),
        args_type(args_type) {}

  Symbol(const std::string& name, const DataTypePtr& type,
         const std::initializer_list<DataTypePtr>& args_type)
      : symbol_type(SymbolType::Function),
        name(name),
        type(type),
        args_type(args_type) {}

  SymbolType symbol_type;
  std::string name;
  DataTypePtr type;
  std::vector<DataTypePtr> args_type;
};

using SymbolPtr = std::shared_ptr<Symbol>;

}  // namespace Lox
