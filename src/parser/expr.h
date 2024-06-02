#pragma once
#include <any>
#include <memory>
#include <string>

#include "analysis/data_type.h"
#include "common/log.h"
#include "common/visitor.h"
#include "parser/token.h"

namespace Lox {

class Expr {
 public:
  virtual ~Expr() {}
  virtual Maybe<void> accept(VisitorBase<void>& v) = 0;
  virtual std::string to_string() const = 0;

  DataTypePtr type;
};

using ExprPtr = std::shared_ptr<Expr>;

template <typename Derived>
class ExprVisitorHelper : public Expr {
 public:
  using Expr::Expr;

  virtual Maybe<void> accept(VisitorBase<>& visitor) override {
    if (auto visitor_ptr = dynamic_cast<Visitor<void, Derived>*>(&visitor)) {
      return JUST(visitor_ptr->visit(static_cast<Derived*>(this)));
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
  Assign(const ExprPtr& lhs, Token token, const ExprPtr& value)
      : name(lhs), token(token), value(value) {}

  virtual std::string to_string() const override {
    return fmt::format("({} := {})", name->to_string(), value->to_string());
  }

  ExprPtr name;
  Token token;
  ExprPtr value;
};

class FunctionCall : public ExprVisitorHelper<FunctionCall> {
 public:
  FunctionCall(const ExprPtr& callee, Token paren,
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

class Identifier : public ExprVisitorHelper<Identifier> {
 public:
  Identifier(Token token, const std::string& name);
  virtual std::string to_string() const override;

  Token token;
  std::string name;
};

class Variable : public ExprVisitorHelper<Variable> {
 public:
  Variable(const std::string& name, const std::string& type)
      : name(name), type(type) {}

  virtual std::string to_string() const override {
    return fmt::format("var {}:{}", name, type);
  }

  std::string name;
  std::string type;
};

// 保存整个编译文件
class Module : public ExprVisitorHelper<Module> {
 public:
  Module(const std::vector<ExprPtr>& stmts);
  Module() {}
  void add(const ExprPtr& stmt) { stmts.push_back(stmt); }

  virtual std::string to_string() const override;

  std::vector<ExprPtr> stmts;
};

class FunctionDef : public ExprVisitorHelper<FunctionDef> {
 public:
  FunctionDef(const std::string& name, const std::vector<ExprPtr>& params,
              const std::string& return_type, const ExprPtr& body)
      : function_name(name),
        params(params),
        return_type(return_type),
        body(body) {}

  virtual std::string to_string() const override {
    std::vector<std::string> arg;

    std::transform(params.begin(), params.end(), std::back_inserter(arg),
                   [](auto&& expr) { return expr->to_string(); });

    return fmt::format("fn {}({}) -> {} {{ \n{}\n }}", function_name,
                       fmt::join(arg, ", "), return_type, body->to_string());
  }

  std::string function_name;
  std::vector<ExprPtr> params;
  std::string return_type;
  ExprPtr body;
};

class BlockStmt : public ExprVisitorHelper<BlockStmt> {
 public:
  BlockStmt(const std::vector<ExprPtr>& stmts);
  virtual std::string to_string() const override;

  std::vector<ExprPtr> stmts;
};

class ForStmt : public ExprVisitorHelper<ForStmt> {
 public:
  ForStmt(const ExprPtr& init, const ExprPtr& cond, const ExprPtr& update,
          const ExprPtr& body)
      : init(init), cond(cond), update(update), body(body) {}
  virtual std::string to_string() const override {
    return fmt::format("for ({}; {}; {}) {{\n{}\n}}", init->to_string(),
                       cond->to_string(), update->to_string(),
                       body->to_string());
  }
  ExprPtr init;
  ExprPtr cond;
  ExprPtr update;
  ExprPtr body;
};

class IfStmt : public ExprVisitorHelper<IfStmt> {
 public:
  IfStmt(const ExprPtr& cond, const ExprPtr& if_block,
         const ExprPtr& else_block)
      : cond(cond), if_block(if_block), else_block(else_block) {}

  virtual std::string to_string() const override {
    if (else_block) {
      return fmt::format("if ({}) {{ \n{}\n }} else {{\n{}\n}}",
                         cond->to_string(), if_block->to_string(),
                         else_block->to_string());
    } else {
      return fmt::format("if ({}) {{ \n{}\n }}", cond->to_string(),
                         if_block->to_string());
    }
  }
  ExprPtr cond;
  ExprPtr if_block;
  ExprPtr else_block;
};

class ExpressionStmt : public ExprVisitorHelper<ExpressionStmt> {
 public:
  ExpressionStmt(Token token, const ExprPtr& stmt);
  virtual std::string to_string() const override;

  Token token;
  ExprPtr expr;
};

class VarExpressionStmt : public ExprVisitorHelper<ExpressionStmt> {
 public:
  VarExpressionStmt(const ExprPtr& var, const ExprPtr& stmt);
  VarExpressionStmt(const ExprPtr& var);

  virtual std::string to_string() const override;

  ExprPtr var;
  ExprPtr expr;
};

class ReturnStmt : public ExprVisitorHelper<ReturnStmt> {
 public:
  ReturnStmt() {}
  ReturnStmt(const ExprPtr& expr) : expr(expr) {}
  virtual std::string to_string() const override {
    if (expr) {
      return fmt::format("return {}", expr->to_string());
    } else {
      return "return";
    }
  }
  ExprPtr expr;
};

}  // namespace Lox
