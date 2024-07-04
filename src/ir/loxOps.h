#pragma once

// clang-format off
#include "ir/loxDialect.h"
#include "ir/loxTypes.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"
#include "mlir/Interfaces/CallInterfaces.h"
#include "mlir/Interfaces/FunctionInterfaces.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Dialect.h" 
#include "mlir/Interfaces/ControlFlowInterfaces.h"

#define GET_OP_CLASSES
#include "ir/gen/loxOps.h.inc"
// clang-format on
