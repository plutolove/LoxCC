#include "analysis/analysis.h"

#include "analysis/data_type.h"
#include "analysis/symbol.h"
#include "common/maybe.h"
#include "common/type_trait.h"
#include "parser/expr.h"

namespace Lox {

Maybe<void> Analysis::analysis(const ExprPtr& ast) {
  auto var = JUST(typeVisit(ast.get()));
  return true;
}

Maybe<void> Analysis::visit(Binary* binary) {
  JUST(typeVisit(binary->lhs.get()));
  JUST(typeVisit(binary->rhs.get()));
  binary->type = commonType(binary->lhs->type, binary->rhs->type);
  return true;
}

Maybe<void> Analysis::visit(Assign* assig) {
  JUST(typeVisit(assig->name.get()));
  JUST(typeVisit(assig->value.get()));

  if (not InstanceOf<Identifier>(assig->name)) {
    return NewErr("unknow variable name: {}", assig->name->to_string());
  }
  auto name = typeid_cast<const Identifier*>(assig->name.get());
  auto symbol = JUST(scope_symbol_table.getSymbol(name->name));

  if (not symbol->isVariable())
    return NewErr("symbol is not variable: {}", name->to_string());

  if (not isSameType(symbol->type, assig->value->type)) {
    return NewErr("assig expr type not same: {} {}", symbol->type->typeName(),
                  assig->value->type->typeName());
  }
  return true;
}

Maybe<void> Analysis::visit(FunctionCall* call) {
  if (not InstanceOf<Identifier>(call->callee)) {
    return NewErr("unknow function name: {}", call->callee->to_string());
  }
  for (auto&& arg : call->arguments) {
    JUST(typeVisit(arg.get()));
  }
  auto callee = typeid_cast<const Identifier*>(call->callee.get());
  auto symbol = JUST(scope_symbol_table.getSymbol(callee->name));

  if (not symbol->isFunction()) {
    return NewErr("symbol {} is not function", callee->name);
  }

  if (symbol->args_type.size() != call->arguments.size()) {
    return NewErr("function arg not same with declar, {} : {}",
                  symbol->to_string(), call->to_string());
  }
  return true;
}

Maybe<void> Analysis::visit(Grouping* group) {
  JUST(typeVisit(group->expression.get()));
  group->type = group->expression->type;
  return true;
}

Maybe<void> Analysis::visit(Literal* constant) {
  if (constant->isFloat()) {
    constant->type = JUST(data_type_manager.get("f64"));
  } else if (constant->isInt()) {
    constant->type = JUST(data_type_manager.get("i64"));
  } else {
    return NewErr("not support type: {}", constant->to_string());
  }
  return true;
}

Maybe<void> Analysis::visit(Logical* binary) {
  JUST(typeVisit(binary->lhs.get()));
  JUST(typeVisit(binary->rhs.get()));
  binary->type = commonType(binary->lhs->type, binary->rhs->type);
  return true;
}

Maybe<void> Analysis::visit(Unary* binary) {
  JUST(typeVisit(binary->right.get()));
  binary->type = binary->right->type;
  return true;
}

Maybe<void> Analysis::visit(Identifier* ident) {
  auto symbol = JUST(scope_symbol_table.getSymbol(ident->name));
  ident->type = symbol->type;
  return true;
}

Maybe<void> Analysis::visit(Variable* var) {
  auto dt = JUST(data_type_manager.get(var->type_name));
  var->type = dt;
  Symbol symbol{var->name, dt};
  scope_symbol_table.declar(var->name, symbol);

  return true;
}

Maybe<void> Analysis::visit(Module* module_) {
  for (auto&& func : module_->stmts) {
    JUST(typeVisit(func.get()));
  }
  return true;
}

Maybe<void> Analysis::visit(FunctionDef* func) {
  {
    auto scope = scope_symbol_table.newScope();
    for (auto& arg : func->params) {
      JUST(typeVisit(arg.get()));
    }

    for (auto& stmt : func->stmts) {
      JUST(typeVisit(stmt.get()));
    }
  }
  auto ret_type = JUST(data_type_manager.get(func->return_type));
  auto args_type = func->getArgsType();
  Symbol symbol{func->function_name, ret_type, args_type};
  JUST(scope_symbol_table.declar(func->function_name, symbol));
  func->type = ret_type;
  return true;
}

Maybe<void> Analysis::visit(BlockStmt* block) {
  auto scope = scope_symbol_table.newScope();
  for (auto&& expr : block->stmts) {
    JUST(typeVisit(expr.get()));
  }
  return true;
}

Maybe<void> Analysis::visit(ForStmt* for_stmt) {
  JUST(typeVisit(for_stmt->cond.get()));
  JUST(typeVisit(for_stmt->init.get()));
  JUST(typeVisit(for_stmt->update.get()));
  JUST(typeVisit(for_stmt->body.get()));
  return true;
}

Maybe<void> Analysis::visit(IfStmt* if_stmt) {
  JUST(typeVisit(if_stmt->cond.get()));
  JUST(typeVisit(if_stmt->if_block.get()));
  JUST(typeVisit(if_stmt->else_block.get()));
  return true;
}

Maybe<void> Analysis::visit(VarExpressionStmt* var_stmt) {
  JUST(typeVisit(var_stmt->var.get()));
  if (var_stmt->expr) JUST(typeVisit(var_stmt->expr.get()));
  if (not isSameType(var_stmt->var->type, var_stmt->expr->type)) {
    return NewErr("var type name not same, {}", var_stmt->to_string());
  }
  return true;
}

Maybe<void> Analysis::visit(ReturnStmt* ret) { return true; }

}  // namespace Lox
