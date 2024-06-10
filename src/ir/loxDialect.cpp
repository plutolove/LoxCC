// clang-format off

#include "ir/loxDialect.h"
#include "ir/loxOps.h"
#include "ir/loxTypes.h"
#include "mlir/IR/Builders.h"
#include "llvm/ADT/TypeSwitch.h"

#include "ir/gen/Dialect.cpp.inc"
#define GET_TYPEDEF_CLASSES
#include "ir/gen/loxTypes.cpp.inc"
#define GET_OP_CLASSES
#include "ir/gen/loxOps.cpp.inc"

// clang-format on

namespace mlir {
namespace Lox {

void LoxDialect::initialize() {
  addTypes<
#define GET_TYPEDEF_LIST
#include "ir/gen/loxTypes.cpp.inc"
      >();
  addOperations<
#define GET_OP_LIST
#include "ir/gen/loxTypes.cpp.inc"
      >();
}

}  // namespace Lox
}  // namespace mlir
