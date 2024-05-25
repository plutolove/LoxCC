#pragma once
#include "common/visitor.h"
#include "parser/expr.h"
namespace Lox {

class Interpreter : VisitorHelper<void, Binary, Unary> {
 public:
  virtual void visit(Binary* binary) override {}
  virtual void visit(Unary* ptr) override {}
};

}  // namespace Lox
