#ifndef LOX_TYPES_TD 
#define LOX_TYPES_TD

include "ir/loxDialect.td"
include "mlir/IR/AttrTypeBase.td"


def LoxStructType :
    DialectType<LoxDialect, CPred<"::llvm::isa<StructType>($_self)">,
                "lox struct type">;

#endif
