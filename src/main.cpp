#include <fstream>
#include <type_traits>
#include <utility>

#include "analysis/analysis.h"
#include "common/log.h"
#include "common/maybe.h"
#include "gflags/gflags.h"
#include "ir/loxDialect.h"
#include "ir/loxTypes.h"
#include "llvm/Support/raw_ostream.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include "mlir/Dialect/SCF/IR/SCF.h"
#include "mlir/IR/AsmState.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/Parser/Parser.h"
#include "mlir/Support/FileUtilities.h"
#include "parser/pratt_parser.h"
#include "parser/scanner.h"
#include "parser/visitor_helper.h"

using namespace mlir;

template <typename T>
struct Destructor {
  static void free(T* ptr) { ptr->~T(); }
};

mlir::MemRefType create1DMemRefType(int length, mlir::Type elementType) {
  mlir::MemRefType::Builder builder({length}, elementType);
  return builder;
}

int main(int argc, char** argv) {
  mlir::DialectRegistry registry;
  registry.insert<mlir::Lox::LoxDialect>();

  MLIRContext ctx;
  auto str_type = mlir::Lox::StructType::get(
      {create1DMemRefType(30, mlir::IntegerType::get(&ctx, 8))});

  ctx.loadDialect<func::FuncDialect, arith::ArithDialect, memref::MemRefDialect,
                  scf::SCFDialect>();

  // 创建 OpBuilder
  OpBuilder builder(&ctx);
  auto mod = builder.create<ModuleOp>(builder.getUnknownLoc());
  auto loc = builder.getUnknownLoc();

  // 设置插入点
  builder.setInsertionPointToEnd(mod.getBody());

  // 创建 func
  auto i32 = builder.getI32Type();
  auto funcType = builder.getFunctionType({i32, i32}, {i32});
  auto func =
      builder.create<func::FuncOp>(builder.getUnknownLoc(), "test", funcType);

  return 0;
}
