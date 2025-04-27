#pragma once
#include <iostream>

template<typename T, typename... Types>
void print (T firstArg, Types... args)
{
std::cout << firstArg << '\n';
if constexpr(sizeof...(args) > 0) {
  print(args...);
}
  // error if sizeof...(args)==0
  // and no print() for no arguments declared
}
