#ifndef LOX_TYPES_TD 
#define LOX_TYPES_TD

include "ir/loxDialect.td"
include "mlir/IR/AttrTypeBase.td"

class Poly_Type<string name, string typeMnemonic> : TypeDef<LoxDialect, name> {
  let mnemonic = typeMnemonic;
}

def Polynomial : Poly_Type<"Polynomial", "poly"> {
  let summary = "A polynomial with u32 coefficients";

  let description = [{
    A type for polynomials with integer coefficients in a single-variable polynomial ring.
  }];

  let parameters = (ins "int":$degreeBound);
  let assemblyFormat = "`<` $degreeBound `>`";
}


#endif
