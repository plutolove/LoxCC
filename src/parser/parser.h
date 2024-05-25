#pragma once
#include <initializer_list>

#include "common/maybe.h"
#include "parser/expr.h"
#include "parser/token.h"

namespace Lox {

class Parser {
 public:
  Parser(const std::vector<Token>& tokens) : tokens(tokens) {}
  Maybe<Expr> parse() { return expression(); }

 private:
  Token previous() { return tokens[current - 1]; }
  Token peek() { return tokens[current]; }

  Maybe<Expr> expression();
  Maybe<Expr> assignment();
  Maybe<Expr> primary();
  Maybe<Expr> or_expr();
  Maybe<Expr> and_expr();
  Maybe<Expr> call();
  Maybe<Expr> finishCall(ExprPtr callee);
  Maybe<Expr> unary();
  Maybe<Expr> factor();
  Maybe<Expr> term();
  Maybe<Expr> comparison();
  Maybe<Expr> equality();

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
