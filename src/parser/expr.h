#pragma once
#include <any>
#include <memory>

#include "common/log.h"
#include "common/visitor.h"
#include "parser/token.h"

namespace Lox {

class Expr {
 public:
  virtual ~Expr() {}
  virtual void accept(VisitorBase<void>& v) = 0;
};

using ExprPtr = std::shared_ptr<Expr>;

template <typename Derived>
class ExprVisitorHelper : public Expr {
 public:
  using Expr::Expr;

  virtual void accept(VisitorBase<>& visitor) override {
    if (auto visitor_ptr = dynamic_cast<Visitor<void, Derived>*>(&visitor)) {
      visitor_ptr->visit(static_cast<Derived*>(this));
    } else {
      FATAL("type cast failed: {} to {}", TypeName(Expr), TypeName(Derived));
      std::abort();
    }
  }
};

class Binary : public ExprVisitorHelper<Binary> {
 public:
  using ExprVisitorHelper<Binary>::ExprVisitorHelper;

  Binary(const ExprPtr& lhs, Token op, const ExprPtr& rhs)
      : lhs(lhs), op(op), rhs(rhs) {}

  ExprPtr lhs;
  Token op;
  ExprPtr rhs;
};

class Assign : public ExprVisitorHelper<Assign> {
 public:
  Assign(Token name, const ExprPtr& value) : name(name), value(value) {}

  Token name;
  ExprPtr value;
};

class Call : public ExprVisitorHelper<Call> {
 public:
  Call(const ExprPtr& callee, Token paren,
       const std::vector<ExprPtr>& arguments)
      : callee(callee), paren(paren), arguments(arguments) {}

  ExprPtr callee;
  Token paren;
  std::vector<ExprPtr> arguments;
};

class Get : public ExprVisitorHelper<Get> {
 public:
  Get(const ExprPtr& object, Token name) : object(object), name(name) {}

  ExprPtr object;
  Token name;
};

class Set : public ExprVisitorHelper<Set> {
 public:
  Set(const ExprPtr& object, Token name, const ExprPtr& value)
      : object(object), name(name), value(value) {}

  ExprPtr object;
  Token name;
  ExprPtr value;
};

class Super : public ExprVisitorHelper<Super> {
 public:
  Super(Token keyword, Token method) : keyword(keyword), method(method) {}

  Token keyword;
  Token method;
};

class This : public ExprVisitorHelper<This> {
 public:
  This(Token keyword) : keyword(keyword) {}

  Token keyword;
};

class Grouping : public ExprVisitorHelper<Grouping> {
 public:
  Grouping(const ExprPtr& expression) : expression(expression) {}
  ExprPtr expression;
};

class Literal : public ExprVisitorHelper<Literal> {
 public:
  template <typename T>
  Literal(T val, Token token) : value(val), token(token) {}

  std::any value;
  Token token;
};

class Logical : public ExprVisitorHelper<Logical> {
 public:
  Logical(const ExprPtr& lhs, Token op, const ExprPtr& rhs)
      : lhs(lhs), op(op), rhs(rhs) {}

  ExprPtr lhs;
  Token op;
  ExprPtr rhs;
};

class Unary : public ExprVisitorHelper<Unary> {
 public:
  Unary(Token op, const ExprPtr& right) : op(op), right(right) {}

  Token op;
  ExprPtr right;
};

class Variable : public ExprVisitorHelper<Variable> {
 public:
  Variable(Token name) : name(name) {}
  Token name;
};

}  // namespace Lox
