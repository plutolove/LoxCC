#include "parser/rule/prefix_parse_rule.h"

#include "boost/lexical_cast.hpp"
#include "common/auto_registration_factory.h"
#include "common/maybe.h"
#include "parser/expr.h"
#include "parser/token.h"

namespace Lox {

Maybe<Expr> GroupParseRule::parse(Parser* parser, Token token) const {
  auto ret = JUST(parser->parsePrecedence(Precedence::None));
  JUST(parser->consume(TokenType::RIGHT_PAREN, "Expect ')' after expression"));
  return ret;
}

Maybe<Expr> UnaryParseRule::parse(Parser* parser, Token token) const {
  auto expr = JUST(parser->parsePrecedence(token.precedence));
  ExprPtr ret = std::make_shared<Unary>(token, expr);
  return ret;
}

Maybe<Expr> LiteralParseRule::parse(Parser* parser, Token token) const {
  ExprPtr ret;
  if (TokenType::INT == token.type) {
    ret = std::make_shared<Literal>(boost::lexical_cast<int64_t>(token.lexeme),
                                    token);
  } else if (TokenType::FLOAT == token.type) {
    ret = std::make_shared<Literal>(boost::lexical_cast<double>(token.lexeme),
                                    token);
  } else if (TokenType::FLOAT == token.type) {
    ret = std::make_shared<Literal>(boost::lexical_cast<double>(token.lexeme),
                                    token);
  } else if (TokenType::FALSE == token.type or TokenType::TRUE == token.type) {
    ret = std::make_shared<Literal>(
        (TokenType::TRUE == token.type ? true : false), token);
  } else {
    return NewErr("Expected a Literal, got a different: {}", token.toString());
  }
  return ret;
}

Maybe<Expr> IdentifierParseRule::parse(Parser* parser, Token token) const {
  ExprPtr ret = std::make_shared<Identifier>(token, token.lexeme);
  return ret;
}

REGISTER_CLASS(TokenType, TokenType::LEFT_PAREN, PrefixRuleBase,
               GroupParseRule);

REGISTER_CLASS(TokenType, TokenType::MINUS, PrefixRuleBase, UnaryParseRule);

REGISTER_CLASS(TokenType, TokenType::BANG, PrefixRuleBase, UnaryParseRule);

REGISTER_CLASS(TokenType, TokenType::INT, PrefixRuleBase, LiteralParseRule);
REGISTER_CLASS(TokenType, TokenType::FLOAT, PrefixRuleBase, LiteralParseRule);
REGISTER_CLASS(TokenType, TokenType::STRING, PrefixRuleBase, LiteralParseRule);
REGISTER_CLASS(TokenType, TokenType::TRUE, PrefixRuleBase, LiteralParseRule);
REGISTER_CLASS(TokenType, TokenType::FALSE, PrefixRuleBase, LiteralParseRule);
REGISTER_CLASS(TokenType, TokenType::IDENTIFIER, PrefixRuleBase,
               IdentifierParseRule);

}  // namespace Lox
