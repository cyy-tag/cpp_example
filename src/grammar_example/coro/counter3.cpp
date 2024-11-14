#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>

//什么时候编译器会创建corotinue_handle和promise的

template<typename PromiseType>
struct GetPromise {
  PromiseType *p_;
  bool await_ready(){ return false; }
  bool await_suspend(std::coroutine_handle<PromiseType> h) {
    p_ = &h.promise();
    return false;
  }
  PromiseType* await_resume() { return p_; }
};

struct ReturnObject3 {
  struct promise_type {
    unsigned value_;
    ReturnObject3 get_return_object() {
      return {
        .h_ = std::coroutine_handle<promise_type>::from_promise(*this)
      };
    }

    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void unhandled_exception() {}
  };

  std::coroutine_handle<promise_type> h_;
  // std::coroutine_handle<promise_type> operator()() const { return h_; }
  // std::coroutine_handle<> operator()() const { return h_; }

  //允许隐式类型转换 -- 不容易理解
  operator std::coroutine_handle<promise_type>() const { return h_; }
  // operator std::coroutine_handle<>() const { return h_; }
  //这样不如使用函数获取指定的数据
  // std::coroutine_handle<promise_type>& get_handle() { return h_; }
};

ReturnObject3
counter3()
{
  auto pp = co_await GetPromise<ReturnObject3::promise_type>{};
  for(unsigned i = 0; ; ++i) {
    pp->value_ = i;
    co_await std::suspend_always{};
  }
}

int
main()
{
  std::coroutine_handle<ReturnObject3::promise_type> h = counter3();
  ReturnObject3::promise_type &promise = h.promise();
  for(int i = 0; i < 3; i++) {
    std::cout << "counter3: " << promise.value_ << std::endl;
    h();
  }
  h.destroy();
}

