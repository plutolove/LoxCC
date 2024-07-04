#pragma once
#include "boost/mp11/detail/mp_list.hpp"
#include "boost/mp11/list.hpp"
#include "common/visitor.h"
#include "parser/expr.h"

namespace Lox {

using AstVisitorHelper =
    VisitorHelper<void, Binary, Assign, FunctionCall, Grouping, Literal,
                  Logical, Unary, Identifier, Variable, Module, FunctionDef,
                  BlockStmt, ForStmt, IfStmt, VarExpressionStmt, ReturnStmt>;

template <typename Tl, typename R,
          template <typename, typename, typename> class Unit, typename Root>
class VisitorHelperDerived;

template <typename H, typename... Ts, typename R,
          template <typename, typename, typename> class Unit, typename Root>
class VisitorHelperDerived<boost::mp11::mp_list<H, Ts...>, R, Unit, Root>
    : public Unit<
          R, H,
          VisitorHelperDerived<boost::mp11::mp_list<Ts...>, R, Unit, Root>> {};

template <typename T, typename R,
          template <typename, typename, typename> class Unit, typename Root>
class VisitorHelperDerived<boost::mp11::mp_list<T>, R, Unit, Root>
    : public Unit<R, T, Root> {};

using VisitorImpl = VisitorHelperDerived<
    boost::mp11::mp_list<Binary, Assign, FunctionCall, Grouping, Literal,
                         Logical, Unary, Identifier, Variable, Module,
                         FunctionDef, BlockStmt, ForStmt, IfStmt,
                         VarExpressionStmt, ReturnStmt>,
    void, VisitorDerived, VisitorBase<>>;

}  // namespace Lox
