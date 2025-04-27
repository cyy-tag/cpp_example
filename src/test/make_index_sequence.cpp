#include <iostream>
#include <utility>

template <typename T, std::size_t... Is>
void initArray(T* arr, std::index_sequence<Is...>) {
    using Expander = int[];
    Expander{(arr[Is] = T())...};
}

// 定义一个函数, 它接受一个 std::tuple 并打印每一个元素
template <typename Tuple, std::size_t... Is>
void printTuple(const Tuple& t, std::index_sequence<Is...>) {
  //fold expression
  ((std::cout << std::get<Is>(t) << ' '), ...);
  //这里的省略号...表示参数包Is中的每个元素都将依次执行前面的表达式
}

// 重载函数模板, 使其能够推断引序列的长度
template <typename... Args>
void printTuple(const std::tuple<Args...>& t) {
  std::cout << sizeof...(Args) << std::endl;
  printTuple(t,
            std::make_index_sequence<sizeof...(Args)>{});
}

//test fold expression
template <typename... Args>
void PrintFoldArgs(Args... args) {
  ((std::cout << args << ' '), ...);
}

int main() {
  int arr[5];
  initArray(arr, std::make_index_sequence<5>());

  for (int i : arr) {
      std::cout << i << ' ';
  }
  std::cout << '\n';

  std::tuple<int, double, std::string, int> myTuple{1, 2.0, "Hello", 2};
  printTuple(myTuple);
  std::cout << '\n';

  PrintFoldArgs(1, 2, 3, "hello", "world");
  std::cout << '\n';
  return 0;
}
