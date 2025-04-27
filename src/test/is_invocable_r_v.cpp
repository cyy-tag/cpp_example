#include <iostream>
#include <type_traits>

//std::is_invocable_r_v在编译时检查是否可以
//将给定的可调用对象(函数、函数对象、成员函数指针等)
int Func(int) { return 0; }

//std::is_nothrow_invocable_r_v在编译时检查是否可以不抛出异常
//地调用一个可调用对象（函数、函数对象或成员函数指针）

int MyFunction(int) noexcept {
  return 1;
}

int main(int argc, char*argv[])
{
  static_assert(std::is_invocable_r_v<int, decltype(Func), int>,
              "Func should be invocalbe with int argument");
  static_assert(std::is_nothrow_invocable_r_v<int, decltype(MyFunction), int>,
                "MyFunction should be invocable without throwing exceptions");
  //不抛出异常的可调用对象，肯定满足可调用对象
  static_assert(std::is_invocable_r_v<int, decltype(MyFunction), int>,
              "statisfy nothrown must statisfy invocalbe");
  std::cout << "MyFunction is noexcept invocable with int argument\n";
  return 0;
}
