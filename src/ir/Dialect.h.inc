/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Dialect Declarations                                                       *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|* From: lox.td                                                               *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

namespace Lox {

class LoxDialect : public ::mlir::Dialect {
  explicit LoxDialect(::mlir::MLIRContext *context);

  void initialize();
  friend class ::mlir::MLIRContext;
public:
  ~LoxDialect() override;
  static constexpr ::llvm::StringLiteral getDialectNamespace() {
    return ::llvm::StringLiteral("Lox");
  }
};
} // namespace Lox
MLIR_DECLARE_EXPLICIT_TYPE_ID(::Lox::LoxDialect)
