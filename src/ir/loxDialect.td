#ifndef LOX_DIALECT_TD 
#define LOX_DIALECT_TD

include "mlir/IR/OpBase.td"

def LoxDialect : Dialect {
  let name = "Lox";
  let cppNamespace = "::mlir::Lox";
  let summary = "Lox Dialect"; 

  /* let hasConstantMaterializer = 1; */
}

#endif
