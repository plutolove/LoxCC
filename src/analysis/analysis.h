#pragma once
#include "analysis/data_type_manager.h"
#include "analysis/scope_symbol_table.h"
#include "common/maybe.h"
#include "parser/expr.h"
#include "parser/visitor_helper.h"

namespace Lox {

class Analysis : public AstVisitorHelper {
 public:
  Analysis() {}

  Maybe<void> typeVisit(Expr* ptr) {
    return ptr->accept(*static_cast<VisitorBase<>*>(this));
  }

  Maybe<void> analysis(const ExprPtr& ast);

  virtual Maybe<void> visit(Binary* binary) override;
  virtual Maybe<void> visit(Assign* binary) override;
  virtual Maybe<void> visit(FunctionCall* binary) override;
  virtual Maybe<void> visit(Grouping* binary) override;
  virtual Maybe<void> visit(Literal* binary) override;
  virtual Maybe<void> visit(Logical* binary) override;
  virtual Maybe<void> visit(Unary* binary) override;
  virtual Maybe<void> visit(Identifier* binary) override;
  virtual Maybe<void> visit(Variable* binary) override;
  virtual Maybe<void> visit(Module* binary) override;
  virtual Maybe<void> visit(FunctionDef* binary) override;
  virtual Maybe<void> visit(BlockStmt* binary) override;
  virtual Maybe<void> visit(ForStmt* binary) override;
  virtual Maybe<void> visit(IfStmt* binary) override;
  virtual Maybe<void> visit(VarExpressionStmt* binary) override;
  virtual Maybe<void> visit(ReturnStmt* binary) override;

 private:
  DataTypeManager data_type_manager{};
  ScopeSymbolTable scope_symbol_table{};
};

}  // namespace Lox
