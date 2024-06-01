#include "parser/expr.h"

#include <algorithm>

namespace Lox {

Identifier::Identifier(Token token, const std::string& name)
    : token(token), name(name) {}

std::string Identifier::to_string() const { return name; }

Module::Module(const std::vector<ExprPtr>& stmts) : stmts(stmts) {}

std::string Module::to_string() const {
  std::vector<std::string> strs;
  std::for_each(stmts.begin(), stmts.end(), [&strs, this](auto&& val) {
    strs.push_back(val->to_string());
  });
  return fmt::format("{}", fmt::join(strs, "\n"));
}

BlockStmt::BlockStmt(const std::vector<ExprPtr>& stmts) : stmts(stmts) {}

std::string BlockStmt::to_string() const {
  std::vector<std::string> strs;
  std::for_each(stmts.begin(), stmts.end(), [&strs, this](auto&& val) {
    strs.push_back(val->to_string());
  });
  return fmt::format("{}", fmt::join(strs, "\n"));
}

ExpressionStmt::ExpressionStmt(Token token, const ExprPtr& stmt)
    : token(token), expr(stmt) {}

std::string ExpressionStmt::to_string() const { return expr->to_string(); }

VarExpressionStmt::VarExpressionStmt(const ExprPtr& var, const ExprPtr& expr)
    : var(var), expr(expr) {}

VarExpressionStmt::VarExpressionStmt(const ExprPtr& var) : var(var) {}

std::string VarExpressionStmt::to_string() const {
  if (expr) return fmt::format("{} = {}", var->to_string(), expr->to_string());
  return fmt::format("{}", var->to_string());
}

}  // namespace Lox
