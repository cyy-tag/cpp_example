#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>


struct ReturnObject {
  struct promise_type {
    ReturnObject get_return_object(){ return {}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void unhandled_exception() {}
  };
};

struct Awaiter 
{
  Awaiter(std::coroutine_handle<> * h) {
    hp_ = h;
    std::cout << "construct Awaiter" << std::endl;
  }
  std::coroutine_handle<> *hp_;
  constexpr bool await_ready() const noexcept { return false; }
  void await_suspend(std::coroutine_handle<> h)
  { 
    std::cout << " await_suspend " << std::endl;
    *hp_ = h; 
  }
  constexpr void await_resume() const noexcept {}
};

ReturnObject
counter(std::coroutine_handle<> *continuation_out)
{
  Awaiter a{continuation_out};
  std::cout << " start for" << std::endl;
  for(unsigned i = 0; ; ++i) {
    co_await a;
    std::cout << "counter: " << i << std::endl;
  }
}

signed
main()
{
  std::coroutine_handle<> h;
  counter(&h);
  for(int i = 0; i < 3; i++) {
    std::cout << "In mainl function\n";
    h();
  }
  h.destroy();
}
