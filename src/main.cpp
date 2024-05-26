#include "common/log.h"
#include "common/maybe.h"
#include "parser/pratt_parser.h"
#include "parser/scanner.h"

using namespace Lox;

int main(int argc, char** argv) {
  // Lox::Scanner scan_test("fn test(x : int) -> int");
  // Lox::Scanner scan("-1+2*3-4");(* (* (+ a b) c) (+ d e))
  Lox::Scanner scan("(a + b) * c * x[z]- (d + e)");
  // Lox::Scanner scan("a + b[c]");
  auto tokens = scan.scanTokens();
  Lox::Parser parser(*tokens.Data());
  auto expr = parser.parse();
  if (expr.isOk()) {
    INFO("expr str: {}", expr.Data()->to_string());
  } else {
    ERROR("parse failed: {}", expr.ErrMsg());
  }
  return 0;
}
