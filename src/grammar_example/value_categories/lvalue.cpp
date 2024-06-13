#include <iostream>
#include <vector>
#include "test_value.h"

// 左值特征属性：具有名称的变量或对象，可以进行取地址操作
class CTest{
public:
  static const int s_variable_{0};
public:
  void mf() {}
  int c_;
};

int main()
{
  CTest ctest{};
  int c = 3;
  //具名的变量
  static_assert(is_lvalue<decltype((ctest))>::value);
  static_assert(is_lvalue<decltype((c))>::value);

  int a = 1, b = 1;
  // 赋值表达式的左边
  a = b, a += b, a %= b;
  static_assert(is_lvalue<decltype((a))>::value);
  static_assert(is_lvalue<decltype((b))>::value);

  // //前缀++和前缀--
  static_assert(is_lvalue<decltype((++a))>::value);
  static_assert(is_lvalue<decltype((--b))>::value);

  // //索引下标取值[]
  std::vector<int> vec(2, 0);
  static_assert(is_lvalue<decltype((vec[0]))>::value);
  static_assert(is_lvalue<decltype((vec[1]))>::value);

  // 解引用
  int *ptr_a = new int{9};
  static_assert(is_lvalue<decltype((*ptr_a))>::value);

  // a.m a->m m不是枚举量和非静态成员函数
  static_assert(is_lvalue<decltype((ctest.c_))>::value);

  // 字符串字面量, 存放在静态存储区(全局静态存储区)
  static_assert(is_lvalue<decltype(("Hello,World!"))>::value);

  // 右值引用遍历 int && p = rvalue p是一个左值
}
