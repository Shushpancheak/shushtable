#pragma once
#include <cassert>
#include <functional>

template<typename T>
class CheckObject {
public:
  explicit CheckObject(T* this_obj)
    : this_(this_obj) {
    assert(this_->Check());
  }

  ~CheckObject() {
    assert(this_->Check());
  }

private:
  T* this_;
};

#ifndef NDEBUG
#define $ assert(this != nullptr); CheckObject<std::remove_reference<decltype(*this)>::type>(this);
#else
#define $ ;
#endif