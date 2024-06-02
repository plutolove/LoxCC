#pragma once
#include <list>
#include <string>
#include <type_traits>
#include <variant>

#include "common/type_trait.h"
#include "fmt/format.h"

namespace Lox {

class ErrorProto {
 public:
  ErrorProto() {}

  template <typename... Args>
  ErrorProto(const std::string& file, int64_t line, const std::string& func,
             fmt::format_string<Args...> format, Args&&... args)
      : error_message(
            fmt::format("{} : {}", fmt::format("{}:{}:{}", file, line, func),
                        fmt::format(format, std::forward<Args>(args)...))) {}

  template <typename... Args>
  void append(const std::string& file, int64_t line, const std::string& func,
              fmt::format_string<Args...> format, Args&&... args) {
    error_message.append("\n");

    error_message.append(
        fmt::format("{} : {}", fmt::format("{}:{}:{}", file, line, func),
                    fmt::format(format, std::forward<Args>(args)...)));
  }

  void addStackFrame(const std::string& file, int64_t line,
                     const std::string& func, const std::string& token) {
    stack_frame.push_back(
        fmt::format("{}:{}:{} -> {}", file, line, func, token));
  }

  bool empty() const { return error_message.empty(); }
  std::string errorMsg() {
    std::string msg = error_message;
    for (auto iter = stack_frame.rbegin(); iter != stack_frame.rend(); ++iter) {
      msg.append("\n");
      msg.append(*iter);
    }
    return msg;
  }

 private:
  std::string error_message;
  std::list<std::string> stack_frame;
};

#define NewErrImpl(file, line, func, ...) \
  std::make_shared<ErrorProto>(file, line, func, ##__VA_ARGS__)

#define NewErr(...) NewErrImpl(__FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

#define AppendErr(err, ...) \
  err->append(__FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

template <typename T, typename = guard::Guard>
class Maybe;

template <typename T>
class Maybe<
    T, guard::TypeGuard<!std::is_same_v<T, void> && !std::is_arithmetic_v<T>>> {
 public:
  using ValueType = std::shared_ptr<T>;
  using ErrorType = std::shared_ptr<ErrorProto>;

  Maybe(const T& val) : data_(std::make_shared<T>(val)) {}
  Maybe(const ValueType& ptr) : data_(ptr) {}
  Maybe(ValueType&& ptr) : data_(std::move(ptr)) {}
  Maybe(const ErrorType& err) : data_(err) {}
  Maybe(const ErrorProto& err) : data_(std::make_shared<ErrorProto>(err)) {}

  Maybe(ErrorProto&& err)
      : data_(std::make_shared<ErrorProto>(std::move(err))) {}

  bool isOk() const { return std::holds_alternative<ValueType>(data_); }

  ValueType Data() { return std::get<ValueType>(data_); }

  std::string ErrMsg() { return std::get<ErrorType>(data_)->errorMsg(); }

  ErrorType Error() { return std::get<ErrorType>(data_); }

 private:
  std::variant<ValueType, ErrorType> data_;
};

template <typename T>
class Maybe<T, guard::TypeGuard<std::is_same_v<T, void>>> {
 public:
  using ValueType = bool;
  using ErrorType = std::shared_ptr<ErrorProto>;

  template <typename D>
  Maybe(D&& d) : data_(true) {}

  Maybe(const ErrorType& err) : data_(err) {}
  Maybe(const ErrorProto& err) : data_(std::make_shared<ErrorProto>(err)) {}

  Maybe(ErrorProto&& err)
      : data_(std::make_shared<ErrorProto>(std::move(err))) {}

  bool isOk() const { return std::holds_alternative<ValueType>(data_); }

  ValueType Data() { return std::get<ValueType>(data_); }

  std::string ErrMsg() { return std::get<ErrorType>(data_)->errorMsg(); }

  ErrorType Error() { return std::get<ErrorType>(data_); }

 private:
  std::variant<bool, ErrorType> data_;
};

template <typename T>
class Maybe<T, guard::TypeGuard<std::is_arithmetic_v<T>>> {
 public:
  using ValueType = T;
  using ErrorType = std::shared_ptr<ErrorProto>;

  Maybe(ValueType val) : data_(val) {}

  Maybe(const ErrorType& err) : data_(err) {}
  Maybe(const ErrorProto& err) : data_(std::make_shared<ErrorProto>(err)) {}

  Maybe(ErrorProto&& err)
      : data_(std::make_shared<ErrorProto>(std::move(err))) {}

  bool isOk() const { return std::holds_alternative<ValueType>(data_); }

  ValueType Data() { return std::get<ValueType>(data_); }

  std::string ErrMsg() { return std::get<ErrorType>(data_)->errorMsg(); }

  ErrorType Error() { return std::get<ErrorType>(data_); }

 private:
  std::variant<ValueType, ErrorType> data_;
};

#define JUST(...)                                                              \
  ({                                                                           \
    auto&& value_to_check_ = __VA_ARGS__;                                      \
    if (!value_to_check_.isOk()) {                                             \
      value_to_check_.Error()->addStackFrame(__FILE__, __LINE__, __FUNCTION__, \
                                             #__VA_ARGS__);                    \
      return value_to_check_.Error();                                          \
    }                                                                          \
    value_to_check_;                                                           \
  }).Data()

}  // namespace Lox

