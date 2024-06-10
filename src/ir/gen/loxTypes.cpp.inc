/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* TypeDef Definitions                                                        *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

#ifdef GET_TYPEDEF_LIST
#undef GET_TYPEDEF_LIST

::mlir::Lox::PolynomialType

#endif  // GET_TYPEDEF_LIST

#ifdef GET_TYPEDEF_CLASSES
#undef GET_TYPEDEF_CLASSES

static ::mlir::OptionalParseResult generatedTypeParser(::mlir::AsmParser &parser, ::llvm::StringRef *mnemonic, ::mlir::Type &value) {
  return ::mlir::AsmParser::KeywordSwitch<::mlir::OptionalParseResult>(parser)
    .Case(::mlir::Lox::PolynomialType::getMnemonic(), [&](llvm::StringRef, llvm::SMLoc) {
      value = ::mlir::Lox::PolynomialType::parse(parser);
      return ::mlir::success(!!value);
    })
    .Default([&](llvm::StringRef keyword, llvm::SMLoc) {
      *mnemonic = keyword;
      return std::nullopt;
    });
}

static ::mlir::LogicalResult generatedTypePrinter(::mlir::Type def, ::mlir::AsmPrinter &printer) {
  return ::llvm::TypeSwitch<::mlir::Type, ::mlir::LogicalResult>(def)    .Case<::mlir::Lox::PolynomialType>([&](auto t) {
      printer << ::mlir::Lox::PolynomialType::getMnemonic();
t.print(printer);
      return ::mlir::success();
    })
    .Default([](auto) { return ::mlir::failure(); });
}

namespace mlir {
namespace Lox {
namespace detail {
struct PolynomialTypeStorage : public ::mlir::TypeStorage {
  using KeyTy = std::tuple<int>;
  PolynomialTypeStorage(int degreeBound) : degreeBound(std::move(degreeBound)) {}

  KeyTy getAsKey() const {
    return KeyTy(degreeBound);
  }

  bool operator==(const KeyTy &tblgenKey) const {
    return (degreeBound == std::get<0>(tblgenKey));
  }

  static ::llvm::hash_code hashKey(const KeyTy &tblgenKey) {
    return ::llvm::hash_combine(std::get<0>(tblgenKey));
  }

  static PolynomialTypeStorage *construct(::mlir::TypeStorageAllocator &allocator, KeyTy &&tblgenKey) {
    auto degreeBound = std::move(std::get<0>(tblgenKey));
    return new (allocator.allocate<PolynomialTypeStorage>()) PolynomialTypeStorage(std::move(degreeBound));
  }

  int degreeBound;
};
} // namespace detail
PolynomialType PolynomialType::get(::mlir::MLIRContext *context, int degreeBound) {
  return Base::get(context, std::move(degreeBound));
}

::mlir::Type PolynomialType::parse(::mlir::AsmParser &odsParser) {
  ::mlir::Builder odsBuilder(odsParser.getContext());
  ::llvm::SMLoc odsLoc = odsParser.getCurrentLocation();
  (void) odsLoc;
  ::mlir::FailureOr<int> _result_degreeBound;
  // Parse literal '<'
  if (odsParser.parseLess()) return {};

  // Parse variable 'degreeBound'
  _result_degreeBound = ::mlir::FieldParser<int>::parse(odsParser);
  if (::mlir::failed(_result_degreeBound)) {
    odsParser.emitError(odsParser.getCurrentLocation(), "failed to parse Polynomial parameter 'degreeBound' which is to be a `int`");
    return {};
  }
  // Parse literal '>'
  if (odsParser.parseGreater()) return {};
  assert(::mlir::succeeded(_result_degreeBound));
  return PolynomialType::get(odsParser.getContext(),
      int((*_result_degreeBound)));
}

void PolynomialType::print(::mlir::AsmPrinter &odsPrinter) const {
  ::mlir::Builder odsBuilder(getContext());
  odsPrinter << "<";
  odsPrinter.printStrippedAttrOrType(getDegreeBound());
  odsPrinter << ">";
}

int PolynomialType::getDegreeBound() const {
  return getImpl()->degreeBound;
}

} // namespace Lox
} // namespace mlir
MLIR_DEFINE_EXPLICIT_TYPE_ID(::mlir::Lox::PolynomialType)
namespace mlir {
namespace Lox {

/// Parse a type registered to this dialect.
::mlir::Type LoxDialect::parseType(::mlir::DialectAsmParser &parser) const {
  ::llvm::SMLoc typeLoc = parser.getCurrentLocation();
  ::llvm::StringRef mnemonic;
  ::mlir::Type genType;
  auto parseResult = generatedTypeParser(parser, &mnemonic, genType);
  if (parseResult.has_value())
    return genType;
  
  parser.emitError(typeLoc) << "unknown  type `"
      << mnemonic << "` in dialect `" << getNamespace() << "`";
  return {};
}
/// Print a type registered to this dialect.
void LoxDialect::printType(::mlir::Type type,
                    ::mlir::DialectAsmPrinter &printer) const {
  if (::mlir::succeeded(generatedTypePrinter(type, printer)))
    return;
  
}
} // namespace Lox
} // namespace mlir

#endif  // GET_TYPEDEF_CLASSES

