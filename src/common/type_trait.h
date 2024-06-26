#pragma once
#include <cstdint>
#include <memory>
#include <type_traits>
#include <typeinfo>

namespace Lox {

template <typename To, typename From>
std::enable_if_t<std::is_reference_v<To>, To> typeid_cast(From& from) {
  try {
    if ((typeid(From) == typeid(To)) || (typeid(from) == typeid(To)))
      return static_cast<To>(from);
  } catch (const std::exception& e) {
    return nullptr;
  }
  return nullptr;
}

template <typename To, typename From>
std::enable_if_t<std::is_pointer_v<To>, To> typeid_cast(From* from) {
  try {
    if ((typeid(From) == typeid(std::remove_pointer_t<To>)) ||
        (from && typeid(*from) == typeid(std::remove_pointer_t<To>)))
      return static_cast<To>(from);
    else
      return nullptr;
  } catch (const std::exception& e) {
    return nullptr;
  }
  return nullptr;
}

namespace detail {

template <typename T>
struct is_shared_ptr : std::false_type {};

template <typename T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template <typename T>
inline constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

}  // namespace detail

template <typename To, typename From>
std::enable_if_t<detail::is_shared_ptr_v<To>, To> typeid_cast(
    const std::shared_ptr<From>& from) {
  try {
    if ((typeid(From) == typeid(typename To::element_type)) ||
        (from && typeid(*from) == typeid(typename To::element_type)))
      return std::static_pointer_cast<typename To::element_type>(from);
    else
      return nullptr;
  } catch (const std::exception& e) {
    return nullptr;
  }
  return nullptr;
}

template <typename T, typename F>
bool InstanceOf(const F* ptr) {
  auto* ret = typeid_cast<const T*>(ptr);
  if (ret) return true;
  return false;
}

template <typename T, typename F>
bool InstanceOf(const std::shared_ptr<F>& ptr) {
  auto ret = typeid_cast<const T*>(ptr.get());
  if (ret) return true;
  return false;
}

namespace guard {

using Guard = int;

template <bool enable>
using TypeGuard = std::enable_if_t<enable, Guard>;

}  // namespace guard

}  // namespace Lox
