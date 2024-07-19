#pragma once
#include <memory>
#include <deque>

template<typename T, typename ...Args>
class ObjectPool
{
public:
  ObjectPool(size_t size, Args ... args) {
    for(size_t i = 0; i < size; i++) {
      dq_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }
  }

  inline std::unique_ptr<T> Get() {
    std::unique_ptr<T> ret = std::move(dq_.front());
    dq_.pop_front();
    return ret;
  }

  inline void Set(std::unique_ptr<T> obj) {
    dq_.push_back(std::move(obj));
  }

private:
  std::deque<std::unique_ptr<T>> dq_;
};
