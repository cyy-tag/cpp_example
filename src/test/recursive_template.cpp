#include <iostream>
#include <type_traits>

// 基础案例: 没有类型参数
template<typename... Ts>
struct Length {
  static constexpr std::size_t value = 0;
};

// 递归案例: 至少有一个类型参数
template<typename T, typename... Ts>
struct Length<T, Ts...> {
  // 递归调用 Length<Ts...> 并加上当前类型的数量1
  static constexpr std::size_t value = 1 + Length<Ts...>::value;
};

int main() {
  std::cout << "Length of void: " << Length<void>::value << std::endl;
  std::cout << "Length of int, double: " << Length<int, double>::value << std::endl;
  std::cout << "Length of char, int, float, double: " << Length<char, int, float, double>::value << std::endl;
  
  return 0;
}
