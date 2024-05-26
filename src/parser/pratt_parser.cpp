#include "parser/pratt_parser.h"

#include "common/auto_registration_factory.h"
#include "common/maybe.h"
#include "parser/rule/infix_parse_rule.h"
#include "parser/rule/prefix_parse_rule.h"
#include "parser/token.h"

namespace Lox {

Maybe<Expr> Parser::expression() { return parsePrecedence(Precedence::Assig); }

Maybe<Expr> Parser::parsePrecedence(Precedence prece) {
  auto token = advance();
  auto rule = GET_CLASS(PrefixRuleBase, TokenType, token.type);
  if (not rule) {
    return NewErr("Expect expression, get {}", token.toString());
  }
  auto lhs = JUST(rule->parse(this, token));
  WARN("lhs str: {}", lhs->to_string());
  auto right_prece = peek().precedence;

  while (prece < right_prece and not isAtEnd()) {
    auto token = advance();
    auto rule = GET_CLASS(InfixRuleBase, TokenType, token.type);
    if (not rule) {
      return NewErr("need binary operator: {}", token.toString());
    }
    lhs = JUST(rule->parse(this, lhs, token));
    WARN("-lhs: {}", lhs->to_string());
    right_prece = peek().precedence;
    INFO("before: {}, after: {}", token.toString(), peek().toString());
  }

  return lhs;
}

void Parser::synchronize() {
  advance();

  while (!isAtEnd()) {
    if (previous().type == TokenType::SEMICOLON) return;

    switch (peek().type) {
      case TokenType::CLASS:
      case TokenType::FUN:
      case TokenType::VAR:
      case TokenType::FOR:
      case TokenType::IF:
      case TokenType::WHILE:
      case TokenType::PRINT:
      case TokenType::RETURN:
        return;
    }

    advance();
  }
}
}  // namespace Lox
