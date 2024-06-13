#include <vector>
#include <string>
#include "test_value.h"

//纯右值：不可取地址
class CTest{
public:
  static const int s_variable_{0};
public:
  void mf() {}
  int c_;
};

int main(int argc, char*argv[])
{
  //字面值，除字符串
  static_assert(is_prvalue<decltype((34))>::value);
  static_assert(is_prvalue<decltype((true))>::value);
  static_assert(is_prvalue<decltype((nullptr))>::value);

  //函数返回值不是引用类型时，返回的为右值
  std::string str("233333");
  static_assert(is_prvalue<decltype((str.substr(1, 2)))>::value);

  //后缀++和后缀--
  int a = 1, b = 1;
  static_assert(is_prvalue<decltype((a++))>::value);
  static_assert(is_prvalue<decltype((b--))>::value);

  //符号运算,结果是临时变量为右值
  static_assert(is_prvalue<decltype((a + b))>::value);
  static_assert(is_prvalue<decltype((a - b))>::value);
  static_assert(is_prvalue<decltype((a % b))>::value);
  static_assert(is_prvalue<decltype((a & b))>::value);
  static_assert(is_prvalue<decltype((a << b))>::value);

  //逻辑运算
  static_assert(is_prvalue<decltype((a&&b))>::value);
  static_assert(is_prvalue<decltype((a || b))>::value);
  static_assert(is_prvalue<decltype((a >= b))>::value);

  //取地址值
  static_assert(is_prvalue<decltype((&a))>::value);

  //a.m m是枚举变量或者非静态成员函数

  //this指针

  //枚举变量

  //lambda 表达式
  static_assert(is_prvalue<decltype(([](int x){ return x * x;}))>::value);

  return 0;
}
