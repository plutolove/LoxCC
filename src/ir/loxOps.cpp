#include "ir/loxOps.h"

#include "mlir/Interfaces/FunctionImplementation.h"

namespace mlir {
namespace Lox {

// ::mlir::ParseResult FuncOp::parse(::mlir::OpAsmParser &parser,
//                                   ::mlir::OperationState &result) {
//   auto buildFuncType = [](auto &builder, auto argTypes, auto results, auto,
//                           auto) {
//     return builder.getFunctionType(argTypes, results);
//   };
//   return function_interface_impl::parseFunctionOp(
//       parser, result, false, getFunctionTypeAttrName(result.name),
//       buildFuncType, getArgAttrsAttrName(result.name),
//       getResAttrsAttrName(result.name));
// }

// void FuncOp::print(::mlir::OpAsmPrinter &p) {
//   mlir::function_interface_impl::printFunctionOp(
//       p, *this, /*isVariadic=*/false, getFunctionTypeAttrName(),
//       getArgAttrsAttrName(), getResAttrsAttrName());
// }

}  // namespace Lox
}  // namespace mlir
