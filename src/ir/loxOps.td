#ifndef LOX_OPS_TD 
#define LOX_OPS_TD

include "ir/loxDialect.td"
include "mlir/IR/OpBase.td"
include "mlir/Interfaces/InferTypeOpInterface.td"
include "mlir/Interfaces/SideEffectInterfaces.td"
include "mlir/Interfaces/FunctionInterfaces.td"
include "mlir/Interfaces/ControlFlowInterfaces.td"
include "mlir/IR/RegionKindInterface.td"
include "mlir/Interfaces/CallInterfaces.td"

class LoxOp<string mnemonic, list<Trait> traits = []> :
    Op<LoxDialect, mnemonic, traits>;

def ConstantOp : LoxOp<"const", [Pure]> {
  let summary = "const operation";
  let arguments = (ins APIntAttr:$value);
  let results = (outs AnyInteger:$result);

  let builders = [
    // Build a constant with a given constant floating-point value.
    OpBuilder<(ins "double":$value), [{
    $_state.addAttribute("attr", $_builder.getF32FloatAttr(value));}]>
  ];
}

def ReturnOp : LoxOp<"ret", [Terminator, ReturnLike]> {
  let summary = "return operation";
  let arguments = (ins AnyType:$data);
  let assemblyFormat = "$data attr-dict `:` type($data)";
}

def FuncOp : LoxOp<"func", [
  IsolatedFromAbove,
  FunctionOpInterface,
  RegionKindInterface]> {
  let summary = "function";
  let arguments = (ins
    SymbolNameAttr:$sym_name,
    TypeAttrOf<FunctionType>:$function_type,
    // FunctionOpInterface 需要两个 Attr 来记录 arg 和 res 的名字
    OptionalAttr<DictArrayAttr>:$arg_attrs,
    OptionalAttr<DictArrayAttr>:$res_attrs
  );

  let regions = (region AnyRegion:$body);
  /* let hasCustomAssemblyFormat = true; */

  let extraClassDeclaration = [{
    // Method of FunctionOpInterface
    mlir::Region * getCallableRegion() {return &getBody();}
    // getFunctionType 函数会自动生成
    // mlir::FunctionType getFunctionType();

    // Method of CallableOpInterface
    llvm::ArrayRef<mlir::Type> getArgumentTypes() {return getFunctionType().getInputs();}
    llvm::ArrayRef<mlir::Type> getResultTypes() {return getFunctionType().getResults();}

    // Method of RegionKindInterface
    static mlir::RegionKind getRegionKind(unsigned idx) { return mlir::RegionKind::SSACFG; }
  }];
}

def CallOp : LoxOp<"call", [CallOpInterface]> {
  let summary = "call operation";
  let arguments = (ins SymbolRefAttr:$callee, Variadic<AnyType>:$arg_operands);
  let results = (outs AnyType:$result);
  let assemblyFormat = "$callee `(` $arg_operands `)` attr-dict `:` functional-type($arg_operands, results)";
  let extraClassDeclaration = [{
    mlir::CallInterfaceCallable getCallableForCallee() { return getCalleeAttr(); }
    void setCalleeFromCallable(mlir::CallInterfaceCallable callee) { setCalleeAttr(callee.get<mlir::SymbolRefAttr>()); }
  }];
}

#endif
