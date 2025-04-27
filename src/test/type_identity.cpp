#include <iostream>
#include <type_traits>

template<class T>
void print_type() {
  std::cout << "Type: " << typeid(T).name() << std::endl;
}
// 实现 将类型值进行保存
// template <typename T>
// struct type_identity { typedef T type; };
int main(int argc, char*argv[])
{
  // 直接打印类型
  print_type<int>();

  // 使用 std::type_identity 打印类型
  print_type<std::type_identity<int>::type>();

  return 0;
}
