#ifndef LOX_OPS_TD 
#define LOX_OPS_TD

include "ir/loxDialect.td"
include "mlir/IR/OpBase.td"
include "mlir/Interfaces/InferTypeOpInterface.td"
include "mlir/Interfaces/SideEffectInterfaces.td"
include "mlir/Interfaces/FunctionInterfaces.td"
include "mlir/Interfaces/ControlFlowInterfaces.td"
include "mlir/IR/RegionKindInterface.td"

class LoxOp<string mnemonic, list<Trait> traits = []> :
    Op<LoxDialect, mnemonic, traits>;

/* def ConstantOp : LoxOp<"const", [Pure]> { */
/*   let summary = "const operation"; */
/*   let arguments = (ins APIntAttr:$value); */
/*   let results = (outs AnyInteger:$result); */
/* } */

#endif
