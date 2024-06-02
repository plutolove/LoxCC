#pragma once
#include <memory>
#include <unordered_map>

#include "boost/preprocessor/cat.hpp"

namespace Lox {

template <typename Base, typename Key, typename... Args>
class AutoRegistrationFactory {
 public:
  template <typename Derived>
  struct RegisterType {
    RegisterType(const Key& k, const Args&... args) {
      AutoRegistrationFactory<Base, Key>::Get().mutable_objects()->emplace(
          k, std::make_shared<Derived>(args...));
    }
  };

  AutoRegistrationFactory()
      : objs_(new std::unordered_map<Key, std::shared_ptr<Base>>()) {}

  static AutoRegistrationFactory<Base, Key>& Get() {
    static AutoRegistrationFactory<Base, Key> obj{};
    return obj;
  }

  static Base* getObj(const Key& key) { return Get().get_obj(key); }

  static std::shared_ptr<Base> getSharedObj(const Key& key) {
    return Get().get_shared_obj(key);
  }

  std::shared_ptr<Base> getShared(const Key& key) const {
    return Get().get_shared_obj(key);
  }

  bool has_obj(const Key& key) const {
    auto iter = objs_->find(key);
    return iter != objs_->end();
  }

 private:
  std::unique_ptr<std::unordered_map<Key, std::shared_ptr<Base>>> objs_;

  Base* get_obj(const Key& key) const {
    auto iter = objs_->find(key);
    if (iter == objs_->end()) return nullptr;
    return iter->second.get();
  }

  std::shared_ptr<Base> get_shared_obj(const Key& key) const {
    auto iter = objs_->find(key);
    if (iter == objs_->end()) return nullptr;
    return iter->second;
  }

  bool has_creators() const { return objs_.get() != nullptr; }

  const std::unordered_map<Key, std::shared_ptr<Base>>& Objects() const {
    return *objs_.get();
  }

  std::unordered_map<Key, std::shared_ptr<Base>>* mutable_objects() {
    if (!objs_) {
      objs_.reset(new std::unordered_map<Key, std::shared_ptr<Base>>());
    }
    return objs_.get();
  }
};

#define REGISTER_VAR_NAME BOOST_PP_CAT(__auto_reg_var_name_, __COUNTER__)

#define REGISTER_CLASS(Key, key, Base, Derived)                    \
  static AutoRegistrationFactory<Base, Key>::RegisterType<Derived> \
  REGISTER_VAR_NAME(key)

#define GET_CLASS(Base, Key, key) \
  AutoRegistrationFactory<Base, Key>::getObj(key)

}  // namespace Lox
