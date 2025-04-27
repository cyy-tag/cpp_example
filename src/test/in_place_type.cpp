#include <any>
#include <iostream>
#include <string>
#include <variant>

int main() {
  // 正确使用 std::in_place_type 原位构造 std::any 中的 int 类型
  std::any a(std::in_place_type<int>, 42);
  if (std::any_cast<int>(&a) != nullptr) {
      std::cout << "std::any contains int: " << std::any_cast<int>(a) << std::endl;
  }
  //工作原理：使用模板和构造函数重载来区分不同的构造方式，并实现原位构造。
  return 0;
}
