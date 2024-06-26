/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Dialect Definitions                                                        *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|* From: lox.td                                                               *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

MLIR_DEFINE_EXPLICIT_TYPE_ID(::mlir::Lox::LoxDialect)
namespace mlir {
namespace Lox {

LoxDialect::LoxDialect(::mlir::MLIRContext *context)
    : ::mlir::Dialect(getDialectNamespace(), context, ::mlir::TypeID::get<LoxDialect>()) {
  
  initialize();
}

LoxDialect::~LoxDialect() = default;

} // namespace Lox
} // namespace mlir
