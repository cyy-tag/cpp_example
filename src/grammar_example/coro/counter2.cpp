#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>

struct ReturnObject2 {
  struct promise_type {
    ReturnObject2 get_return_object() {
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
  // operator std::coroutine_handle<promise_type>() const { return h_; }
  // operator std::coroutine_handle<>() const { return h_; }
  //这样不如使用函数获取指定的数据
  std::coroutine_handle<promise_type>& get_handle() { return h_; }
};

ReturnObject2
counter2()
{
  for(unsigned i = 0; ; ++i) {
    co_await std::suspend_always{};
    std::cout << "counter2: " << i << std::endl;
  }
}

int
main()
{
  ReturnObject2 future = counter2();
  // std::coroutine_handle<> h = counter2();
  for(int i = 0; i < 3; i++) {
    std::cout << "In main function\n";
    future.get_handle()();
    // h();
  }
  future.get_handle().destroy();
  // h.destroy();
}
