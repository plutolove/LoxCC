#include <fstream>
#include <type_traits>
#include <utility>

#include "analysis/analysis.h"
#include "common/log.h"
#include "common/maybe.h"
#include "gflags/gflags.h"
#include "ir/loxDialect.h"
#include "llvm/Support/raw_ostream.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include "mlir/Dialect/SCF/IR/SCF.h"
#include "mlir/IR/AsmState.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/Parser/Parser.h"
#include "mlir/Support/FileUtilities.h"
#include "parser/pratt_parser.h"
#include "parser/scanner.h"

using namespace mlir;

template <typename T>
struct Destructor {
  static void free(T* ptr) { ptr->~T(); }
};

int main(int argc, char** argv) {
  mlir::DialectRegistry registry;
  registry.insert<mlir::Lox::LoxDialect>();

  MLIRContext ctx;
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

  // 添加基本块
  auto entry = func.addEntryBlock();
  auto args = entry->getArguments();

  // 设置插入点
  builder.setInsertionPointToEnd(entry);

  // 创建 arith.addi
  auto addi =
      builder.create<arith::AddIOp>(builder.getUnknownLoc(), args[0], args[1]);

  auto memrefType = mlir::MemRefType::get({32}, builder.getIntegerType(8));
  auto alloc =
      builder.create<memref::AllocaOp>(builder.getUnknownLoc(), memrefType);

  mlir::func::FuncOp funcOp = mod.lookupSymbol<mlir::func::FuncOp>("test_call");
  if (!funcOp) {
    OpBuilder::InsertionGuard insertionGuard(builder);
    builder.setInsertionPointToStart(mod.getBody());

    funcOp = builder.create<mlir::func::FuncOp>(
        builder.getUnknownLoc(), "test_call",
        builder.getFunctionType({memrefType}, builder.getF32Type()));
    funcOp.setPrivate();
    funcOp->setAttr("llvm.linkage", builder.getStringAttr("external"));
  }

  auto ifOp = builder.create<mlir::scf::IfOp>(
      loc, llvm::ArrayRef<mlir::Type>{builder.getF32Type()}, addi, true);

  builder.setInsertionPointToStart(&ifOp.getThenRegion().front());
  // 在这里添加你的代码，例如：
  auto x_true = builder.create<arith::ConstantOp>(
      loc, builder.getF32Type(), builder.getF32FloatAttr(231.2));

  builder.create<mlir::scf::YieldOp>(loc, llvm::ArrayRef<mlir::Value>{x_true});

  // 创建 else 分支
  builder.setInsertionPointToStart(&ifOp.getElseRegion().front());
  // 在这里添加你的代码，例如：
  auto x_false = builder.create<arith::ConstantOp>(
      loc, builder.getF32Type(), builder.getF32FloatAttr(213445.2));

  builder.create<mlir::scf::YieldOp>(loc, llvm::ArrayRef<mlir::Value>{x_false});

  // 将插入点设置回原来的位置
  builder.setInsertionPointAfter(ifOp);

  auto if_res = ifOp.getResult(0);

  // memref::AllocOp;
  std::vector<mlir::Value> convertedValues{alloc};

  // // 创建一个调用操作。
  auto callOp = builder.create<mlir::func::CallOp>(builder.getUnknownLoc(),
                                                   funcOp, convertedValues);
  auto call_res = callOp.getResult(0);
  auto fadd = builder.create<arith::AddFOp>(loc, if_res, call_res);

  auto ret = builder.create<arith::FPToSIOp>(loc, builder.getI32Type(), fadd);

  builder.create<func::ReturnOp>(builder.getUnknownLoc(), ValueRange({addi}));

  mod->print(llvm::outs());
  INFO("str size: {}", sizeof(std::string));
  char str_ptr[32];
  std::string* str_val = new ((void*)str_ptr) std::string("345w345645");
  INFO("str: {}", *str_val);
  Destructor<std::string>::free(str_val);

  return 0;
}
