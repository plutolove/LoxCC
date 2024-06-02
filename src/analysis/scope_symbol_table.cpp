#include "analysis/scope_symbol_table.h"

#include "common/maybe.h"

namespace Lox {

ScopePtr ScopeSymbolTable::newScope() {
  auto scope =
      std::make_unique<llvm::ScopedHashTableScope<llvm::StringRef, Symbol>>(
          symbol_table);
  return std::make_unique<Scope>(std::move(scope), &curr_scope_symbols);
}

Maybe<void> ScopeSymbolTable::declar(const std::string& name, Symbol symbol) {
  if (hasDeclar(name)) {
    return NewErr("Repeated definition of var: {}", name);
  }

  curr_scope_symbols.insert(name);
  symbol_table.insert(name, symbol);
  return true;
}

}  // namespace Lox