#pragma once
#include <memory>
#include <queue>
#include <functional>

template<typename T>
class ObjectPool
{
public:
  using CreateObjecFunc = std::function<std::unique_ptr<T>()>;
  ObjectPool(size_t size, CreateObjecFunc func) 
   :create_cnt_(0), create_func_(std::move(func))
  {
    for(size_t i = 0; i < size; i++) {
      q_.push(create_func_());
    }
    create_cnt_ = size;
    pool_cnt_ = size;
  }

  inline std::unique_ptr<T> Get() {
    if(q_.empty()) {
      ++create_cnt_;
      return create_func_();
    }
    std::unique_ptr<T> ret = std::move(q_.front());
    --pool_cnt_;
    q_.pop();
    return ret;
  }

  inline void Set(std::unique_ptr<T> obj) {
    ++pool_cnt_;
    q_.push(std::move(obj));
  }

private:
  size_t create_cnt_;
  size_t pool_cnt_;
  CreateObjecFunc create_func_;
  std::queue<std::unique_ptr<T>> q_;
};
