#include <fstream>

#include "analysis/analysis.h"
#include "common/log.h"
#include "common/maybe.h"
#include "gflags/gflags.h"
#include "parser/pratt_parser.h"
#include "parser/scanner.h"

using namespace Lox;

DEFINE_string(path, "", "");

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  std::ifstream file(FLAGS_path);

  std::string content((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());

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
  Lox::Analysis analysis;
  auto result = analysis.analysis(expr.Data());
  if (not result.isOk()) {
    ERROR("analysis failed: {}", result.ErrMsg());
  }
  return 0;
}

