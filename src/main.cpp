#include <fstream>

#include "common/log.h"
#include "common/maybe.h"
#include "gflags/gflags.h"
#include "parser/pratt_parser.h"
#include "parser/scanner.h"

using namespace Lox;

DEFINE_string(path, "", "");

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  std::ifstream file(FLAGS_path);  // 替换为实际文件路径

  std::string content((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
  INFO("content: {}", content);
  //
  // Lox::Scanner scan_test("fn test(x : int) -> int");
  // Lox::Scanner scan("-1+2*3-4");(* (* (+ a b) c) (+ d e))
  // Lox::Scanner scan(
  // "(a + b) * c * (12 + x)[z]- (d + e) and (23 > 23) or (234<324)");
  // Lox::Scanner scan("var_name.b.c = call(-4*-5+2/3, a, b, c);");
  Lox::Scanner scan(content);
  auto tokens = scan.scanTokens();
  Lox::Parser parser(*tokens.Data());
  auto expr = parser.parse();
  if (expr.isOk()) {
    INFO("src: {}", scan.getSource());
    INFO("{}", expr.Data()->to_string());
  } else {
    ERROR("parse failed: {}", expr.ErrMsg());
  }
  return 0;
}

