#pragma once
#include "common/visitor.h"
#include "parser/expr.h"

namespace Lox {

using AstVisitorHelper =
    VisitorHelper<void, Binary, Assign, FunctionCall, Grouping, Literal,
                  Logical, Unary, Identifier, Variable, Module, FunctionDef,
                  BlockStmt, ForStmt, IfStmt, VarExpressionStmt, ReturnStmt>;

}  // namespace Lox
