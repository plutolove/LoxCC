#include "parser/scanner.h"

#include "common/log.h"
#include "common/maybe.h"

namespace Lox {

Scanner::Scanner(const std::string& src) : source(src) {}

Maybe<std::vector<Token>> Scanner::scanTokens() {
  while (!isAtEnd()) {
    start = current;
    scanToken();
  }

  tokens.push_back(Token{.type = TokenType::END, .lexeme = "", .line = line});
  if (err->empty()) return std::make_shared<std::vector<Token>>(tokens);
  return err;
}

void Scanner::scanToken() {
  char c = advance();
  switch (c) {
    case '(':
      addToken(TokenType::LEFT_PAREN);
      break;
    case ')':
      addToken(TokenType::RIGHT_PAREN);
      break;
    case '[':
      addToken(TokenType::LEFT_BRACKET);
      break;
    case ']':
      addToken(TokenType::RIGHT_BRACKET);
      break;
    case '{':
      addToken(TokenType::LEFT_BRACE);
      break;
    case '}':
      addToken(TokenType::RIGHT_BRACE);
      break;
    case ',':
      addToken(TokenType::COMMA);
      break;
    case '.':
      addToken(TokenType::DOT);
      break;
    case '-':
      addToken(match('>') ? TokenType::ARROW : TokenType::MINUS);
      break;
    case '+':
      addToken(TokenType::PLUS);
      break;
    case ':':
      addToken(TokenType::COLON);
      break;
    case ';':
      addToken(TokenType::SEMICOLON);
      break;
    case '*':
      addToken(TokenType::STAR);
      break;
    case '!':
      addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
      break;
    case '=':
      addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
      break;
    case '<':
      addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
      break;
    case '>':
      addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
      break;
      //< two-char-tokens
      //> slash
    case '/':
      if (match('/')) {
        while (peek() != '\n' && !isAtEnd()) advance();
      } else {
        addToken(TokenType::SLASH);
      }
      break;
    case ' ':
    case '\r':
    case '\t':
      // Ignore whitespace.
      break;
    case '\n':
      line++;
      break;
      //< whitespace

      //> string-start
    case '"':
      peekString();
      break;
      //< string-end

      //> char-error
    default:
      /* Scanning char-error < Scanning digit-start
              Lox.error(line, "Unexpected character.");
      */
      //> digit-start
      if (isDigit(c)) {
        number();
        //> identifier-start
      } else if (isAlpha(c)) {
        identifier();
        //< identifier-start
      } else {
        AppendErr(err, "Unexpected charactoer in line: {}", line);
        for (auto&& token : tokens) {
          INFO("token: {}", token.toString());
        }
        // Lox.error(line, "Unexpected character.");
      }
      //< digit-start
      break;
  }
}

void Scanner::identifier() {
  while (isAlphaNumeric(peek())) advance();

  /* Scanning identifier < Scanning keyword-type
      addToken(IDENTIFIER);
  */
  //> keyword-type
  auto type = TokenType::IDENTIFIER;
  auto text = source.substr(start, current - start);
  auto iter = key_words.find(text);
  if (iter != key_words.end()) type = iter->second;
  addToken(type);
  //< keyword-type
}

void Scanner::peekString() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') line++;
    advance();
  }

  if (isAtEnd()) {
    AppendErr(err, "Unterminated string line: {}", line);
    // Lox.error(line, "Unterminated string.");
    return;
  }

  // The closing ".
  advance();

  // Trim the surrounding quotes.
  auto value = source.substr(start + 1, current - 2 - start);
  addToken(TokenType::STRING, value);
}

void Scanner::arrow() { addToken(TokenType::ARROW); }

void Scanner::number() {
  while (isDigit(peek())) advance();

  // Look for a fractional part.
  if (peek() == '.' && isDigit(peekNext())) {
    // Consume the "."
    advance();

    while (isDigit(peek())) advance();
    addToken(TokenType::FLOAT);
    return;
  }

  addToken(TokenType::INT);
}

}  // namespace Lox
