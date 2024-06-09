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
  Symbol() {}
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

  Symbol(const Symbol&) = default;
  Symbol(Symbol&&) = default;

  bool isFunction() const { return SymbolType::Function == symbol_type; }
  bool isVariable() const { return SymbolType::Variable == symbol_type; }

  std::string to_string() const {
    if (isFunction()) {
      std::vector<std::string> args_str;
      std::for_each(args_type.begin(), args_type.end(),
                    [&args_str](auto&& arg) {
                      args_str.push_back(std::string(arg->typeName()));
                    });

      return fmt::format("{}({}) -> {}", name, fmt::join(args_str, ", "),
                         type->typeName());
    } else {
      return fmt::format("var {}:{}", name, type->typeName());
    }
  }

  SymbolType symbol_type;
  std::string name;
  DataTypePtr type;
  std::vector<DataTypePtr> args_type;
};

using SymbolPtr = std::shared_ptr<Symbol>;

}  // namespace Lox
