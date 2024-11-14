#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>

struct Future {
  class promise_type {
  public:

    promise_type() {
      std::cout << "call promise_type default construct" << std::endl;
    }

    void* operator new(std::size_t size)
    {
      std::cout << "operator new size: " << size << " promise_type size: " << sizeof(promise_type) << std::endl;
      void* ptr = malloc(size);
      std::cout << "malloc address: " << std::hex << ptr << std::endl;
      if(!ptr) throw std::bad_alloc{};
      return ptr;
    }

    void operator delete(void* ptr, std::size_t sz)
    {
      std::cout << "free address: " << std::hex << ptr << std::endl;
      free(ptr);
    }

    /* get_return_object在开始执行协程函数代码之前执行
     * 这是因为协程有可能在resume之前销毁在当前线程或其他线程
     * 销毁后再调用get_return_object()会引发异常
     */
    Future get_return_object() {
      std::cout << "call promise get_return_object" << std::endl;
      return {
        .h_ = std::coroutine_handle<promise_type>::from_promise(*this)
      };
    }

    /*
     * 在协程初始化好后, 以及调用完get_return_object获得返回对象后
     * 执行co_await promise.initial_suspend()逻辑
     * 允许在执行协程函数代码之前选择是否进行挂起
     */
    std::suspend_never initial_suspend() 
    { 
      std::cout << "call promise initial_suspend" << std::endl;
      return {};
    }

    /*
     * 尽可能在final_syspend挂起，迫使在外部调用destroy销毁协程
     * 这使编译器更容易确定协程帧生命周期的作用域，进行优化
     * 执行final_suspend后，协程只能执行destroy进行销毁
     * resume是ub行为
     */
    std::suspend_never final_suspend() noexcept { return {}; }
    void unhandled_exception() {}

  };

  void Resume() { h_.resume(); }
  void Destroy() { h_.destroy(); }

  std::coroutine_handle<promise_type> h_;
};

struct Awaiter
{
  Awaiter() {
    std::cout << "call Awaiter Construct" << std::endl;
  }

  bool await_ready() const noexcept
  {
    std::cout << "call await_ready" << std::endl;
    return false;
  }

  void await_suspend(std::coroutine_handle<>)
  {
    std::cout << "call await_suspend" << std::endl;
  }

  constexpr void await_resume() const noexcept {}

};

Future
CoroTest()
{
  std::cout << "Start Exec CoroTest" << std::endl;
  for(unsigned i = 0; ; ++i) {
    co_await Awaiter{};
  }
}

int
main()
{
  Future f = CoroTest();
  for(int i = 0; i < 3; ++i) {
    std::cout << " main " << i << std::endl;
    f.Resume();
  }
  f.Destroy();
}
