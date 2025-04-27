#pragma once

#include <iostream>
#include <vector>
#include <array>

class Foo
{
public:
  constexpr Foo();
public:
  int a;
};

class Test
{
public:
  void modify() {
    num[0].a = 6;
    num[2].a = 18;
    std::cout << num[2].a << std::endl;
  }
public:
  constexpr static const uint8_t max_mod_{64};
  constinit thread_local static  std::array<Foo, max_mod_> num;
};
