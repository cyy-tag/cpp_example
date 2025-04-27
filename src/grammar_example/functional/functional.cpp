#include <functional>
#include <benchmark/benchmark.h>

#include "proxy.h"

PRO_DEF_MEM_DISPATCH(MemFoo, Foo);

struct TestAble : pro::facade_builder
    ::add_convention<MemFoo, void() noexcept>
    ::support_copy<pro::constraint_level::nontrivial>
    ::build {};

class Test
{
public:
  Test(){}
  void Foo() noexcept { hello(); }
  static void Bar(void*object){ static_cast<Test*>(object)->hello(); }
  virtual void VirFunc(){};
  void hello(){}

private:
  int val;
};

class Deriv : public Test
{
public:
  void VirFunc() override { hello1(); };
  void hello1(){};
};

Test t;
Deriv d;
Test* base = &d; //test virtual
std::function<void(void)> test_func = std::bind(&Test::Foo, &t); //test functional
void (*test_static)(void* object) = &Test::Bar; //test static functional
pro::proxy<TestAble> p = &t;

static void BM_StaticFunc(benchmark::State& state) {
  for(auto _ : state) {
    test_static(static_cast<void*>(&t));
  }
}

static void BM_Functional(benchmark::State& state) {
  for(auto _ : state) {
    test_func();
  }
}

static void BM_NormalFunc(benchmark::State& state) {
  for(auto _ : state) {
    t.Foo();
  }
}

static void BM_VirtualFunc(benchmark::State& state) {
  for(auto _ : state) {
    base->VirFunc();
  }
}

static void BM_ProxyFunc(benchmark::State& state) {
  for(auto _ : state) {
    p->Foo();
  }
}

BENCHMARK(BM_StaticFunc);
BENCHMARK(BM_Functional);
BENCHMARK(BM_NormalFunc);
BENCHMARK(BM_VirtualFunc);
BENCHMARK(BM_ProxyFunc);

BENCHMARK_MAIN();
