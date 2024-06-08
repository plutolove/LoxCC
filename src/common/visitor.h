#pragma once
#include "common/maybe.h"

namespace Lox {

template <typename R = void>
class VisitorBase {
 public:
  virtual ~VisitorBase() {}
};

template <typename R, typename Derived>
class Visitor : virtual public VisitorBase<R> {
 public:
  using ValueType = Maybe<R>;
  virtual ValueType visit(Derived* ptr) = 0;
};

template <typename R, typename... Args>
class VisitorHelper : public Visitor<R, Args>... {};

}  // namespace Lox
