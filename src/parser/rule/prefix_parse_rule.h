#pragma once

#include "parser/expr.h"
#include "parser/pratt_parser.h"

namespace Lox {

class PrefixRuleBase {
 public:
  virtual Maybe<Expr> parse(Parser* parser, Token token) const = 0;
};

class GroupParseRule : public PrefixRuleBase {
 public:
  virtual Maybe<Expr> parse(Parser* parser, Token token) const override;
};

class UnaryParseRule : public PrefixRuleBase {
 public:
  virtual Maybe<Expr> parse(Parser* parser, Token token) const override;
};

class LiteralParseRule : public PrefixRuleBase {
 public:
  virtual Maybe<Expr> parse(Parser* parser, Token token) const override;
};

class IdentifierParseRule : public PrefixRuleBase {
 public:
  virtual Maybe<Expr> parse(Parser* parser, Token token) const override;
};

}  // namespace Lox
