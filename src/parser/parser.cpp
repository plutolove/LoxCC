#include "parser/parser.h"

#include <memory>

#include "boost/lexical_cast.hpp"
#include "common/type_trait.h"
#include "parser/expr.h"

namespace Lox {

Maybe<Expr> Parser::expression() {
  return equality();
  // return assignment();
}
Maybe<Expr> Parser::assignment() {
  auto expr = JUST(or_expr());

  if (match(TokenType::EQUAL)) {
    Token equals = previous();
    auto value = JUST(assignment());

    if (InstanceOf<Variable>(expr)) {
      Token name = std::static_pointer_cast<Variable>(expr)->name;
      ExprPtr ret = std::make_shared<Assign>(name, value);
      return ret;
    } else if (InstanceOf<Get>(expr)) {
      auto get = std::static_pointer_cast<Get>(expr);
      ExprPtr ret = std::make_shared<Set>(get->object, get->name, value);
      return ret;
    }

    NewErr("Invalid assignment target. {}", equals.toString());
  }

  return expr;
}

Maybe<Expr> Parser::and_expr() { return NewErr(); }

Maybe<Expr> Parser::or_expr() {
  auto expr = JUST(and_expr());

  while (match(TokenType::OR)) {
    Token op = previous();
    auto rhs = JUST(and_expr());
    expr = std::make_shared<Logical>(expr, op, rhs);
  }

  return expr;
}

Maybe<Expr> Parser::equality() {
  auto expr = JUST(comparison());

  while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
    Token op = previous();
    auto right = JUST(comparison());
    expr = std::make_shared<Binary>(expr, op, right);
  }

  return expr;
}

Maybe<Expr> Parser::comparison() {
  auto expr = JUST(term());

  while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                TokenType::LESS_EQUAL})) {
    Token op = previous();
    auto right = JUST(term());
    expr = std::make_shared<Binary>(expr, op, right);
  }

  return expr;
}

Maybe<Expr> Parser::term() {
  auto expr = JUST(factor());

  while (match({TokenType::MINUS, TokenType::PLUS})) {
    Token op = previous();
    auto right = JUST(factor());
    expr = std::make_shared<Binary>(expr, op, right);
  }

  return expr;
}

Maybe<Expr> Parser::factor() {
  auto expr = JUST(unary());

  while (match({TokenType::SLASH, TokenType::STAR})) {
    Token op = previous();
    auto right = JUST(unary());
    expr = std::make_shared<Binary>(expr, op, right);
  }

  return expr;
}

Maybe<Expr> Parser::unary() {
  if (match({TokenType::BANG, TokenType::MINUS})) {
    Token op = previous();
    auto right = JUST(unary());
    ExprPtr ret = std::make_shared<Unary>(op, right);
    return ret;
  }

  return call();
}

Maybe<Expr> Parser::finishCall(ExprPtr callee) {
  std::vector<ExprPtr> arguments;
  if (!check(TokenType::RIGHT_PAREN)) {
    do {
      //> check-max-arity
      if (arguments.size() >= 255) {
        return NewErr("Can't have more than 255 arguments.")
      }
      //< check-max-arity
      auto arg = JUST(expression());
      arguments.push_back(arg);
    } while (match(TokenType::COMMA));
  }

  auto paren =
      JUST(consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments."));

  ExprPtr ret = std::make_shared<Call>(callee, *paren, arguments);
  return ret;
}

Maybe<Expr> Parser::call() {
  auto expr = JUST(primary());
  while (true) {  // [while-true]
    if (match(TokenType::LEFT_PAREN)) {
      expr = JUST(finishCall(expr));
    } else if (match(TokenType::DOT)) {
      Token name = *JUST(
          consume(TokenType::IDENTIFIER, "Expect property name after '.'."));

      expr = std::make_shared<Get>(expr, name);
    } else {
      break;
    }
  }
  return expr;
}

Maybe<Expr> Parser::primary() {
  auto cur_token = peek();
  if (match(TokenType::FALSE)) {
    ExprPtr ret = std::make_shared<Literal>(false, cur_token);
    return ret;
  }
  if (match(TokenType::TRUE)) {
    ExprPtr ret = std::make_shared<Literal>(true, cur_token);
    return ret;
  }
  if (match(TokenType::NIL)) {
    ExprPtr ret = std::make_shared<Literal>(0, cur_token);
    return ret;
  }

  if (match(TokenType::INT)) {
    ExprPtr ret = std::make_shared<Literal>(
        boost::lexical_cast<int64_t>(cur_token.lexeme), cur_token);
    return ret;
  }

  if (match(TokenType::FLOAT)) {
    ExprPtr ret = std::make_shared<Literal>(
        boost::lexical_cast<double>(cur_token.lexeme), cur_token);
    return ret;
  }

  if (match(TokenType::STRING)) {
    ExprPtr ret = std::make_shared<Literal>(cur_token.lexeme, cur_token);
    return ret;
  }

  if (match(TokenType::SUPER)) {
    Token keyword = previous();
    JUST(consume(TokenType::DOT, "Expect '.' after 'super'."));
    auto method =
        JUST(consume(TokenType::IDENTIFIER, "Expect superclass method name."));
    ExprPtr ret = std::make_shared<Super>(keyword, *method);
    return ret;
  }

  if (match(TokenType::THIS)) {
    ExprPtr ret = std::make_shared<This>(previous());
    return ret;
  }

  if (match(TokenType::IDENTIFIER)) {
    ExprPtr ret = std::make_shared<Variable>(previous());
    return ret;
  }

  if (match(TokenType::LEFT_PAREN)) {
    auto expr = JUST(expression());
    JUST(consume(TokenType::RIGHT_PAREN, "Expect ')' after expression."));
    ExprPtr ret = std::make_shared<Grouping>(expr);
    return ret;
  }

  return NewErr("Expect expression. get {}", peek().toString());
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
