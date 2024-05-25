#include "common/log.h"
#include "common/maybe.h"
#include "common/visitor.h"
#include "interpreter/interpreter.h"
#include "parser/expr.h"
#include "parser/parser.h"
#include "parser/scanner.h"

using namespace Lox;

class A : public VisitorHelper<void, Binary> {
 public:
  virtual void visit(Binary* ptr) override {
    INFO("------------------ success");
  }
};

Maybe<std::vector<Token>> test() {
  Lox::Scanner scan("fun def abc() \"ssdfsdf\" , 123, 123.34");
  // auto ret = scan.scanTokens();
  auto&& data = JUST(scan.scanTokens());
  INFO("data size: {}", data->size());
  for (auto&& token : *data) {
    INFO("{}", token.toString());
  }
  return data;
}

#define mac(x) #x
int main(int argc, char** argv) {
  Lox::Interpreter inter;
  // Binary op(nullptr, Token{}, nullptr);
  // A a{};
  // a.visit(&op);
  // auto ret = test();

  Lox::Scanner scan("(5 - (3 - 1)) + -1");
  auto ret = scan.scanTokens();
  if (ret.isOk()) {
    Lox::Parser parser(*ret.Data());
    auto expr = parser.parse();
    if (expr.isOk()) {
      INFO("parse success");
    } else {
      ERROR("parse failed: {}", expr.ErrMsg());
    }
  } else {
    ERROR("scanner failed: {}", ret.ErrMsg());
  }
  mac(sdfgsdfgsdf);
  return 0;
}
