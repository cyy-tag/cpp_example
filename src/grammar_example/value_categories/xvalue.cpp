#include <vector>
#include <string>
#include "test_value.h"

// xvalue(expiring value) is a glvalue(generialized lvaue)
// 广义左值-将亡值

class CTest{
public:
  static const int s_variable_{0};
public:
  void mf() {}
  int c_;
};

struct A
{
    int m;
};
 
A&& operator+(A, A);
A&& f();

int main() {
  // a.m 当a是右值，且m是非静态成员数据时 m是一个将亡值
  // a.*m, a 是右值，mp是指向成员数据的指针
  static_assert(is_xvalue<decltype((CTest().c_))>::value);

  //std::move() 符合move条件的表达式
  CTest ctest{};
  static_assert(is_xvalue<decltype((std::move(ctest)))>::value);

  A a;
  A&& ar = static_cast<A&&>(a);
  //a + a 返回的是右值引用类型，是将亡值
  static_assert(is_xvalue<decltype( (a + a) )>::value);
}
