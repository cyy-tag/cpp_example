#include <tuple>
#include <concepts>
#include <iostream>
#include <utility>

int main() {
  std::tuple<int, double, std::string> myTuple{1, 2.0, "hello"};

  // 获取第一个元素的类型
  using FirstType = std::tuple_element_t<0, decltype(myTuple)>;
  static_assert(std::is_same<FirstType, int>::value, "FirstType is int");

  // 获取第二个元素的类型
  using SecondType = std::tuple_element_t<1, decltype(myTuple)>;
  static_assert(std::is_same<SecondType, double>::value, "SecondType is double");

  // 获取第三个元素的类型
  using ThirdType = std::tuple_element_t<2, decltype(myTuple)>;
  static_assert(std::is_same<ThirdType, std::string>::value, "ThirdType is std::string");

  return 0;
}
