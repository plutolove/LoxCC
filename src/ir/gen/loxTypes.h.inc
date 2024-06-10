/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* TypeDef Declarations                                                       *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

#ifdef GET_TYPEDEF_CLASSES
#undef GET_TYPEDEF_CLASSES


namespace mlir {
class AsmParser;
class AsmPrinter;
} // namespace mlir
namespace mlir {
namespace Lox {
class PolynomialType;
namespace detail {
struct PolynomialTypeStorage;
} // namespace detail
class PolynomialType : public ::mlir::Type::TypeBase<PolynomialType, ::mlir::Type, detail::PolynomialTypeStorage> {
public:
  using Base::Base;
  static constexpr ::llvm::StringLiteral name = "Lox.poly";
  static PolynomialType get(::mlir::MLIRContext *context, int degreeBound);
  static constexpr ::llvm::StringLiteral getMnemonic() {
    return {"poly"};
  }

  static ::mlir::Type parse(::mlir::AsmParser &odsParser);
  void print(::mlir::AsmPrinter &odsPrinter) const;
  int getDegreeBound() const;
};
} // namespace Lox
} // namespace mlir
MLIR_DECLARE_EXPLICIT_TYPE_ID(::mlir::Lox::PolynomialType)

#endif  // GET_TYPEDEF_CLASSES

