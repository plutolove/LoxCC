#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "common/maybe.h"
#include "parser/token.h"

namespace Lox {

class Scanner {
 public:
  Scanner(const std::string& src);

  Maybe<std::vector<Token>> scanTokens();

 private:
  void scanToken();
  void identifier();
  void peekString();
  void number();

  bool isAtEnd() { return current >= source.size(); }
  bool isDigit(char c) { return c >= '0' && c <= '9'; }
  bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
  }

  bool isAlphaNumeric(char ch) { return isAlpha(ch) or isDigit(ch); }

  void addToken(TokenType type, const std::string& val) {
    tokens.push_back(Token{.type = type, .lexeme = val, .line = line});
  }

  void addToken(TokenType type) {
    tokens.push_back(Token{.type = type,
                           .lexeme = source.substr(start, current - start),
                           .line = line});
  }

  bool match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;
    current++;
    return true;
  }

  char peekNext() {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
  }

  char peek() {
    if (isAtEnd()) return '\0';
    return source[current];
  }

  char advance() { return source[current++]; }

  std::string source;
  std::vector<Token> tokens;
  int start{0};
  int current{0};
  int line{1};
  std::shared_ptr<ErrorProto> err = NewErr();

  inline const static std::unordered_map<std::string, TokenType> key_words{
      {"and", TokenType::AND},       {"class", TokenType::CLASS},
      {"else", TokenType::ELSE},     {"false", TokenType::FALSE},
      {"for", TokenType::FOR},       {"fun", TokenType::FUN},
      {"if", TokenType::IF},         {"nil", TokenType::NIL},
      {"or", TokenType::OR},         {"print", TokenType::PRINT},
      {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
      {"this", TokenType::THIS},     {"true", TokenType::TRUE},
      {"var", TokenType::VAR},       {"while", TokenType::WHILE},
  };
};

}  // namespace Lox
