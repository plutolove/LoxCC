#pragma once
#include "parser/expr.h"
#include "parser/pratt_parser.h"

namespace Lox {

class InfixRuleBase {
 public:
  virtual Maybe<Expr> parse(Parser* parser, ExprPtr& lhs,
                            Token token) const = 0;
};

class BinaryParseRule : public InfixRuleBase {
 public:
  virtual Maybe<Expr> parse(Parser* parser, ExprPtr& lhs,
                            Token token) const override;
};

class SubScriptParseRule : public InfixRuleBase {
 public:
  virtual Maybe<Expr> parse(Parser* parser, ExprPtr& lhs,
                            Token token) const override;
};

}  // namespace Lox
