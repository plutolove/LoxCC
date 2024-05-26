#pragma once
#include <list>
#include <string>
#include <variant>

#include "fmt/format.h"

namespace Lox {

class ErrorProto {
 public:
  ErrorProto() {}

  template <typename... Args>
  ErrorProto(fmt::format_string<Args...> format, Args&&... args)
      : error_message(fmt::format(format, std::forward<Args>(args)...)) {}

  template <typename... Args>
  void append(fmt::format_string<Args...> format, Args&&... args) {
    error_message.append("\n");
    error_message.append(fmt::format(format, std::forward<Args>(args)...));
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

#define NewErr(...) std::make_shared<ErrorProto>(__VA_ARGS__);

#define AppendErr(err, ...) err->append(__VA_ARGS__);

template <typename T>
class Maybe {
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

