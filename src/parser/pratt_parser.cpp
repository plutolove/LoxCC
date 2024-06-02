#include "parser/pratt_parser.h"

#include <memory>

#include "common/auto_registration_factory.h"
#include "common/log.h"
#include "common/maybe.h"
#include "parser/expr.h"
#include "parser/rule/infix_parse_rule.h"
#include "parser/rule/prefix_parse_rule.h"
#include "parser/token.h"

namespace Lox {

Maybe<Expr> Parser::parse() {
  ExprPtr ret;
  auto ast_module = std::make_shared<Module>();
  while (not isAtEnd()) {
    auto stmt = JUST(parseFunction());
    ast_module->add(stmt);
  }
  ret = ast_module;
  return ret;
}

Maybe<std::vector<ExprPtr>> Parser::Params() {
  std::vector<ExprPtr> params;
  JUST(consume(TokenType::LEFT_PAREN, "Expect '(' after function name"));
  if (not check(TokenType::RIGHT_PAREN)) {
    do {
      auto var_name =
          JUST(consume(TokenType::IDENTIFIER, "Expect identifier var name"));
      JUST(consume(TokenType::COLON, "Expect : , like var_name : type"));
      auto var_type =
          JUST(consume(TokenType::IDENTIFIER, "Expect identifier var type"));
      ExprPtr param =
          std::make_shared<Variable>(var_name->lexeme, var_type->lexeme);
      params.push_back(param);
    } while (match(TokenType::COMMA));
  }
  JUST(consume(TokenType::RIGHT_PAREN, "Expect ')' after function args"));
  return params;
}

Maybe<Expr> Parser::parseVarStmt() {
  ExprPtr ret;
  auto var_name = JUST(consume(TokenType::IDENTIFIER, "Expect variable name"));
  JUST(consume(TokenType::COLON, "Expect ':' after variable name"));
  auto type = JUST(consume(TokenType::IDENTIFIER, "Expect variable type"));
  ExprPtr var = std::make_shared<Variable>(var_name->lexeme, type->lexeme);

  if (match(TokenType::EQUAL)) {
    auto init = JUST(expression());
    ret = std::make_shared<VarExpressionStmt>(var, init);
  } else {
    ret = std::make_shared<VarExpressionStmt>(var);
  }
  JUST(consume(TokenType::SEMICOLON, "Expect ';' after stmt"));
  return ret;
}

Maybe<Expr> Parser::parseReturnStmt() {
  if (check(TokenType::SEMICOLON)) {
    ExprPtr ret = std::make_shared<ReturnStmt>();
    return ret;
  }
  auto expr = JUST(expression());
  ExprPtr ret = std::make_shared<ReturnStmt>(expr);
  JUST(consume(TokenType::SEMICOLON, "Expect ';' after expression"));
  return ret;
}

Maybe<Expr> Parser::parseExprStmt() {
  ExprPtr ret = JUST(expression());
  JUST(consume(TokenType::SEMICOLON, "Expect ';' after expression"));
  return ret;
}

Maybe<Expr> Parser::parseIfStmt() {
  JUST(consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'"));
  auto cond = JUST(expression());
  JUST(consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition"));
  auto if_block = JUST(parseStmts());
  ExprPtr else_block;
  if (match(TokenType::ELSE)) {
    else_block = JUST(parseStmts());
  }
  ExprPtr ret = std::make_shared<IfStmt>(cond, if_block, else_block);
  return ret;
}

Maybe<Expr> Parser::parseStmts() {
  // if (match(TOKEN_FOR)) {
  //    forStatement();
  //  } else if (match(TOKEN_IF)) {
  //    ifStatement();
  //  } else if (match(TOKEN_RETURN)) {
  //    returnStatement();
  //  } else if (match(TOKEN_WHILE)) {
  //    whileStatement();
  //  } else if (match(TOKEN_LEFT_BRACE)) {
  //    beginScope();
  //    block();
  //    endScope();
  //  } else {
  //    expressionStatement();
  //  }

  // if (match(TokenType::FOR)) {
  // } else
  if (match(TokenType::IF)) {
    return JUST(parseIfStmt());
  } else if (match(TokenType::VAR)) {
    return JUST(parseVarStmt());
  } else if (match(TokenType::RETURN)) {
    return JUST(parseReturnStmt());
  } else if (match(TokenType::LEFT_BRACE)) {
    return parseBlock();
  } else {
    return JUST(parseExprStmt());
  }
}

Maybe<Expr> Parser::parseBlock() {
  ExprPtr ret;
  std::vector<ExprPtr> blocks;
  while (not check(TokenType::RIGHT_BRACE) and not isAtEnd()) {
    auto block = JUST(parseStmts());
    blocks.push_back(block);
  }
  JUST(consume(TokenType::RIGHT_BRACE, "Expect '}' in function body"));
  ret = std::make_shared<BlockStmt>(blocks);
  return ret;
}

Maybe<Expr> Parser::parseFunction() {
  ExprPtr ret;
  JUST(consume(TokenType::FUN, "Expect fn define function"));
  auto function_name =
      JUST(consume(TokenType::IDENTIFIER, "Expect identifier function name"));
  auto params = JUST(Params());
  JUST(consume(TokenType::ARROW, "Expect -> after function params"));
  auto ret_type = JUST(
      consume(TokenType::IDENTIFIER, "Expect identifier function return type"));
  auto body = JUST(parseStmts());
  ret = std::make_shared<FunctionDef>(function_name->lexeme, *params,
                                      ret_type->lexeme, body);
  return ret;
}

Maybe<Expr> Parser::expression() {
  auto expr = parsePrecedence(Precedence::None);
  return expr;
}

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
    // INFO("before: {}, after: {}", token.toString(), peek().toString());
  }
  return lhs;
}

Maybe<std::vector<ExprPtr>> Parser::Arguments() {
  std::vector<ExprPtr> args;
  size_t arg_cnt = 0;
  if (not check(TokenType::RIGHT_PAREN)) {
    do {
      auto arg = JUST(parsePrecedence(Precedence::Assig));
      args.push_back(arg);
    } while (match(TokenType::COMMA));
  }
  JUST(consume(TokenType::RIGHT_PAREN, "Expect ')' after function args"));
  return args;
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
