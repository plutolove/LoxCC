#pragma once
#include <string>

#include "fmt/format.h"
#include "magic_enum.hpp"

namespace Lox {

enum class TokenType {
  // Single-character tokens.
  LEFT_PAREN,     // (
  RIGHT_PAREN,    // )
  LEFT_BRACE,     // {
  RIGHT_BRACE,    // }
  LEFT_BRACKET,   // [
  RIGHT_BRACKET,  // ]
  COMMA,          // ,
  DOT,            // .
  MINUS,          // -
  PLUS,           // +
  SEMICOLON,      // ;
  COLON,          // :
  SLASH,          // /
  STAR,           // *

  // One or two character tokens.
  BANG,           // !
  BANG_EQUAL,     // !=
  EQUAL,          // =
  EQUAL_EQUAL,    // ==
  GREATER,        // >
  GREATER_EQUAL,  // >=
  LESS,           // <
  LESS_EQUAL,     // <=
  ARROW,          // ->

  // Literals.
  IDENTIFIER,
  STRING,
  INT,
  FLOAT,

  // Keywords.
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  END,
};

enum class Precedence {
  None = 0,
  Assig,
  Or,
  And,
  EQUALITY,
  COMPARISON,
  TERM,
  FACTOR,
  UNARY,
  CALL,
  PRIMARY
};

struct Token {
  std::string toString() const {
    return fmt::format("{}: '{}', prece: {}, line: {}",
                       magic_enum::enum_name(type), lexeme,
                       magic_enum::enum_name(precedence), line);
  }

  TokenType type;
  std::string lexeme;
  Precedence precedence;
  int line;
};

}  // namespace Lox
