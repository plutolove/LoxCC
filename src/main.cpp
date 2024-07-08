#include <fstream>
#include <optional>
#include <type_traits>
#include <utility>

#include "analysis/analysis.h"
#include "common/log.h"
#include "common/maybe.h"
#include "gflags/gflags.h"
#include "ir/loxDialect.h"
#include "ir/loxOps.h"
#include "ir/loxTypes.h"
#include "llvm/Support/raw_ostream.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/Extensions/AllExtensions.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include "mlir/Dialect/SCF/IR/SCF.h"
#include "mlir/IR/AsmState.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/DialectRegistry.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/InitAllDialects.h"
#include "mlir/InitAllExtensions.h"
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
  INFO("size str: {}", sizeof(std::string));
  mlir::DialectRegistry registry;
  // mlir::func::registerAllExtensions(registry);
  mlir::MLIRContext context(registry);
  // Load our Dialect in this MLIR Context.
  context.getOrLoadDialect<mlir::Lox::LoxDialect>();

  auto str_type = mlir::Lox::StructType::get(
      {mlir::VectorType::get({32}, mlir::IntegerType::get(&context, 8))});

  // 创建 OpBuilder
  OpBuilder builder(&context);
  auto mod = builder.create<ModuleOp>(builder.getUnknownLoc());
  auto loc = builder.getUnknownLoc();

  // 设置插入点
  builder.setInsertionPointToEnd(mod.getBody());

  // 创建 func
  auto funcType = builder.getFunctionType({str_type, str_type}, std::nullopt);
  llvm::StringRef name = "test";
  auto func = builder.create<mlir::Lox::FuncOp>(loc, name, funcType);

  // 添加基本块
  auto entry = func.addEntryBlock();
  auto args = entry->getArguments();

  // 设置插入点
  builder.setInsertionPointToEnd(entry);

  llvm::StringRef call_name = "str_concat";
  SmallVector<mlir::Value, 4> operands;
  operands.push_back(args[0]);
  operands.push_back(args[1]);
  builder.create<mlir::Lox::CallOp>(loc, str_type, call_name, operands);
  mod->dump();

  return 0;
}
