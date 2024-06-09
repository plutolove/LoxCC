#include "analysis/scope_symbol_table.h"

#include "common/maybe.h"

namespace Lox {

ScopePtr ScopeSymbolTable::newScope() {
  return std::make_unique<Scope>(
      std::make_unique<llvm::ScopedHashTableScope<llvm::StringRef, Symbol>>(
          symbol_table),
      &curr_scope_symbols);
}

Maybe<void> ScopeSymbolTable::declar(const std::string& name, Symbol symbol) {
  if (hasDeclarCurrScope(name)) {
    return NewErr("Repeated definition of var: {}", name);
  }

  curr_scope_symbols.insert(name);
  symbol_table.insert(name, symbol);

  return true;
}

Maybe<Symbol> ScopeSymbolTable::getSymbol(const std::string& name) const {
  if (hasSymbol(name)) return symbol_table.lookup(name);
  return NewErr("symbol not found: {}", name);
}

}  // namespace Lox
