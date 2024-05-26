#pragma once
#include <initializer_list>

#include "common/maybe.h"
#include "parser/expr.h"
#include "parser/token.h"

namespace Lox {

class GroupParseRule;
class UnaryParseRule;
class LiteralParseRule;
class BinaryParseRule;
class SubScriptParseRule;

class Parser {
 public:
  Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

  Maybe<Expr> parsePrecedence(Precedence prece);

  Maybe<Expr> parse() { return expression(); }

  // friend class
  friend class GroupParseRule;
  friend class UnaryParseRule;
  friend class LiteralParseRule;
  friend class BinaryParseRule;
  friend class SubScriptParseRule;

 private:
  Maybe<Expr> expression();

  Token previous() { return tokens[current - 1]; }
  Token peek() { return tokens[current]; }

  bool isAtEnd() { return peek().type == TokenType::END; }

  Token advance() {
    if (!isAtEnd()) current++;
    return previous();
  }

  bool match(TokenType type) {
    if (check(type)) {
      advance();
      return true;
    }
    return false;
  }

  bool match(std::initializer_list<TokenType> types) {
    for (auto&& type : types) {
      if (check(type)) {
        advance();
        return true;
      }
    }
    return false;
  }

  bool check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
  }

  Maybe<Token> consume(TokenType type, std::string message) {
    if (check(type)) return std::make_shared<Token>(advance());

    return NewErr("token: {}, msg: {}", peek().toString(), message);
  }

  void synchronize();

  std::vector<Token> tokens;
  int current{0};
};

}  // namespace Lox
