#include "parser/rule/infix_parse_rule.h"

#include "common/auto_registration_factory.h"
#include "parser/expr.h"
#include "parser/token.h"

namespace Lox {

Maybe<Expr> BinaryParseRule::parse(Parser* parser, ExprPtr& lhs,
                                   Token token) const {
  INFO("binary parse: {}", token.toString());
  auto rhs = JUST(parser->parsePrecedence(token.precedence));
  ExprPtr ret = std::make_shared<Binary>(lhs, token, rhs);
  return ret;
}

Maybe<Expr> SubScriptParseRule::parse(Parser* parser, ExprPtr& lhs,
                                      Token token) const {
  INFO("-----------------------------------------");
  INFO("subscript parse: {}", token.toString());

  auto rhs = JUST(parser->parsePrecedence(Precedence::None));
  INFO("subscrit rhs: {}", rhs->to_string());
  JUST(
      parser->consume(TokenType::RIGHT_BRACKET, "Expect ']' after expression"));
  ExprPtr ret = std::make_shared<Binary>(lhs, token, rhs);
  return ret;
}

REGISTER_CLASS(TokenType, TokenType::MINUS, InfixRuleBase, BinaryParseRule);
REGISTER_CLASS(TokenType, TokenType::PLUS, InfixRuleBase, BinaryParseRule);
REGISTER_CLASS(TokenType, TokenType::STAR, InfixRuleBase, BinaryParseRule);
REGISTER_CLASS(TokenType, TokenType::SLASH, InfixRuleBase, BinaryParseRule);
REGISTER_CLASS(TokenType, TokenType::GREATER, InfixRuleBase, BinaryParseRule);
REGISTER_CLASS(TokenType, TokenType::GREATER_EQUAL, InfixRuleBase,
               BinaryParseRule);
REGISTER_CLASS(TokenType, TokenType::LESS, InfixRuleBase, BinaryParseRule);
REGISTER_CLASS(TokenType, TokenType::LESS_EQUAL, InfixRuleBase,
               BinaryParseRule);

REGISTER_CLASS(TokenType, TokenType::BANG_EQUAL, InfixRuleBase,
               BinaryParseRule);
REGISTER_CLASS(TokenType, TokenType::EQUAL_EQUAL, InfixRuleBase,
               BinaryParseRule);

REGISTER_CLASS(TokenType, TokenType::LEFT_BRACKET, InfixRuleBase,
               SubScriptParseRule);

}  // namespace Lox
