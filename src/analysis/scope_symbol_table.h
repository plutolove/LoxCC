#pragma once
#include <memory>
#include <unordered_set>

#include "analysis/symbol.h"
#include "common/maybe.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/ScopedHashTable.h"
#include "llvm/ADT/StringRef.h"

namespace Lox {

struct Scope {
  Scope(std::unique_ptr<llvm::ScopedHashTableScope<llvm::StringRef, Symbol>>
            scope,
        std::unordered_set<std::string>* ptr)
      : inner_scope(std::move(scope)), curr_scope_symbols_ref(ptr) {}

  ~Scope() { curr_scope_symbols_ref->clear(); }

  std::unique_ptr<llvm::ScopedHashTableScope<llvm::StringRef, Symbol>>
      inner_scope;
  std::unordered_set<std::string>* curr_scope_symbols_ref{};
};

using ScopePtr = std::unique_ptr<Scope>;

class ScopeSymbolTable {
 public:
  ScopeSymbolTable() {}

  ScopePtr newScope();

  Maybe<void> declar(const std::string& name, Symbol symbol);

  Maybe<Symbol> getSymbol(const std::string& name) const;

 private:
  bool hasDeclarCurrScope(const std::string& name) const {
    return curr_scope_symbols.count(name);
  }

  bool hasSymbol(const std::string& name) const {
    return symbol_table.count(name);
  }

  std::unordered_set<std::string> curr_scope_symbols;
  llvm::ScopedHashTable<llvm::StringRef, Symbol> symbol_table;
};

}  // namespace Lox
