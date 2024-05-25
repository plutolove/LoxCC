#pragma once
#include <string>

#include "fmt/format.h"
#include "magic_enum.hpp"

namespace Lox {

enum class TokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // One or two character tokens.
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

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

struct Token {
  std::string toString() const {
    return fmt::format("{}:'{}':{}", magic_enum::enum_name(type), lexeme, line);
  }

  TokenType type;
  std::string lexeme;
  int line;
};

}  // namespace Lox
