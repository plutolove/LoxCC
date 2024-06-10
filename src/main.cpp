#include <fstream>

#include "analysis/analysis.h"
#include "common/log.h"
#include "common/maybe.h"
#include "gflags/gflags.h"
#include "llvm/Support/raw_ostream.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/AsmState.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/Parser/Parser.h"
#include "mlir/Support/FileUtilities.h"
#include "parser/pratt_parser.h"
#include "parser/scanner.h"

using namespace Lox;

DEFINE_string(path, "", "");

// int main(int argc, char** argv) {
//   // gflags::ParseCommandLineFlags(&argc, &argv, true);

//  // std::ifstream file(FLAGS_path);

//  // std::string content((std::istreambuf_iterator<char>(file)),
//  //                     std::istreambuf_iterator<char>());

//  // Lox::Scanner scan(content);
//  // auto tokens = scan.scanTokens();
//  // Lox::Parser parser(*tokens.Data());
//  // auto expr = parser.parse();
//  // if (expr.isOk()) {
//  //   INFO("src: {}", scan.getSource());
//  //   INFO("{}", expr.Data()->to_string());
//  // } else {
//  //   ERROR("parse failed: {}", expr.ErrMsg());
//  // }
//  // Lox::Analysis analysis;
//  // auto result = analysis.analysis(expr.Data());
//  // if (not result.isOk()) {
//  //   ERROR("analysis failed: {}", result.ErrMsg());
//  // }
//  //

//  return 0;
//}
using namespace mlir;
int main(int argc, char** argv) {
  MLIRContext ctx;
  ctx.loadDialect<func::FuncDialect, arith::ArithDialect>();

  // 创建 OpBuilder
  OpBuilder builder(&ctx);
  auto mod = builder.create<ModuleOp>(builder.getUnknownLoc());

  // 设置插入点
  builder.setInsertionPointToEnd(mod.getBody());

  // 创建 func
  auto i32 = builder.getI32Type();
  auto funcType = builder.getFunctionType({i32, i32}, {i32});
  auto func =
      builder.create<func::FuncOp>(builder.getUnknownLoc(), "test", funcType);

  // 添加基本块
  auto entry = func.addEntryBlock();
  auto args = entry->getArguments();

  // 设置插入点
  builder.setInsertionPointToEnd(entry);

  // 创建 arith.addi
  auto addi =
      builder.create<arith::AddIOp>(builder.getUnknownLoc(), args[0], args[1]);

  // 创建 func.return
  builder.create<func::ReturnOp>(builder.getUnknownLoc(), ValueRange({addi}));
  mod->print(llvm::outs());
  return 0;
}
