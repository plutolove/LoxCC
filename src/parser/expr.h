#pragma once
#include <any>
#include <memory>
#include <string>

#include "common/log.h"
#include "common/visitor.h"
#include "parser/token.h"

namespace Lox {

class Expr {
 public:
  virtual ~Expr() {}
  virtual void accept(VisitorBase<void>& v) = 0;
  virtual std::string to_string() const = 0;
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

  virtual std::string to_string() const override {
    if (TokenType::LEFT_BRACKET == op.type) {
      return fmt::format("([] {} {})", lhs->to_string(), rhs->to_string());
    }
    return fmt::format("({} {} {})", op.lexeme, lhs->to_string(),
                       rhs->to_string());
  }

  ExprPtr lhs;
  Token op;
  ExprPtr rhs;
};

class Assign : public ExprVisitorHelper<Assign> {
 public:
  Assign(Token name, const ExprPtr& value) : name(name), value(value) {}

  virtual std::string to_string() const override {
    return fmt::format("({} {})", name.lexeme, value->to_string());
  }

  Token name;
  ExprPtr value;
};

class Call : public ExprVisitorHelper<Call> {
 public:
  Call(const ExprPtr& callee, Token paren,
       const std::vector<ExprPtr>& arguments)
      : callee(callee), paren(paren), arguments(arguments) {}

  virtual std::string to_string() const override {
    auto func_name = callee->to_string();
    std::vector<std::string> arg_str;
    arg_str.reserve(arguments.size());
    for (auto&& arg : arguments) {
      arg_str.push_back(arg->to_string());
    }
    return fmt::format("{}({})", func_name, fmt::join(arg_str, ", "));
  }

  ExprPtr callee;
  Token paren;
  std::vector<ExprPtr> arguments;
};

class Get : public ExprVisitorHelper<Get> {
 public:
  Get(const ExprPtr& object, Token name) : object(object), name(name) {}

  virtual std::string to_string() const override { return "get"; }

  ExprPtr object;
  Token name;
};

class Set : public ExprVisitorHelper<Set> {
 public:
  Set(const ExprPtr& object, Token name, const ExprPtr& value)
      : object(object), name(name), value(value) {}

  virtual std::string to_string() const override { return "set"; }

  ExprPtr object;
  Token name;
  ExprPtr value;
};

class Grouping : public ExprVisitorHelper<Grouping> {
 public:
  Grouping(const ExprPtr& expression) : expression(expression) {}

  virtual std::string to_string() const override {
    return fmt::format("({})", expression->to_string());
  }

  ExprPtr expression;
};

class Literal : public ExprVisitorHelper<Literal> {
 public:
  template <typename T>
  Literal(T val, Token token) : value(val), token(token) {}

  virtual std::string to_string() const override {
    if (token.type == TokenType::INT) {
      return fmt::format("{}", std::any_cast<int64_t>(value));
    } else if (TokenType::FLOAT == token.type) {
      return fmt::format("{}", std::any_cast<double>(value));
    } else {
      return fmt::format("{}", std::any_cast<std::string>(value));
    }
  }

  std::any value;
  Token token;
};

class Logical : public ExprVisitorHelper<Logical> {
 public:
  Logical(const ExprPtr& lhs, Token op, const ExprPtr& rhs)
      : lhs(lhs), op(op), rhs(rhs) {}

  virtual std::string to_string() const override {
    return fmt::format("({} {} {})", op.lexeme, lhs->to_string(),
                       rhs->to_string());
  }

  ExprPtr lhs;
  Token op;
  ExprPtr rhs;
};

class Unary : public ExprVisitorHelper<Unary> {
 public:
  Unary(Token op, const ExprPtr& right) : op(op), right(right) {}

  virtual std::string to_string() const override {
    return fmt::format("({} {})", op.lexeme, right->to_string());
  }

  Token op;
  ExprPtr right;
};

class Variable : public ExprVisitorHelper<Variable> {
 public:
  Variable(Token name) : name(name) {}

  virtual std::string to_string() const override {
    return fmt::format("{}", name.lexeme);
  }

  Token name;
};

}  // namespace Lox
